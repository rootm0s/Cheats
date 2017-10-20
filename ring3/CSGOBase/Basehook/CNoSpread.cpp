#include "DllMain.h"
CBaseCombatWeapon* CNoSpread::GetBaseCombatActiveWeapon(CBaseEntity *pEntity)
{
	EHANDLE hHandle = (EHANDLE)*(PDWORD)((DWORD)pEntity + NetVars::Player::m_hActiveWeapon);
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pEntList->GetClientEntityFromHandle(hHandle);
	return pWeapon;
}
float GetSpread(CBaseCombatWeapon *pWeapon)
{
	typedef float(__thiscall* GetSpreadFn)(PVOID);
	GetSpreadFn GetSpread = (GetSpreadFn)((*(PDWORD*)pWeapon)[459]);
	return GetSpread(pWeapon);
}

float GetInaccuracy(CBaseCombatWeapon *pWeapon)
{
	typedef float(__thiscall* GetInaccuracyFn)(PVOID);
	GetInaccuracyFn GetCone = (GetInaccuracyFn)((*(PDWORD*)pWeapon)[460]);
	return GetCone(pWeapon);
}

void UpdateAccuracyPenalty(CBaseCombatWeapon *pWeapon)
{
	typedef void(__thiscall* UpdateAccuracyPenaltyFn)(PVOID);
	UpdateAccuracyPenaltyFn AccuracyPenalty = (UpdateAccuracyPenaltyFn)((*(PDWORD*)pWeapon)[461]);
	AccuracyPenalty(pWeapon);
}

int CNoSpread::GetWeaponId(CBaseCombatWeapon *pWeapon)
{
	typedef int(__thiscall* GetWeaponIDFn)(PVOID);
	GetWeaponIDFn GetID = (GetWeaponIDFn)((*(PDWORD*)pWeapon)[433]); //with index it's better
	return GetID(pWeapon);
}

void sinCos(float radians, PFLOAT sine, PFLOAT cosine)
{
	__asm
	{
		fld dword ptr[radians]
			fsincos
			mov edx, dword ptr[cosine]
			mov eax, dword ptr[sine]
			fstp dword ptr[edx]
			fstp dword ptr[eax]
	}
}

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )

void CNoSpread::AngleVectors2(const QAngle &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sinCos(DEG2RAD(angles.y), &sy, &cy);
	sinCos(DEG2RAD(angles.x), &sp, &cp);

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

void CNoSpread::angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u)
{
	float sp, sy, sr, cp, cy, cr;

	sinCos(DEG2RAD(angles[0]), &sp, &cp);
	sinCos(DEG2RAD(angles[1]), &sy, &cy);
	sinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;

	r[0] = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
	r[1] = -1.0f * sr * sp * sy + -1.0f * cr * cy;
	r[2] = -1.0f * sr * cp;

	u[0] = cr * sp * cy + -sr * -sy;
	u[1] = cr * sp * sy + -sr * cy;
	u[2] = cr * cp;
}

float sseSqrt(float x)
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
			movss root, xmm0
	}

	return root;
}

void CNoSpread::vectorAngles(float* forward, float* angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f) angles[0] += 360.0f;
		if (angles[1] < 0.0f) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}

float AngleNormalize(float angle)
{
	while (angle < -180)    angle += 360;
	while (angle > 180)    angle -= 360;

	return angle;
}

//
//void CNoSpread::applySpread ( int seed, PFLOAT pflInAngles, PFLOAT pflOutAngles )
//{
//	pflInAngles [0] = AngleNormalize ( pflInAngles [0] );
//	pflInAngles [1] = AngleNormalize ( pflInAngles [1] );
//
//
//	typedef void ( __cdecl* RandomSeed_t )( int );
//	static RandomSeed_t pRandomSeed = 0;
//
//	typedef float ( __cdecl* RandomFloat_t )( float, float );
//	static RandomFloat_t pRandomFloat = 0;
//
//	if ( !pRandomSeed )
//	{
//		pRandomSeed = ( RandomSeed_t ) ( GetProcAddress ( GetModuleHandle ( "vstdlib" ), "RandomSeed" ) );
//		if ( !pRandomSeed )
//			return;
//	}
//
//	if ( !pRandomFloat )
//	{
//		pRandomFloat = ( RandomFloat_t ) ( GetProcAddress ( GetModuleHandle ( "vstdlib" ), "RandomFloat" ) );
//		if ( !pRandomFloat )
//			return;
//	}
//
//	CBaseEntity* pLocal = ( CBaseEntity* ) g_pEntList->GetClientEntity ( g_pEngine->GetLocalPlayer ( ) );
//
//	if ( !pLocal )
//		return;
//
//	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon ( pLocal );
//
//	if ( !m_pWeapon )
//		return;
//
//	UpdateAccuracyPenalty ( m_pWeapon );
//
//	pRandomSeed ( ( seed & 0xFF ) + 1 );
//	float flA = pRandomFloat ( 0.0f, 2.0f * M_PI );
//	float flB = pRandomFloat ( 0.0f, GetSpread ( m_pWeapon ) );
//	float flC = pRandomFloat ( 0.0f, 2.0f * M_PI );
//	float flD = pRandomFloat ( 0.0f, GetInaccuracy ( m_pWeapon ) );
//
//	float forward [3], right [3], up [3], vecDir [3];
//	float view [3], spread [3], dest [3];
//
//	spread [0] = ( cos ( flA ) * flB ) + ( cos ( flC ) * flD );
//	spread [1] = ( sin ( flA ) * flB ) + ( sin ( flC ) * flD );
//
//	angleVectors ( pflInAngles, forward, right, up );
//
//	vecDir [0] = forward [0] + spread [0] * right [0] + spread [1] * up [0];
//	view [0] = 8192.0f * vecDir [0];
//
//	vecDir [1] = forward [1] + spread [0] * right [1] + spread [1] * up [1];
//	view [1] = 8192.0f * vecDir [1];
//
//	vecDir [2] = forward [2] + spread [0] * right [2] + spread [1] * up [2];
//	view [2] = 8192.0f * vecDir [2];
//
//	vectorAngles ( view, dest );
//
//	float Difference [3];
//	float fDiff = 0.0f;
//	float fDiffOld = 180.0f;
//
//	for ( ;; )
//	{
//		angleVectors ( dest, forward, right, up );
//		vecDir [0] = forward [0] + spread [0] * right [0] + spread [1] * up [0];
//		view [0] = 8192.0f * vecDir [0];
//		vecDir [1] = forward [1] + spread [0] * right [1] + spread [1] * up [1];
//		view [1] = 8192.0f * vecDir [1];
//		vecDir [2] = forward [2] + spread [0] * right [2] + spread [1] * up [2];
//		view [2] = 8192.0f * vecDir [2];
//
//		float EstimatedViewAngles [3];
//		vectorAngles ( view, EstimatedViewAngles );
//		EstimatedViewAngles[0] = AngleNormalize ( EstimatedViewAngles[0] );
//		EstimatedViewAngles[1] = AngleNormalize ( EstimatedViewAngles[1] );
//
//		Difference[0] = pflInAngles[0] - EstimatedViewAngles[0];
//		Difference [1] = pflInAngles [1] - EstimatedViewAngles [1];
//
//		fDiff = sqrt ( ( Difference[0]*Difference[0] ) + ( Difference[1]*Difference[1] ) );
//
//		if ( ( fDiff <= 0.001f ) || ( fDiff >= fDiffOld ) )
//		{
//			break;
//		}
//		fDiffOld = fDiff;
//		pflOutAngles [0] = dest[0] + Difference[0];
//		pflOutAngles [1] = dest [1] + Difference [1];
//	}
//
//
//	//pflOutAngles [0] = pflInAngles [0] - dest [0];
//	//pflOutAngles [1] = pflInAngles [1] - dest [1];
//}
//

void CNoSpread::NewAngleVectors(const QAngle &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sinCos(DEG2RAD(angles.y), &sy, &cy);
	sinCos(DEG2RAD(angles.x), &sp, &cp);

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}
void CNoSpread::applySpread(int seed, PFLOAT pflInAngles, PFLOAT pflOutAngles)
{
	typedef void(__cdecl* RandomSeed_t)(int);
	static RandomSeed_t pRandomSeed = 0;

	typedef float(__cdecl* RandomFloat_t)(float, float);
	static RandomFloat_t pRandomFloat = 0;

	if (!pRandomSeed)
	{
		pRandomSeed = (RandomSeed_t)(GetProcAddress(GetModuleHandle("vstdlib"), "RandomSeed"));
		if (!pRandomSeed)
			return;
	}

	if (!pRandomFloat)
	{
		pRandomFloat = (RandomFloat_t)(GetProcAddress(GetModuleHandle("vstdlib"), "RandomFloat"));
		if (!pRandomFloat)
			return;
	}

	CBaseEntity* pLocal = (CBaseEntity*)g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!pLocal)
		return;

	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pLocal);

	if (!m_pWeapon)
		return;

	UpdateAccuracyPenalty(m_pWeapon);

	pRandomSeed((seed & 0xFF) + 1);
	float flA = pRandomFloat(0.0f, 2.0f * M_PI);
	float flB = pRandomFloat(0.0f, GetSpread(m_pWeapon));
	float flC = pRandomFloat(0.0f, 2.0f * M_PI);
	float flD = pRandomFloat(0.0f, GetInaccuracy(m_pWeapon));

	float forward[3], right[3], up[3], vecDir[3];
	float view[3], spread[3], dest[3];

	spread[0] = (cos(flA) * flB) + (cos(flC) * flD);
	spread[1] = (sin(flA) * flB) + (sin(flC) * flD);

	angleVectors(pflInAngles, forward, right, up);

	vecDir[0] = forward[0] + spread[0] * right[0] + spread[1] * up[0];
	view[0] = 8192.0f * vecDir[0];

	vecDir[1] = forward[1] + spread[0] * right[1] + spread[1] * up[1];
	view[1] = 8192.0f * vecDir[1];

	vecDir[2] = forward[2] + spread[0] * right[2] + spread[1] * up[2];
	view[2] = 8192.0f * vecDir[2];

	vectorAngles(view, dest);
	

	pflOutAngles[0] = pflInAngles[0] - dest[0];
	pflOutAngles[1] = pflInAngles[1] - dest[1];
	pflOutAngles[2] = 0.0f;
}
