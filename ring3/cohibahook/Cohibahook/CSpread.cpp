#include "main.h"
#define M_PI	3.14159265358979323846f
#define square( x ) ( x * x )

ValveSDK::CBaseCombatWeapon g_CombatWeapon;

float RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return m_RandomFloat(min, max);
}

void RandomSeed(UINT seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(seed);
	return;
}

float CSpread::GetInaccuracy(ValveSDK::CBaseCombatWeapon *pWeapon)
{
	DWORD dwInaccuracyVMT = (*reinterpret_cast<PDWORD_PTR*>(pWeapon))[478]; //459 //464 //477
	return ((float(__thiscall*)(ValveSDK::CBaseCombatWeapon*)) dwInaccuracyVMT)(pWeapon);
	typedef float(__thiscall* GetInaccuracyFn)(PVOID);
}

float CSpread::GetSpread(ValveSDK::CBaseCombatWeapon *pWeapon)
{
	DWORD dwSpreadVMT = (*reinterpret_cast<PDWORD_PTR*>(pWeapon))[479]; //460 //465//478
	return ((float(__thiscall*)(ValveSDK::CBaseCombatWeapon*)) dwSpreadVMT)(pWeapon);
	typedef float(__thiscall* GetSpreadFn)(PVOID);
}

void CSpread::UpdateAccuracyPenalty(ValveSDK::CBaseCombatWeapon *pWeapon)
{
	DWORD dwUpdateVMT = (*reinterpret_cast<PDWORD_PTR*>(pWeapon))[480]; //461 //466//479
	return ((void(__thiscall*)(ValveSDK::CBaseCombatWeapon*)) dwUpdateVMT)(pWeapon);
}


ValveSDK::CBaseCombatWeapon* CSpread::GetBaseCombatActiveWeapon(CBaseEntity *pEntity)
{
	ULONG hHandle = (ULONG)*(PDWORD)((DWORD)pEntity + 0x12C0);
	ValveSDK::CBaseCombatWeapon* pWeapon = (ValveSDK::CBaseCombatWeapon*)g_Valve.pEntList->GetClientEntityFromHandle(hHandle);
	return pWeapon;
}

void CSpread::angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u)
{
	float sp, sy, sr, cp, cy, cr;

	g_Math.sinCos(DEG2RAD(angles[0]), &sp, &cp);
	g_Math.sinCos(DEG2RAD(angles[1]), &sy, &cy);
	g_Math.sinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;

	r[0] = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
	r[1] = -1.0f * sr * sp * sy + -1.0f * cr * cy;
	r[2] = -1.0f * sr * cp;

	u[0] = cr * sp * cy + -sr * -sy;
	u[1] = cr * sp * sy + -sr * cy;
	u[2] = cr * cp;

	angles[2] = 0;
}
void CSpread::vectorAngles(float* forward, float* angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = g_Math.sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f) angles[0] += 360.0f;
		if (angles[1] < 0.0f) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}
__forceinline float AngleNormalize(float x)
{
	x = fmod(x + 180, 360);
	if (x < 0)
		x += 360;
	return x - 180;
}
VOID inline SinCos(float radians, float *sine, float *cosine)
{
	_asm
	{
		fld		DWORD PTR[radians]
			fsincos

			mov edx, DWORD PTR[cosine]
			mov eax, DWORD PTR[sine]

			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
	}
}
VOID SpreadVectorAngles(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}
void SpreadAngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float angle;
	static float sr, sp, sy, cr, cp, cy, cpi = (M_PI / 180);

	angle = angles.y * cpi;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles.x * cpi;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles.z * cpi;
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

void CSpread::angleVectors(Vector angles, Vector &f, Vector &r, Vector &u, bool bNoThanks)
{
	float sp, sy, sr, cp, cy, cr;

	g_Math.sinCos(DEG2RAD(angles[0]), &sp, &cp);
	g_Math.sinCos(DEG2RAD(angles[1]), &sy, &cy);
	g_Math.sinCos(DEG2RAD(angles[2]), &sr, &cr);

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

inline void CrossProductZ(const float* a, const float* b, float* r)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}
void CSpread::SpreadFactor(ValveSDK::CInput::CUserCmd *pCmd, ValveSDK::CBaseCombatWeapon*pWeapon)
{
	Vector vecForward, vecRight, vecDir, vecUp, vecAntiDir;
	float flSpread, flInaccuracy, flSpreadX, flSpreadY;

	Vector qAntiSpread;
	g_Aimbot.NormalizeVector(pCmd->viewangles);
	g_Aimbot.ClampAngles(pCmd->viewangles);

	UpdateAccuracyPenalty(pWeapon);

	flSpread = GetSpread(pWeapon);

	flInaccuracy = GetInaccuracy(pWeapon);
	pCmd->random_seed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;
	RandomSeed((pCmd->random_seed & 0xFF) + 1);


	float flRandPi_1 = RandomFloat(0.0f, 2.0f * M_PI);
	float flRandInaccuracy = RandomFloat(0.0f, flInaccuracy);
	float flRandPi_2 = RandomFloat(0.0f, 2.0f * M_PI);
	float flRandSpread = RandomFloat(0, flSpread);



	flSpreadX = cos(flRandPi_1) * flRandInaccuracy + cos(flRandPi_2) * flRandSpread;
	flSpreadY = sin(flRandPi_1) * flRandInaccuracy + sin(flRandPi_2) * flRandSpread;

	SpreadAngleVectors(pCmd->viewangles, &vecForward, &vecRight, &vecUp);

	vecDir.x = (float)((float)(vecRight.x * flSpreadX) + vecForward.x) + (float)(vecUp.x * flSpreadY);
	vecDir.y = (float)((float)(flSpreadX * vecRight.y) + vecForward.y) + (float)(flSpreadY * vecUp.y);
	vecDir.z = (float)((float)(vecRight.z * flSpreadX) + vecForward.z) + (float)(vecUp.z * flSpreadY);

	vecAntiDir = vecForward + (vecRight * -flSpreadX) + (vecUp * -flSpreadY);

	vecAntiDir.NormalizeInPlace();

	SpreadVectorAngles(vecAntiDir, qAntiSpread);

	pCmd->viewangles = qAntiSpread;
	g_Aimbot.NormalizeVector(pCmd->viewangles);
	g_Aimbot.ClampAngles(pCmd->viewangles);
}

CSpread g_pSpread;