#include "DLLMain.h"
#include "Math.h"

float vGetSpread(CBaseCombatWeapon *pWeapon)
{
	typedef float(__thiscall* GetSpreadFn)(PVOID);
	GetSpreadFn GetSpread = (GetSpreadFn)((*(PDWORD*)pWeapon)[459]);
	return GetSpread(pWeapon);
}

float vGetInaccuracy(CBaseCombatWeapon *pWeapon)
{
	typedef float(__thiscall* GetInaccuracyFn)(PVOID);
	GetInaccuracyFn GetCone = (GetInaccuracyFn)((*(PDWORD*)pWeapon)[460]);
	return GetCone(pWeapon);
}

void vUpdateAccuracyPenalty(CBaseCombatWeapon *pWeapon)
{
	typedef void(__thiscall* UpdateAccuracyPenaltyFn)(PVOID);
	UpdateAccuracyPenaltyFn AccuracyPenalty = (UpdateAccuracyPenaltyFn)((*(PDWORD*)pWeapon)[461]);
	AccuracyPenalty(pWeapon);
}
//void ApplySpreadFix(ValveSDK::CInput::CUserCmd* cmd, CBaseCombatWeapon* pWeapon)
//{
//	int random_seed = (cmd->random_seed & 255) + 1;
//
//	typedef void(__cdecl* RandomSeed_t)(int);
//	static RandomSeed_t pRandomSeed = 0;
//
//	typedef float(__cdecl* RandomFloat_t)(float, float);
//	static RandomFloat_t pRandomFloat = 0;
//
//	float fSpread = vGetSpread(pWeapon);
//	float fInaccuracy = vGetInaccuracy(pWeapon);
//
//	float fA = pRandomFloat(0.0f, 2.0f * M_PI);
//	float fB = pRandomFloat(0.0f, fSpread);
//	float fC = pRandomFloat(0.0f, 2.0f * M_PI);
//	float fD = pRandomFloat(0.0f, fInaccuracy);
//
//	Vector spread;
//
//	spread.x = (cos(fA) * fB) + (cos(fC) * fD);
//	spread.y = (sin(fA) * fB) + (sin(fC) * fD);
//
//	spread.x = -spread.x;
//	spread.y = -spread.y;
//
//	Vector forward, right, up;
//	float vForward[3] = { forward.x, forward.y, forward.z };
//	float vright[3] = { right.x, right.y, right.z };
//	float vup[3] = { up.x, up.y, up.z };
//	float dir[3];
//	source_engine::Math::AngleVectors(cmd->viewangles, vForward, vright, vup);
//
//	dir = vForward + spread.x * vright + spread.y * vup;
//
//	Vector3Normalize(dir);
//
//	float identity[3][3];
//
//	identity[2][0] = 1.0f;
//	identity[2][1] = -spread.x;
//	identity[2][2] = spread.y;
//
//	Vector3Normalize(identity[2]);
//
//	identity[0][0] = 0.0f;
//	identity[0][1] = -spread.x;
//	identity[0][2] = (1.0f / spread.y) + (1.0f / identity[2][2]) + spread.y;
//
//	if (spread.x > 0.0f && spread.y < 0.0f)
//	{
//		if (identity[0][1] < 0.0f)
//			identity[0][1] = -identity[0][1];
//	}
//	else if (spread.x < 0.0f && spread.y < 0.0f)
//	{
//		if (identity[0][1] > 0.0f)
//			identity[0][1] = -identity[0][1];
//	}
//
//	if (identity[0][2] < 0.0f)
//		identity[0][2] = -identity[0][2];
//
//	Vector3Normalize(identity[0]);
//
//	CrossProduct(identity[0], identity[2], identity[1]);
//
//	Vector3Normalize(identity[1]);
//
//	float cross = (identity[1][1] * identity[2][0]) - (identity[1][0] * identity[2][1]);
//	float roll = 0.0f;
//
//	if (cmd->viewangles.x > 84.0f || cmd->viewangles.x < -84.0f)
//		roll = RAD2DEG(atan2f(identity[1][2], sqrtf(cross)));
//	else
//		roll = RAD2DEG(atan2f(identity[1][2], cross));
//
//	if (roll < 0.0f)
//		roll += 360.0f;
//
//	VectorAngles(dir, up, cmd->viewangles);
//	cmd->viewangles.z += roll;
//}
	void CSpread::GetSpreadFix(CBaseEntity* pLocal, UINT seed, Vector& pflInAngles)
	{
		typedef void(__cdecl* RandomSeed_t)(int);
		static RandomSeed_t pRandomSeed = 0;

		typedef float(__cdecl* RandomFloat_t)(float, float);
		static RandomFloat_t pRandomFloat = 0;

		if (!pRandomSeed)
		{
			pRandomSeed = (RandomSeed_t)(GetProcAddress(GetModuleHandleA("vstdlib"), "RandomSeed"));
			if (!pRandomSeed)
				return;
		}

		if (!pRandomFloat)
		{
			pRandomFloat = (RandomFloat_t)(GetProcAddress(GetModuleHandleA("vstdlib"), "RandomFloat"));
			if (!pRandomFloat)
				return;
		}

		if (!pLocal)
			return;

		g_Aimbot.NormalizeVector(pflInAngles);

		CBaseCombatWeapon * m_pWeapon = g_NoSpread.GetBaseCombatActiveWeapon(pLocal);

		pRandomSeed((seed & 0xFF) + 1);
		float flA = pRandomFloat(0.0f, 6.283185f);
		float flB = pRandomFloat(0.0f, vGetSpread(m_pWeapon));
		float flC = pRandomFloat(0.0f, 6.283185f);
		float flD = pRandomFloat(0.0f, vGetInaccuracy(m_pWeapon));
		
		float vForward[3], vRight[3], vUp[3];
		Vector vDir;
		Vector vView, vSpread;

		vSpread.x = (cos(flA) * flB) + (cos(flC) * flD);
		vSpread.y = (sin(flA) * flB) + (sin(flC) * flD);

		Vector qAng = pflInAngles;

		Vector qDifference;
		Vector EstimatedViewAngles;

		float fDiff = 0.0f;
		float fDiffOld = 180.0f;

		for (;;)
		{
			source_engine::Math::AngleVectors(pflInAngles, vForward, vRight, vUp);

			Vector vFor = { vForward[0], vForward[1], vForward[2] };
			Vector vRig = { vRight[0], vRight[1], vRight[2] };
			Vector vUps = { vUp[0], vUp[1], vUp[2] };

			vDir.x = vForward[0] + vSpread.x * vRight[0] + vSpread.y * vUp[0];
			vDir.y = vForward[1] + vSpread.x * vRight[1] + vSpread.y * vUp[1];
			vDir.z = vForward[2] + vSpread.x * vRight[2] + vSpread.y * vUp[2];

			vView.x = 8192.0f * vDir.x;
			vView.y = 8192.0f * vDir.y;
			vView.z = 8192.0f * vDir.z;

			source_engine::Math::VectorAngles(vView, EstimatedViewAngles);

			g_Aimbot.NormalizeVector(EstimatedViewAngles);

			qDifference = qAng - EstimatedViewAngles;

			g_Aimbot.NormalizeVector(qDifference);

			fDiff = sqrt((qDifference.x * qDifference.x) + (qDifference.y * qDifference.y));

			if ((fDiff <= 0.001f) || (fDiff >= fDiffOld))
				break;

			fDiffOld = fDiff;

			pflInAngles += qDifference;

			g_Aimbot.NormalizeVector(pflInAngles); 
			g_Aimbot.ClampAngle(pflInAngles);
		}
	}