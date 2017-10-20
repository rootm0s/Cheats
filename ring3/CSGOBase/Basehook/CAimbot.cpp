#include "DllMain.h"
CBaseCombatWeapon* GetBaseCombatLocalActiveWeapon(CBaseEntity* pEntity)
{
	EHANDLE hHandle = (EHANDLE)*(PDWORD)((DWORD)pEntity + NetVars::Player::m_hActiveWeapon);
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pEntList->GetClientEntityFromHandle(hHandle);
	return pWeapon;
}
/*
static void GetMaterialParameters(int iMaterial, float &flPenetrationModifier, float &flDamageModifier)
{
	switch (iMaterial)
	{
	case CHAR_TEX_METAL:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_DIRT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_CONCRETE:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.25;
		break;
	case CHAR_TEX_GRATE:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.99;
		break;
	case CHAR_TEX_VENT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_TILE:
		flPenetrationModifier = 0.65;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_COMPUTER:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_WOOD:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.6;
		break;
	default:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.5;
		break;
	}
}

bool CAimbot::AutoWall(const Vector &vecAbsStart, Vector &vecAbsEnd, CBaseEntity* pLocal, float flDamage)
{
	ValveSDK::CTrace::Ray_t ray;
	ValveSDK::CTrace::trace_t tr;
	ValveSDK::CTrace::CTraceFilter filter;
	filter.pSkip = pLocal;
	CBaseEntity* pBaseEntity = NULL;
	float flTravelledDistance = 0.f, flTempLength = 0.f, flLength, flPenetrationModifier;
	int iLocalID = g_pEngine->GetLocalPlayer();
	Vector vecDir = (vecAbsEnd - vecAbsStart), vecSrc, vecEnd, tmpVec;
	VectorCopy(vecAbsStart, vecSrc);
	flLength = vecDir.Length();
	vecDir /= flLength;

	CBaseCombatWeapon* pWeapon = GetBaseCombatLocalActiveWeapon(pLocal);
	if (!pWeapon) return false;

	int iWeapon = g_NoSpread.GetWeaponId(pWeapon);
	if (iWeapon == 0) return false;

	WeaponInfo_s WeaponInfo = GetWeaponInfo(iWeapon, pWeapon, pLocal);

	float flMustDamage = WeaponInfo.fCurrentDamage * flDamage;

	float flDamageModifier = 0.5f;
	while (WeaponInfo.iPenetration || WeaponInfo.fCurrentDamage > 0)
	{
		vecEnd = vecSrc + vecDir * 8.f;

		if (flTravelledDistance > WeaponInfo.flDistance) return false;

		ray.Init(vecSrc, vecEnd);
		g_pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

		if (tr.m_pEnt && tr.m_pEnt->GetIndex() != iLocalID)
		{
			pBaseEntity = (CBaseEntity*)tr.m_pEnt;
			filter.pSkip = pBaseEntity;
		}

		g_pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

		flTravelledDistance += 8.f;

		if (tr.fraction != 1.0f)
		{
			surfacedata_t* pData = g_pPhysics->GetSurfaceData(tr.surface.surfaceProps);
			GetMaterialParameters(pData->game.material, flPenetrationModifier, flDamageModifier);

			VectorSubtract(tr.endpos, vecAbsStart, tmpVec);
			flTempLength = tmpVec.Length();

			WeaponInfo.fCurrentDamage *= (pow(WeaponInfo.flRangeModifier, (tr.fraction * flTempLength) / 500));
			WeaponInfo.fCurrentDamage *= flDamageModifier;

			if (8.f > (WeaponInfo.flPenetrationPower * flPenetrationModifier)) return false;
			if (flTravelledDistance > WeaponInfo.flPenetrationDistance) return false;

			WeaponInfo.flPenetrationPower -= 8.f / flPenetrationModifier;
			WeaponInfo.flDistance = (WeaponInfo.flDistance - flTravelledDistance) * 0.5f;

			if (flTempLength >= flLength)
			{
				if (WeaponInfo.fCurrentDamage >= flMustDamage)
					return true;
				else
					return false;
			}

			WeaponInfo.iPenetration--;
		}
		else
		{
			VectorSubtract(tr.endpos, vecAbsStart, tmpVec);

			flTempLength = tmpVec.Length();

			if (flTempLength >= flLength)
			{
				if (WeaponInfo.fCurrentDamage >= flMustDamage)
					return true;
				else
					return false;
			}
		}
		vecSrc = vecEnd;
	}
	return false;
}
*/

bool CAimbot::isValidPlayer(CBaseEntity* pBaseEntity)
{

	if(pBaseEntity  && !pBaseEntity->IsDormant() && pBaseEntity->GetLifeState() == LIFE_ALIVE && pBaseEntity->GetHealth() > 0 )
	{
		return true;
	}
	return false;	
}
float _VectorNormalize(Vector &vec)
{
	float radius = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

	// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
	float iradius = 1.f / (radius + FLT_EPSILON);

	vec.x *= iradius;
	vec.y *= iradius;
	vec.z *= iradius;

	return radius;
}
Vector CAimbot::DecayPunchangle(Vector vPunchAngle)
{
	float len = _VectorNormalize(vPunchAngle);
	len -= (10.0 + len * 0.5) * g_pGlobalVars->interval_per_tick;
	len = max(len, 0.0);
	vPunchAngle *= len;
	return vPunchAngle;
}
bool IsSpawnProtected(CBaseEntity *pEnt)
{
	ValveSDK::Color clrRender = *(ValveSDK::Color*)((DWORD)pEnt+0x70);
	int r,g,b,a;
	clrRender.GetColor(r,g,b,a);

	if(r != 255)
		return false;

	return true;
}
//========================================================================================
// Create Vector
//========================================================================================
void CalcAngle(Vector &src, Vector &dst, QAngle &angles)
{
	QAngle delta = QAngle((src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]));
	double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = atan(delta[2] / hyp)		*(180.0 / M_PI);
	angles[1] = atan(delta[1] / delta[0])	*(180.0 / M_PI);
	angles[2] = 0.0f;
	if (delta[0] >= 0.0) { angles[1] += 180.0f; }
}
void MakeVector(QAngle angle, QAngle& vector)
{
	float pitch = float(angle[0] * M_PI / 180);
	float yaw = float(angle[1] * M_PI / 180);
	float tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}
//========================================================================================
// FOV-Calculation by Kolo
//========================================================================================
float CAimbot::GetFov(Vector vLocalOrigin, Vector vPosition, Vector vForward)
{
	Vector vLocal;

	VectorSubtract(vPosition, vLocalOrigin, vLocal);

	vLocal.NormalizeInPlace();

	float fValue = vForward.Dot(vLocal);

	//np for kolo's math skills
	if (fValue < -1.0f)
		fValue = -1.0f;

	if (fValue > 1.0f)
		fValue = 1.0f;

	return RAD2DEG(acos(fValue));
}
//========================================================================================
// Old FOV-Calculation from 2007
//========================================================================================
float CAimbot::GetFov2(QAngle angle, Vector src, Vector dst){
	QAngle ang, aim;
	float fov;
	CalcAngle(src, dst, ang);
	MakeVector(angle, aim);
	MakeVector(ang, ang);
	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI);
	return fov;
}
//========================================================================================
// SetupBones
//========================================================================================
bool CAimbot::SetupBones ( CBaseEntity *pAnimating, matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
	bool bRet;
	__asm
	{
		PUSH currentTime
			PUSH boneMask
			PUSH nMaxBones
			PUSH pBoneToWorldOut
			MOV ECX, DWORD PTR DS : [pAnimating]
			ADD ECX, 4
			MOV EDX, DWORD PTR DS : [pAnimating]
			MOV EAX, DWORD PTR DS : [EDX + 4]
			MOV EDX, DWORD PTR DS : [EAX + 0x34]
			CALL EDX
			MOV bRet, AL
	}
	return bRet;
}
//========================================================================================
// Normalizing
//========================================================================================
void CAimbot::Normalize(float& flValue)
{
	if (flValue < -180.0f) flValue += 360.0f;
	if (flValue >  180.0f) flValue -= 360.0f;
}
void CAimbot::NormalizeVector(Vector& angle)
{
		for (int axis = 0; axis < 3; ++axis)
		{
			while (angle[axis] > 180.f)
			{
				angle[axis] -= 360.f;
			}

			while (angle[axis] < -180.f)
			{
				angle[axis] += 360.f;
			}
		}

		angle[3] = 0.0f;
}


FORCEINLINE float DotProduct ( const float *v1, const float *v2 )
{
	return v1 [0] * v2 [0] + v1 [1] * v2 [1] + v1 [2] * v2 [2];
}

enum Hibtox_e
{
	PELVIS = 0,
	L_THIGH,
	L_CALF,
	L_FOOT,
	R_THIGH,
	R_CALF,
	R_FOOT,
	SPINE1,
	SPINE2,
	SPINE3,
	NECK,
	HEAD,
	L_UPPERARM,
	L_FOREARM,
	L_HAND,
	R_UPPERARM,
	R_FOREARM,
	R_HAND,
};

struct Hitbox_t
{
	int       iBone;
	Vector    vMin;
	Vector    vMax;
};

Hitbox_t    Hitbox [18];

void SetupHitbox ( void )
{
	Hitbox [PELVIS].vMin = Vector ( -6.420f, -4.280f, -6.955f );
	Hitbox [PELVIS].vMax = Vector ( 4.280f, 4.280f, 6.955f );
	Hitbox [PELVIS].iBone = 0;

	Hitbox [L_THIGH].vMin = Vector ( 5.029f, -3.959f, -2.140f );
	Hitbox [L_THIGH].vMax = Vector ( 22.149f, 3.424f, 4.280f );
	Hitbox [L_THIGH].iBone = 26;

	Hitbox [L_CALF].vMin = Vector ( 2.675f, -3.210f, -2.247f );
	Hitbox [L_CALF].vMax = Vector ( 19.260f, 2.675f, 2.247f );
	Hitbox [L_CALF].iBone = 27;

	Hitbox [L_FOOT].vMin = Vector ( 2.943f, -2.675f, -2.408f );
	Hitbox [L_FOOT].vMax = Vector ( 6.688f, 9.095f, 2.408f );
	Hitbox [L_FOOT].iBone = 28;

	Hitbox [R_THIGH].vMin = Vector ( 5.029f, -3.959f, -4.280f );
	Hitbox [R_THIGH].vMax = Vector ( 22.149f, 3.424f, 2.140f );
	Hitbox [R_THIGH].iBone = 23;

	Hitbox [R_CALF].vMin = Vector ( 5.029f, -3.959f, -4.280f );
	Hitbox [R_CALF].vMax = Vector ( 22.149f, 3.424f, 2.140f );
	Hitbox [R_CALF].iBone = 24;

	Hitbox [R_FOOT].vMin = Vector ( 2.943f, -2.675f, -2.408f );
	Hitbox [R_FOOT].vMax = Vector ( 6.688f, 9.095f, 2.408f );
	Hitbox [R_FOOT].iBone = 25;

	Hitbox [SPINE1].vMin = Vector ( -4.280f, -4.280f, -5.885f );
	Hitbox [SPINE1].vMax = Vector ( 3.210f, 5.350f, 5.885f );
	Hitbox [SPINE1].iBone = 2;

	Hitbox [SPINE2].vMin = Vector ( -4.280f, -4.280f, -5.885f );
	Hitbox [SPINE3].vMax = Vector ( 3.210f, 5.350f, 5.885f );
	Hitbox [SPINE3].iBone = 3;

	Hitbox [SPINE3].vMin = Vector ( -4.280f, -4.280f, -5.885f );
	Hitbox [SPINE3].vMax = Vector ( 6.420f, 5.350f, 5.885f );
	Hitbox [SPINE3].iBone = 4;

	Hitbox [NECK].vMin = Vector ( -1.070f, -2.622f, -2.151f );
	Hitbox [NECK].vMax = Vector ( 3.103f, 2.622f, 2.151f );
	Hitbox [NECK].iBone = 5;

	Hitbox [HEAD].vMin = Vector ( -2.140f, -3.745f, -3.670f );
	Hitbox [HEAD].vMax = Vector ( 6.955f, 3.745f, 3.670f );
	Hitbox [HEAD].iBone = 10;

	Hitbox [L_UPPERARM].vMin = Vector ( -2.675f, -3.210f, -2.140f );
	Hitbox [L_UPPERARM].vMax = Vector ( 12.840f, 3.210f, 2.140f );
	Hitbox [L_UPPERARM].iBone = 7;

	Hitbox [L_FOREARM].vMin = Vector ( -0.000f, -2.140f, -2.140f );
	Hitbox [L_FOREARM].vMax = Vector ( 9.630f, 2.140f, 2.140f );
	Hitbox [L_FOREARM].iBone = 8;

	Hitbox [L_HAND].vMin = Vector ( -1.723f, -1.220f, -1.380f );
	Hitbox [L_HAND].vMax = Vector ( 4.473f, 1.220f, 1.380f );
	Hitbox [L_HAND].iBone = 9;

	Hitbox [R_UPPERARM].vMin = Vector ( -2.675f, -3.210f, -2.140f );
	Hitbox [R_UPPERARM].vMax = Vector ( 12.840f, 3.210f, 2.140f );
	Hitbox [R_UPPERARM].iBone = 13;

	Hitbox [R_FOREARM].vMin = Vector ( -0.000f, -2.140f, -2.140f );
	Hitbox [R_FOREARM].vMax = Vector ( 9.630f, 2.140f, 2.140f );
	Hitbox [R_FOREARM].iBone = 14;

	Hitbox [R_HAND].vMin = Vector ( -1.723f, -1.220f, -1.380f );
	Hitbox [R_HAND].vMax = Vector ( 4.473f, 1.220f, 1.380f );
	Hitbox [R_HAND].iBone = 15;
}

void VectorTransformm ( const float *in1, const matrix3x4& in2, float *out )
{
	Assert ( s_bMathlibInitialized );
	Assert ( in1 != out );
	out [0] = DotProduct ( in1, in2 [0] ) + in2 [0][3];
	out [1] = DotProduct ( in1, in2 [1] ) + in2 [1][3];
	out [2] = DotProduct ( in1, in2 [2] ) + in2 [2][3];
}
inline void CAimbot::VectorTransform(const Vector& in1, const matrix3x4 &in2, Vector &out)
{
	VectorTransformm(&in1.x, in2, &out.x);
}
bool CAimbot::GetHitbox ( Vector &vec, int hitbox, int index )
{
	static bool bDidOnce = false;

	if ( !bDidOnce )
	{
		SetupHitbox ( );
		bDidOnce = true;
	}

	matrix3x4 matrix [128];
	float min [3];
	float max [3];


	if ( !SetupBones ( g_pEntList->GetClientEntity ( index ), matrix, 128, 0x00000100, 0.0f ) )
		return false;

	float flIn[3] = { Hitbox[hitbox].vMin[0], Hitbox[hitbox].vMin[1], Hitbox[hitbox].vMin[2] };
	float flIn2[3] = { Hitbox[hitbox].vMax[0], Hitbox[hitbox].vMax[1], Hitbox[hitbox].vMax[2] };

	VectorTransformm(flIn, matrix[Hitbox[hitbox].iBone], min);
	VectorTransformm(flIn2, matrix[Hitbox[hitbox].iBone], max);

	vec [0] = ( min	[0] + max [0] ) / 2;
	vec [1] = ( min [1] + max [1] ) / 2;
	vec [2] = ( min [2] + max [2] ) / 2;

	return true;
}
bool CAimbot::GetHitboxPosition(int iHitBox, Vector& vecOrigin, QAngle qAngles, int index)
{
	if (iHitBox < 0 || iHitBox >= 20)
		return false;
	matrix3x4 pmatrix[128];
	Vector vMin, vMax;
	CBaseEntity* ClientEntity = g_pEntList->GetClientEntity(index);
	if (ClientEntity == NULL)
		return false;
	if (ClientEntity->IsDormant())
		return false;
	const model_t * model;
	model = ClientEntity->GetModel();
	if (model)
	{
		studiohdr_t *pStudioHdr = g_pModel->GetStudiomodel(model);
		if (!pStudioHdr)
			return false;
		if (ClientEntity->SetupBones(pmatrix, 128, 0x00000100, GetTickCount()) == false)
			return false;
		mstudiohitboxset_t *set = pStudioHdr->pHitboxSet(0);
		if (!set)
			return false;
		// pointer to the hitbox
		mstudiobbox_t* pbox = NULL;
		pbox = pStudioHdr->pHitbox(iHitBox, 0);
		g_Aimbot.MatrixAngles(pmatrix[pbox->bone], qAngles, vecOrigin);
		VectorTransform(pbox->bbmin, pmatrix[pbox->bone], vMin);
		VectorTransform(pbox->bbmax, pmatrix[pbox->bone], vMax);
		vecOrigin = (vMin + vMax) * 0.5f;
		return true;
	}
	return true;
}
void StrafeCorrection( ValveSDK::CInput::CUserCmd* cmd, Vector& qaView, Vector qaAim)
{
	FLOAT flMove[3];
	flMove[0] = cmd->forwardmove;
	flMove[1] = cmd->sidemove;
	flMove[2] = cmd->upmove;

	Vector vMove( cmd->forwardmove, cmd->sidemove, cmd->upmove );

	FLOAT flMoveNorm[3];
	g_NoSpread.vectorAngles(flMove,flMoveNorm);

	Vector vMoveNorm(flMoveNorm[0],flMoveNorm[1],flMoveNorm[2]);

	Vector vAdjusted = vMoveNorm + (qaAim - qaView);

	FLOAT flAdjusted[3];
	flAdjusted[0] = vAdjusted.x;
	flAdjusted[1] = vAdjusted.y;
	flAdjusted[2] = vAdjusted.z;

	FLOAT flForward[3];
	FLOAT flSide[3];
	FLOAT flUp[3];

	g_NoSpread.angleVectors(flAdjusted,flForward,flSide,flUp);

	Vector vForward(flForward[0],flForward[1],flForward[2]);
	Vector vSet = vForward * vMove.Length();

	cmd->forwardmove = vSet.x;
	cmd->sidemove = vSet.y;


} 

void CAimbot::ClampAngle(Vector& qaAng)
{
		if (qaAng.y > 180)
			qaAng.y -= 360;

		if (qaAng.y < -180)
			qaAng.y += 360;

		if (qaAng.x > 89.0f)
			qaAng.x = 89.0f;

		if (qaAng.x < -89.0f)
			qaAng.x = -89.0f;

		qaAng.z = 0;
}
VOID RCalculateAngles(const Vector& vecOrigin, const Vector& vecEntity, Vector& vecAngles)
{
	float delta[3] = { vecOrigin.x - vecEntity.x, vecOrigin.y - vecEntity.y, vecOrigin.z - vecEntity.z };
	float hyp = (float)sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

	vecAngles[0] = (float)(atan(delta[2] / hyp) * M_PI);
	vecAngles[1] = (float)(atan(delta[1] / delta[0]) * M_PI);
	vecAngles[2] = 0.f;
	if (delta[0] >= 0.f)
	{
		vecAngles[1] += 180.f;
	}
}

VOID RMakeVector(const Vector& vecIn, Vector& vecOut)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = (float)(vecIn.x * M_PI / 180);
	yaw = (float)(vecIn.y * M_PI / 180);
	tmp = (float)cos(pitch);

	vecOut[0] = (float)(-tmp * -cos(yaw));
	vecOut[1] = (float)(sin(yaw)*tmp);
	vecOut[2] = (float)-sin(pitch);
}
float RGetFOV(const Vector& vecAngle, const Vector& vecOrigin, const Vector& vecEntity)
{
	Vector vecAng, vecAim;

	RCalculateAngles(vecOrigin, vecEntity, vecAng);
	RMakeVector(vecAngle, vecAim);
	RMakeVector(vecAng, vecAng);

	float mag_s = sqrt((vecAim.x * vecAim.x) + (vecAim.y * vecAim.y) + (vecAim.z * vecAim.z));
	float mag_d = sqrt((vecAim.x * vecAim.x) + (vecAim.y * vecAim.y) + (vecAim.z * vecAim.z));

	float u_dot_v = vecAim.x * vecAng.x + vecAim.y * vecAng.y + vecAim.z * vecAng.z;

	return (float)acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI);
}
VOID CAimbot::doAim(ValveSDK::CInput::CUserCmd* pCmd)
{
	float flAim[3];	
	CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	Vector vLocalEye = pLocal->GetEyePosition();
	float CurDistance = 8082.0f; //this variable  need to be outside the loop should works now

	INT tSize = g_pEntList->GetHighestEntityIndex();
	for(INT ax = 0;ax < tSize;ax++)
	{
		CBaseEntity* pBaseEntity = g_pEntList->GetClientEntity(ax);

		if(isValidPlayer(pBaseEntity) && !g_Console.IsActive())
		{
			Vector vTargetDest;

			GetHitbox(vTargetDest, g_Console.getValue("aim_hitbox"), ax);

			Vector vAimspot = vTargetDest - vLocalEye;
			
			float flAimspot[3];
			flAimspot[0] = vAimspot.x;
			flAimspot[1] = vAimspot.y;
			flAimspot[2] = vAimspot.z;

			g_NoSpread.vectorAngles(flAimspot,flAim);

			Vector vAim(flAim[0],flAim[1],flAim[2]);

			Vector punchAngle = pLocal->vecGetPunchAngle();

			float Tmp;

			if (g_Console.getValue("aim_targetby") == 1)
				Tmp = GetFov2(pLocal->GetEyeAngles(), pLocal->GetEyePosition(), vTargetDest);
			else
				Tmp = flGetDistance(vTargetDest, pLocal->GetEyePosition());


			if (pBaseEntity->GetTeamNumber() == 3 || pBaseEntity->GetTeamNumber() == 2)
			{
				if (IsVisible(vLocalEye, vTargetDest, pBaseEntity) && GetFov2(pLocal->GetEyeAngles(), pLocal->GetEyePosition(), vTargetDest) <= g_Console.getValue("aim_fov") && pLocal->GetTeamNumber() != pBaseEntity->GetTeamNumber() && !pBaseEntity->IsProtected())
				{
					if (Tmp < CurDistance)
					{ 
						CurDistance = Tmp;
						if (g_Console.getValue("misc_rcs"))
						{
							gVars.misc_recoilreduce = 0;
							Vector vecPunchAngle = pLocal->vecGetPunchAngle();
							float flLength = VectorNormalize(vecPunchAngle);
							flLength -= (12.f + flLength * 0.5f) * g_pGlobalVars->interval_per_tick;

							if (flLength > 0.f)
								vecPunchAngle *= flLength;
							vAim -= vecPunchAngle * 2.f;
						}
						if (g_Console.getValue("misc_scs"))
						{
							float flLocalSpeed = pLocal->GetAbsVelocity().Length();
							if (flLocalSpeed > 0.f && flLocalSpeed < 1.f)
							{
								int iRandomSeed = pCmd->random_seed;
								g_pSpread.GetSpreadFix(pLocal,iRandomSeed,vAim);
							}
						}
						if (g_Console.getValue("aim_smooth_x") > 0)
						{
							float flDelta = pCmd->viewangles.x - vAim.x;
							Normalize(flDelta);
							int smoothx = g_Console.getValue("aim_smooth_x") * 5;
							vAim.x = pCmd->viewangles.x - flDelta / smoothx;
						}
						if (g_Console.getValue("aim_smooth_y") > 0)
						{
							float flDelta = pCmd->viewangles.y - vAim.y;
							Normalize(flDelta);
							int smoothy = g_Console.getValue("aim_smooth_y") * 5;
							vAim.y = pCmd->viewangles.y - flDelta / smoothy;
						}
						StrafeCorrection(pCmd, pCmd->viewangles, vAim);

						g_Aimbot.NormalizeVector(vAim);
						g_Aimbot.ClampAngle(vAim);
						pCmd->viewangles = vAim;

						if (g_Console.getValue("aim_silent") == 0)
							g_pEngine->SetViewAngles(vAim);

						pCmd->buttons |= IN_ATTACK;
					}
				}
			}			
		}
	}
}