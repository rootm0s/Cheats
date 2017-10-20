#include "main.h"
#define M_PI	3.14159265358979323846f

CAimbot g_Aimbot;

void CAimbot::NormalizeVector(Vector& vec)
{
	for (int i = 0; i < 3; ++i)
	{
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}

	vec[2] = 0.f;
}

void CAimbot::ClampAngles(Vector& vecAngles)
{
	if (vecAngles[0] > 89.f)
		vecAngles[0] = 89.f;
	if (vecAngles[0] < -89.f)
		vecAngles[0] = -89.f;
	if (vecAngles[1] > 180.f)
		vecAngles[1] = 180.f;
	if (vecAngles[1] < -180.f)
		vecAngles[1] = -180.f;

	vecAngles[2] = 0.f;
}

void CAimbot::StopMovement(ValveSDK::CInput::CUserCmd* pUserCmd)
{
	//std::string key = "0x" + g_pConsole->getValue("aimbot_stopkey");
	//int value = atoi(key.c_str());
	if (g_pConsole->getValue("aimbot_stopkey") == 0)
	{
		if (g_pConsole->getValue("aimbot_stop") == 1)
		{
			pUserCmd->sidemove = 0.f;
			pUserCmd->forwardmove = 0.f;
		}
		if (g_pConsole->getValue("aimbot_stop") == 2)
		{
			pUserCmd->sidemove = 0.f;
			pUserCmd->forwardmove = 0.f;
			pUserCmd->buttons |= IN_DUCK;
		}
	}
}

bool CAimbot::CanWallbang(CBaseEntity* pLocal, CBaseEntity* pEntity, Vector vecStart, Vector vecEnd)
{
	ValveSDK::EngineTraceClient::CTraceFilterSkipTwoEntities TraceFilter(pLocal, pEntity);
	ValveSDK::EngineTraceClient::Ray_t ray;
	ray.Init(vecStart, vecEnd);

	ValveSDK::EngineTraceClient::trace_t trace;
	g_Valve.g_pEngineTraceClient->TraceRay(ray, MASK_SOLID, (ValveSDK::EngineTraceClient::CTraceFilter*)&TraceFilter, &trace);

	Vector vecEndPos[3];
	vecEndPos[0] = trace.endpos;

	ray.Init(vecEnd, vecStart);
	g_Valve.g_pEngineTraceClient->TraceRay(ray, MASK_SOLID, (ValveSDK::EngineTraceClient::CTraceFilter*)&TraceFilter, &trace);

	vecEndPos[1] = trace.endpos;
	VectorSubtract(vecEndPos[0], vecEndPos[1], vecEndPos[2]);

	return (vecEndPos[2].Length() < 17.5f) ? true : false; //asniper 19.5
}

ValveSDK::CTrace::trace_t trace;
ValveSDK::CTrace::trace_t trace2;
bool bTraceExit(Vector &vStartyPosy, Vector &vDir)
{
	float multiply = 0;
	//int v10 = 0;

	while (multiply <= 90.0f)
	{
		multiply += 4.0f;

		Vector vDaStart = (vDir * multiply) + vStartyPosy;
		Vector vDaEnd = vDaStart - (vDir * 4.0f);

		int point_contents = (g_Valve.pKoloTrace->GetPointContents(vDaStart, 0x4600400B));

		ValveSDK::CTrace::Ray_t ray2;

		if ((point_contents & 0x600400B) && (!(point_contents & 0x40000000)))
		{
			continue;
		}

		ray2.Init(vDaStart, vDaEnd);
		g_Valve.pKoloTrace->TraceRay(ray2, 0x4600400B, NULL, &trace2);

		if (trace2.startSolid && (trace2.surface.flags & 0x8000))
		{
			CBaseEntity *pNewTraceEnt = trace2.m_pEnt;

			ValveSDK::CTrace::CSimpleTraceFilter tracefilter_new(pNewTraceEnt);

			ValveSDK::CTrace::Ray_t ray3;
			ray3.Init(vDaStart, vStartyPosy);
			g_Valve.pKoloTrace->TraceRay(ray3, 0x600400B, (ValveSDK::CTrace::ITraceFilter*)&tracefilter_new, &trace2);

			if ((trace2.fraction < 1.0f || trace2.allSolid || trace2.startSolid) && !trace2.startSolid)
			{
				vDaStart = trace2.endpos;
				return true;
			}

			continue;
		}

		if (!(trace2.fraction < 1.0f || trace2.allSolid || trace2.startSolid) || trace2.startSolid)
		{
			CBaseEntity *pBreakEnt = trace.m_pEnt;

			if (pBreakEnt)
			{
				CBaseEntity* pWorldEnt = g_Valve.pEntList->GetClientEntity(0);

				if (pBreakEnt != pWorldEnt)
				{
					static DWORD dwEntBreakable = NULL;

					if (dwEntBreakable == NULL)
					{
						dwEntBreakable = Base::Utils::PatternSearch("client.dll", (PBYTE)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68","xxxxxxxxxxx", NULL, NULL);
/*#ifdef DEBUGMODE
						char szAddress[256];
						sprintf(szAddress, "EntBreakable: 0x%x", dwEntBreakable);
						Base::Debug::LOG(szAddress);
#endif*/
					}
					bool bRet;

					__asm
					{
						MOV ECX, pBreakEnt
							CALL dwEntBreakable
							MOV bRet, AL
					}
					if (bRet)
						return true;
				}
			}

			continue;
		}

		if (((trace2.surface.flags >> 7) & 1) && !((trace.surface.flags >> 7) & 1))
			continue;

		float fWanker = ((trace2.plane.normal.x * vDir.x) + (trace2.plane.normal.y * vDir.y) + (trace2.plane.normal.z * vDir.z));

		if (fWanker <= 1.0f)
		{
			float fMody = trace2.fraction * 4.0f;

			Vector vGody = vDaStart - (vDir * fMody);

			vDaStart = vGody;
			return true;
		}
	}
	return false;
}


bool CAimbot::bIsPointPenetrable(WeaponInfo_t wiWeaponInfo, Vector vStart, Vector vEnd)
{
	static ValveSDK::CTrace::trace_t traceEmpty;

	trace = traceEmpty;

	static ValveSDK::CTrace::ITraceFilter *pTraceFilter = (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers;

	Vector vSource = vStart, vDir = (vEnd - vStart), vClip;
	vDir.NormalizeInPlace();

	float fTmpLength, fSumDist, fPow;

	float iCurrentDamage = wiWeaponInfo.iDamage;
	//char array1[10];
	//sprintf(array1, "%f", iCurrentDamage);
	//Base::Debug::LOG(array1);

	float fDamageCheck = 1.0f;

	//kolonote:
	//new penetration: this is always 4 now ehi ehi
	int iPenetration = 4;

	static DWORD dwCliptracetoplayers = NULL;

	if (dwCliptracetoplayers == NULL)
	{
		dwCliptracetoplayers = Base::Utils::PatternSearch("client.dll", (PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10","xxxxxxxxxxxxxxxxxxxxxxxx????xxx", NULL, NULL);
/*#ifdef DEBUGMODE
		char szAddress[256];
		sprintf(szAddress,"ClipTraceToPlayers: 0x%x",dwCliptracetoplayers);
		Base::Debug::LOG(szAddress);
#endif*/
	}


	float fRange = Vector(vEnd - vStart).Length();

	//ValveSDK::CTrace::CSimpleTraceFilter pLocalFilter(pLocal);

	//CBaseEntity *pLastHit = NULL;

	while (iCurrentDamage > 0.0f)
	{
		//Base::Debug::LOG("dmg dealed");
		ValveSDK::CTrace::Ray_t ray;
		ray.Init(vSource, vEnd);

		/*ValveSDK::CTrace::ITraceFilter *pCurrentFilter;

		if (pLastHit)
		{
		ValveSDK::CTrace::CTraceFilterSkipTwoEnts pCurrOne(pLocal, pLastHit);
		pCurrentFilter = &pCurrOne;
		}
		else
		{
		pCurrentFilter = &pLocalFilter;
		}*/

		g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, pTraceFilter, &trace);

		vClip = (vDir * 40.0f) + vEnd;


		_asm
		{
			MOV EAX, pTraceFilter
				LEA ECX, trace
				PUSH ECX
				PUSH EAX
				PUSH 0x4600400B
				LEA EDX, vClip
				LEA ECX, vSource
				CALL dwCliptracetoplayers
				ADD ESP, 0x0C
		}

		//if (trace.m_pEnt)
		//{
		//	pLastHit = trace.m_pEnt;
		//}

		if (trace.fraction != 1.0f)
		{
			ValveSDK::surfacedata_t *pSurfaceData = g_Valve.pPhysics->GetSurfaceData(trace.surface.surfaceProps);

			USHORT material = *(USHORT*)((DWORD)pSurfaceData + 84);
			float fPenetrationPowerModifier = *(PFLOAT)((DWORD)pSurfaceData + 76);

			//fTmpLength += trace.fraction * 8192.0f;
			if (iPenetration == 4)
				fTmpLength = fRange * trace.fraction;
			else
				fTmpLength = (fSumDist + ((fRange - fSumDist) * trace.fraction));

			fPow = (pow(wiWeaponInfo.fRangeModifier, (fTmpLength * 0.002f)));

			iCurrentDamage = iCurrentDamage * fPow;

			//kolonote:
			//i just called this the way IDA did idgaf ehi ehi ehi
			bool a5 = (trace.contents >> 3) & 1;

			bool v104 = (trace.surface.flags >> 7) & 1;

			bool v17 = false;

			if (iPenetration == 0 && !a5 && !v104 && material != 71 && material != 89)
				v17 = true;

			if (wiWeaponInfo.fPenetrationPower <= 0.0f || iPenetration <= 0)
				v17 = true;

			trace2 = traceEmpty;

			//kolonote:
			//i know i suck at naming variables -.-''
			Vector vStartyPosy = trace.endpos;

			if (!bTraceExit(vStartyPosy, vDir)
				&& !(g_Valve.pKoloTrace->GetPointContents(vStartyPosy, 0x600400B) & 0x600400B))
			{
				break;
			}

			if (v17)
			{
				break;
			}

			ValveSDK::surfacedata_t *pSurfaceData2 = g_Valve.pPhysics->GetSurfaceData(trace2.surface.surfaceProps);

			USHORT material2 = *(USHORT*)((DWORD)pSurfaceData2 + 84);
			float fPenetrationPowerModifer2 = *(PFLOAT)((DWORD)pSurfaceData2 + 76);

			float v70 = 0.16f;
			float v31;

			if (a5 || v104)
			{
				if (material != 89 && material != 71)
					v31 = 1.0f;
				else
				{
					v31 = 3.0f;
					v70 = 0.05f;
				}
			}
			else
			{
				if (material != 89)
				{
					if (material != 71)
					{
						if (material == 70)
						{
							v31 = wiWeaponInfo.fPenetrationPower;
						}
						else
						{
							/*if (material == 70 && trace.m_pEnt)
							{
							typedef bool(__thiscall* FirstOne_t)(PVOID pThis);

							bool bFirstOne = ((FirstOne_t)(*(PDWORD)(*(PDWORD)(trace.m_pEnt) + 0x260)))(trace.m_pEnt);

							if (bFirstOne)
							{
							typedef int(__thiscall* SecondOne_t)(PVOID pThis);

							int iScndFirst = ((SecondOne_t)(*(PDWORD)(*(PDWORD)(trace.m_pEnt) + 0x15C)))(trace.m_pEnt);



							int iScndSecond = ((SecondOne_t)(*(PDWORD)(*(PDWORD)(g_Aimbot.pCurrLocalPlaya) + 0x15C)))(g_Aimbot.pCurrLocalPlaya);

							if (iScndFirst == iScndSecond)
							break;
							}
							}*/

							v31 = (fPenetrationPowerModifer2 + fPenetrationPowerModifier) * 0.5f;
							v70 = 0.16f;
						}
					}
					else
					{
						v31 = 3.0f;
						v70 = 0.05f;
					}
				}
				else
				{
					v31 = 3.0f;
					v70 = 0.05f;
				}
				/*if(material == 89 || material == 71)
				{
				v31 = 3.0f;
				v70 = 0.05f;
				}
				else
				v31 = (fPenetrationPowerModifer2 + fPenetrationPowerModifier) * 0.5f;*/
			}

			if (material == material2)
			{
				//if((material2 | 2) == 87)
				if (material2 == 87 || material2 == 85)
					v31 = 3.0f;
				else
				{
					if (material2 == 76)
						v31 = 2.0f;
				}
			}

			float v34 = 1.0f;
			v34 /= v31;

			if (v34 < 0.0f)
				v34 = 0.0f;

			float fNewTmpLength = Vector(trace2.endpos - trace.endpos).Length();

			float fMine = 3.0f;
			fMine /= wiWeaponInfo.fPenetrationPower;
			fMine *= 1.25f;

			float v55 = 0.0f;

			if (fMine >= 0.0f)
				v55 = fMine;

			float v35 = v34;
			v35 *= 3.0f;
			v35 *= v55;

			v35 += (iCurrentDamage * v70);

			float fShiz = fNewTmpLength;
			fShiz *= fNewTmpLength;
			fShiz *= v34;
			fShiz /= 24.0f;

			float fBisc = (v35 + fShiz);

			float v69 = 0.0f;

			if (fBisc >= 0.0f)
				v69 = fBisc;

			float fNewy = (iCurrentDamage - v69);

			iCurrentDamage = fNewy;

			if (fNewy < fDamageCheck)
				break;

			vSource = trace2.endpos;

			fSumDist = fTmpLength + fNewTmpLength;

			--iPenetration;
		}
		else
			return true;
	}
	//Base::Debug::LOG("false");
	return false;
}


bool CAimbot::SetupBones(CBaseEntity *pAnimating, matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
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

FORCEINLINE float DotProduct(const float *v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
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

Hitbox_t    Hitbox[18];

void SetupHitbox(void)
{
	Hitbox[PELVIS].vMin = Vector(-6.420f, -4.280f, -6.955f);
	Hitbox[PELVIS].vMax = Vector(4.280f, 4.280f, 6.955f);
	Hitbox[PELVIS].iBone = 0;

	Hitbox[L_THIGH].vMin = Vector(5.029f, -3.959f, -2.140f);
	Hitbox[L_THIGH].vMax = Vector(22.149f, 3.424f, 4.280f);
	Hitbox[L_THIGH].iBone = 26;

	Hitbox[L_CALF].vMin = Vector(2.675f, -3.210f, -2.247f);
	Hitbox[L_CALF].vMax = Vector(19.260f, 2.675f, 2.247f);
	Hitbox[L_CALF].iBone = 27;

	Hitbox[L_FOOT].vMin = Vector(2.943f, -2.675f, -2.408f);
	Hitbox[L_FOOT].vMax = Vector(6.688f, 9.095f, 2.408f);
	Hitbox[L_FOOT].iBone = 28;

	Hitbox[R_THIGH].vMin = Vector(5.029f, -3.959f, -4.280f);
	Hitbox[R_THIGH].vMax = Vector(22.149f, 3.424f, 2.140f);
	Hitbox[R_THIGH].iBone = 23;

	Hitbox[R_CALF].vMin = Vector(5.029f, -3.959f, -4.280f);
	Hitbox[R_CALF].vMax = Vector(22.149f, 3.424f, 2.140f);
	Hitbox[R_CALF].iBone = 24;

	Hitbox[R_FOOT].vMin = Vector(2.943f, -2.675f, -2.408f);
	Hitbox[R_FOOT].vMax = Vector(6.688f, 9.095f, 2.408f);
	Hitbox[R_FOOT].iBone = 25;

	Hitbox[SPINE1].vMin = Vector(-4.280f, -4.280f, -5.885f);
	Hitbox[SPINE1].vMax = Vector(3.210f, 5.350f, 5.885f);
	Hitbox[SPINE1].iBone = 2;

	Hitbox[SPINE2].vMin = Vector(-4.280f, -4.280f, -5.885f);
	Hitbox[SPINE3].vMax = Vector(3.210f, 5.350f, 5.885f);
	Hitbox[SPINE3].iBone = 3;

	Hitbox[SPINE3].vMin = Vector(-4.280f, -4.280f, -5.885f);
	Hitbox[SPINE3].vMax = Vector(6.420f, 5.350f, 5.885f);
	Hitbox[SPINE3].iBone = 4;

	Hitbox[NECK].vMin = Vector(-1.070f, -2.622f, -2.151f);
	Hitbox[NECK].vMax = Vector(3.103f, 2.622f, 2.151f);
	Hitbox[NECK].iBone = 5;

	Hitbox[HEAD].vMin = Vector(-2.140f, -3.745f, -3.670f);
	Hitbox[HEAD].vMax = Vector(6.955f, 3.745f, 3.670f);
	Hitbox[HEAD].iBone = 10;

	Hitbox[L_UPPERARM].vMin = Vector(-2.675f, -3.210f, -2.140f);
	Hitbox[L_UPPERARM].vMax = Vector(12.840f, 3.210f, 2.140f);
	Hitbox[L_UPPERARM].iBone = 7;

	Hitbox[L_FOREARM].vMin = Vector(-0.000f, -2.140f, -2.140f);
	Hitbox[L_FOREARM].vMax = Vector(9.630f, 2.140f, 2.140f);
	Hitbox[L_FOREARM].iBone = 8;

	Hitbox[L_HAND].vMin = Vector(-1.723f, -1.220f, -1.380f);
	Hitbox[L_HAND].vMax = Vector(4.473f, 1.220f, 1.380f);
	Hitbox[L_HAND].iBone = 9;

	Hitbox[R_UPPERARM].vMin = Vector(-2.675f, -3.210f, -2.140f);
	Hitbox[R_UPPERARM].vMax = Vector(12.840f, 3.210f, 2.140f);
	Hitbox[R_UPPERARM].iBone = 13;

	Hitbox[R_FOREARM].vMin = Vector(-0.000f, -2.140f, -2.140f);
	Hitbox[R_FOREARM].vMax = Vector(9.630f, 2.140f, 2.140f);
	Hitbox[R_FOREARM].iBone = 14;

	Hitbox[R_HAND].vMin = Vector(-1.723f, -1.220f, -1.380f);
	Hitbox[R_HAND].vMax = Vector(4.473f, 1.220f, 1.380f);
	Hitbox[R_HAND].iBone = 15;
}

void VectorTransformm(const float *in1, const matrix3x4& in2, float *out)
{
	Assert(s_bMathlibInitialized);
	Assert(in1 != out);
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

inline void VectorTransform(const Vector& in1, const matrix3x4 &in2, Vector &out)
{
	VectorTransformm(&in1.x, in2, &out.x);
}

bool CAimbot::GetHitbox(Vector &vec, int hitbox, int index)
{
	static bool bDidOnce = false;

	if (!bDidOnce)
	{
		SetupHitbox();
		bDidOnce = true;
	}
	matrix3x4 matrix[128];
	float min[3];
	float max[3];

	if (!SetupBones(g_Valve.pEntList->GetClientEntity(index), matrix, 128, 0x00000100, 0.0f))
		return false;

	float flIn[3] = { Hitbox[hitbox].vMin[0], Hitbox[hitbox].vMin[1], Hitbox[hitbox].vMin[2] };
	float flIn2[3] = { Hitbox[hitbox].vMax[0], Hitbox[hitbox].vMax[1], Hitbox[hitbox].vMax[2] };
	VectorTransformm(flIn, matrix[Hitbox[hitbox].iBone], min);
	VectorTransformm(flIn2, matrix[Hitbox[hitbox].iBone], max);
	vec[0] = (min[0] + max[0]) / 2;
	vec[1] = (min[1] + max[1]) / 2;
	vec[2] = (min[2] + max[2]) / 2;
	return true;
}

void StrafeCorrection(ValveSDK::CInput::CUserCmd* cmd, Vector& qaView, Vector qaAim)
{
	FLOAT flMove[3];
	flMove[0] = cmd->forwardmove;
	flMove[1] = cmd->sidemove;
	flMove[2] = cmd->upmove;

	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);

	FLOAT flMoveNorm[3];
	g_Math.vectorAngles(flMove, flMoveNorm);

	Vector vMoveNorm(flMoveNorm[0], flMoveNorm[1], flMoveNorm[2]);

	Vector vAdjusted = vMoveNorm + (qaAim - qaView);

	FLOAT flAdjusted[3];
	flAdjusted[0] = vAdjusted.x;
	flAdjusted[1] = vAdjusted.y;
	flAdjusted[2] = vAdjusted.z;

	FLOAT flForward[3];
	FLOAT flSide[3];
	FLOAT flUp[3];

	g_Math.angleVectors(flAdjusted, flForward, flSide, flUp);

	Vector vForward(flForward[0], flForward[1], flForward[2]);
	Vector vSet = vForward * vMove.Length();

	cmd->forwardmove = vSet.x;
	cmd->sidemove = vSet.y;
}

bool CAimbot::IsAbleToShoot(CBaseEntity* pLocalClientBaseEntity)
{
	if (IsNullPtr(pLocalClientBaseEntity))
		return false;

	if (!pLocalClientBaseEntity->isValidPlayer())
		return false;

	if (IsNullPtr(pLocalClientBaseEntity->GetActiveBaseCombatWeapon()))
		return false;

	float flServerTime = (float)pLocalClientBaseEntity->GetTickBase() * g_Valve.pGlobalVars->interval_per_tick;
	float flNextPrimaryAttack = pLocalClientBaseEntity->GetActiveBaseCombatWeapon()->GetNextPrimaryAttack();

	return(!(flNextPrimaryAttack > flServerTime));
}

void CalculateAngles(Vector& vecOrigin, Vector& vecOther, Vector& vecAngles)
{
	Vector vecDelta = Vector((vecOrigin[0] - vecOther[0]), (vecOrigin[1] - vecOther[1]), (vecOrigin[2] - vecOther[2]));
	float hyp = sqrtf(vecDelta[0] * vecDelta[0] + vecDelta[1] * vecDelta[1]);

	vecAngles[0] = (float)atan(vecDelta[2] / hyp) * (float)(180.f / M_PI);
	vecAngles[1] = (float)atan(vecDelta[1] / vecDelta[0])	* (float)(180.f / M_PI);
	vecAngles[2] = (float)0.f;

	if (vecDelta[0] >= 0.f)
		vecAngles[1] += 180.f;

	vecAngles[2] = 0.f;
}

void MakeVector(Vector& vecAngle, Vector& out)
{
	float pitch = float(vecAngle[0] * M_PI / 180.f);
	float tmp = float(cos(pitch));
	float yaw = float(vecAngle[1] * M_PI / 180.f);
	out[0] = float(-tmp * -cos(yaw));
	out[1] = float(sin(yaw)*tmp);
	out[2] = float(-sin(pitch));
}

float CAimbot::GetFov(Vector& vecAngle, Vector& vecOrigin, Vector& vecOther, bool bDistancebased)
{
	Vector ang, aim;
	double fov;

	CalculateAngles(vecOrigin, vecOther, ang);
	MakeVector(vecAngle, aim);
	MakeVector(ang, ang);

	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.f / M_PI);

	fov *= 1.4;

	if (bDistancebased)
	{
		float xDist = abs(vecOrigin[0] - vecOther[0]);
		float yDist = abs(vecOrigin[1] - vecOther[1]);
		float Distance = sqrt((xDist * xDist) + (yDist * yDist));

		Distance /= 650.f;

		if (Distance < 0.7f)
			Distance = 0.7f;

		if (Distance > 6.5)
			Distance = 6.5;

		fov *= Distance;
	}

	return (float)fov;
}

Vector CAimbot::HitScans(CBaseEntity *pLocal, CBaseEntity *pTarget)
{
	if (pTarget->isValidPlayer())
	{
		Vector vHead;

		int hitboxarray[18] = {0, 7, 8, 9, 10, 11, 1, 4, 2, 5, 3, 6, 12, 15, 13, 16, 14, 17};

		GetHitbox(vHead, g_pConsole->getValue("aimbot_hitbox"), pTarget->GetIndex());

		if ((g_Valve.g_pEngineTraceClient->IsVisible(pLocal, pLocal->GetEyePosition(), vHead, pTarget) || (g_pConsole->getValue("aimbot_autowall") == 1 && bIsPointPenetrable(wiWeaponInfo, pLocal->GetEyePosition(), vHead))))
		{
			return vHead;
		}
		if (g_pConsole->getValue("aimbot_hitscan") == 1)
		{
			for (int iBone = 0; iBone < 18; iBone++)
			{
				Vector vBone;
				GetHitbox(vBone, hitboxarray[iBone], pTarget->GetIndex());

				if ((g_Valve.g_pEngineTraceClient->IsVisible(pLocal, pLocal->GetEyePosition(), vBone, pTarget) || (g_pConsole->getValue("aimbot_autowall") == 1 && bIsPointPenetrable(wiWeaponInfo, pLocal->GetEyePosition(), vBone))))
				{
					return vBone;
				}

			}
		}
		if (g_pConsole->getValue("aimbot_hitscan") == 2)
		{
			for (int iBone = 0; iBone < 6; iBone++)
			{
				Vector vBone;
				GetHitbox(vBone, hitboxarray[iBone], pTarget->GetIndex());

				if ((g_Valve.g_pEngineTraceClient->IsVisible(pLocal, pLocal->GetEyePosition(), vBone, pTarget) || (g_pConsole->getValue("aimbot_autowall") == 1 && bIsPointPenetrable(wiWeaponInfo, pLocal->GetEyePosition(), vBone))))
				{
					return vBone;
				}

			}
		}
	}
	return Vector(0.0f, 0.0f, 0.0f);
}

VOID CAimbot::doAim(ValveSDK::CInput::CUserCmd* pCmd, bool &SendPacket,ValveSDK::CBaseCombatWeapon *pWeapon)
{
	uintptr_t pWeaponData = pWeapon->GetWeaponData();

	wiWeaponInfo.fMaxRange = *(PFLOAT)(pWeaponData + WEAPONDATA_MAXRANGEOFFSET);
	wiWeaponInfo.iDamage = *(PINT)(pWeaponData + WEAPONDATA_DAMAGEOFFSET);
	wiWeaponInfo.fRangeModifier = *(PFLOAT)(pWeaponData + WEAPONDATA_RANGEMODIFIEROFFSET);
	wiWeaponInfo.fPenetrationPower = *(PFLOAT)(pWeaponData + WEAPONDATA_PENETRATIONPOWEROFFSET);

	float flAim[3];
	float MaxDistance = 99999;
	INT besttarget = -1;
	CBaseEntity* pLocal = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());
	Vector vLocalEye = pLocal->GetEyePosition();
	for (INT i = 1; i < g_Valve.pEntList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(i);
		if (pBaseEntity && pBaseEntity->isValidPlayer() && pLocal->isValidPlayer())
		{
			Vector vTargetDest;

			if (g_pConsole->getValue("aimbot_smart") > 0)
			{
				int FiresShots = pLocal->GetiShots();
				if (FiresShots <= g_pConsole->getValue("aimbot_smart"))
				{
					g_pConsole->HandleCommands("aimbot_hitbox 11");
				}
				else
				{
					g_pConsole->HandleCommands("aimbot_hitbox 0");
				}
			}
			if (g_pConsole->getValue("aimbot_hitscan") >= 1)
			{
				vTargetDest = HitScans(pLocal, pBaseEntity);
			}
			else
			{
				GetHitbox(vTargetDest, g_pConsole->getValue("aimbot_hitbox"), i);
			}

			Vector vAimspot = vTargetDest - vLocalEye;

			float flAimspot[3];
			flAimspot[0] = vAimspot.x;
			flAimspot[1] = vAimspot.y;
			flAimspot[2] = vAimspot.z;

			g_Math.vectorAngles(flAimspot, flAim);
			Vector vAim(flAim[0], flAim[1], flAim[2]);

			if (pBaseEntity->GetTeam() == 3 || pBaseEntity->GetTeam() == 2)
			{
				if (GetFov(pCmd->viewangles, vLocalEye, vTargetDest, false) <= g_pConsole->getValue("aimbot_fov") && (g_Valve.g_pEngineTraceClient->IsVisible(pLocal, pLocal->GetEyePosition(), vTargetDest, pBaseEntity) || (g_pConsole->getValue("aimbot_autowall") == 1 && bIsPointPenetrable(wiWeaponInfo, vLocalEye, vTargetDest))) && pLocal->GetTeam() != pBaseEntity->GetTeam())
				{
					if (GetFov(pCmd->viewangles, vLocalEye, vTargetDest, false) <= MaxDistance)
					{
						MaxDistance = GetFov(pCmd->viewangles, vLocalEye, vTargetDest, false);
						besttarget = i;
					}
				}
			}
		}
	}
	CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(besttarget);
	if (pBaseEntity && pBaseEntity->isValidPlayer() && pLocal->isValidPlayer() && besttarget > 0)
	{
		Vector vTargetDest;

		if (g_pConsole->getValue("aimbot_hitscan") >= 1)
		{
			vTargetDest = HitScans(pLocal, pBaseEntity);
		}
		else
		{
			GetHitbox(vTargetDest, g_pConsole->getValue("aimbot_hitbox"), besttarget);
		}

		Vector vAimspot = vTargetDest - vLocalEye;

		float flAimspot[3];
		flAimspot[0] = vAimspot.x;
		flAimspot[1] = vAimspot.y;
		flAimspot[2] = vAimspot.z;

		g_Math.vectorAngles(flAimspot, flAim);
		Vector vAim(flAim[0], flAim[1], flAim[2]);

		if (pBaseEntity->GetTeam() == 3 || pBaseEntity->GetTeam() == 2)
		{
			if (vTargetDest != Vector(0.0f, 0.0f, 0.0f) && GetFov(pCmd->viewangles, vLocalEye, vTargetDest, false) <= g_pConsole->getValue("aimbot_fov") && (g_Valve.g_pEngineTraceClient->IsVisible(pLocal, pLocal->GetEyePosition(), vTargetDest, pBaseEntity) || (g_pConsole->getValue("aimbot_autowall") == 1 && bIsPointPenetrable(wiWeaponInfo, vLocalEye, vTargetDest))) && pLocal->GetTeam() != pBaseEntity->GetTeam())
			{
				Vector vOldView = pCmd->viewangles;
				float flOldSidemove = pCmd->sidemove;
				float flOldForwardmove = pCmd->forwardmove;
				StopMovement(pCmd);
				StrafeCorrection(pCmd, pCmd->viewangles, vAim);
				NormalizeVector(vAim);
				ClampAngles(vAim);
				pCmd->viewangles = vAim;

				if (!pLocal->GetScope() && pWeapon->IsSniper() && g_pConsole->getValue("aimbot_scope") == 1)
				{
					pCmd->buttons = IN_ATTACK2;
				}
				else
				{
					pCmd->buttons |= IN_ATTACK;
				}

				Vector vMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
				float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
				Vector vMove2;
				Vector vRealView(pCmd->viewangles);

				if (g_pConsole->getValue("aimbot_norecoil") == 1)
				{
					Vector vPunchAngle = Vector(0, 0, 0);
					vPunchAngle = pLocal->GetPunchAngle();
					vPunchAngle += vPunchAngle;
					pCmd->viewangles.x -= vPunchAngle.x;
					pCmd->viewangles.y -= vPunchAngle.y;
					NormalizeVector(pCmd->viewangles);
				}

				if (g_pConsole->getValue("aimbot_nospread") == 1)
				{
					g_Math.VectorAngles3D(vMove, vMove2);
					g_pSpread.SpreadFactor(pCmd, pLocal->GetActiveBaseCombatWeapon());
					flYaw = DEG2RAD(pCmd->viewangles.y - vRealView.y + vMove2.y);
					pCmd->forwardmove = cos(flYaw) * flSpeed;
					pCmd->sidemove = sin(flYaw) * flSpeed;
					NormalizeVector(pCmd->viewangles);
				}

				if (g_pConsole->getValue("aimbot_psilent") == 1)
				{
					static int iChokedTicks = -1;
					iChokedTicks++;
					if (IsAbleToShoot(pLocal) && iChokedTicks < g_pConsole->getValue("aimbot_choke"))
					{
						SendPacket = false;
					}
					else
					{
						SendPacket = true;
						iChokedTicks = -1;
						pCmd->viewangles = vOldView;
						NormalizeVector(pCmd->viewangles);
						ClampAngles(pCmd->viewangles);
						pCmd->sidemove = flOldSidemove;
						pCmd->forwardmove = flOldForwardmove;
					}
				}
			}
		}
	}
}
