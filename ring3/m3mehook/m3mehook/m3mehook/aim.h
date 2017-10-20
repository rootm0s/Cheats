#include <iostream>
#include <string>
#include <vector>
using namespace std;

class CAim
{
public:
	Vector GetBonePosition(CBaseEntity*, int);
	float NormalizePitch(float);
	float Normalize(float);
	QAngle NormalizeAngle(QAngle);
	float AngleDifference(QAngle, QAngle);
	QAngle Smooth(QAngle, QAngle, float);
	bool Trace(Vector, Vector, CBaseEntity*);
	bool CanWallbang(CBaseEntity*, CBaseEntity*, Vector, Vector);
	QAngle CalcAngle(Vector, Vector);
	Vector Predict(CBaseEntity*, Vector);
}; CAim *Aim = new CAim;

Vector CAim::GetBonePosition(CBaseEntity* pEntity, int iBone)
{
	Vector vecBone;
	vecBone.Init();

	matrix3x4a_t mMatrixArray[128];
	pEntity->SetupBones(mMatrixArray, 128, 0x100, Interfaces->Engine->GetLastTimeStamp());
	matrix3x4_t mHitboxMatrix = mMatrixArray[iBone];
	vecBone = Vector(mHitboxMatrix.m_flMatVal[0][3], mHitboxMatrix.m_flMatVal[1][3], mHitboxMatrix.m_flMatVal[2][3]);

	return vecBone;
}

float CAim::NormalizePitch(float flPitch)
{
	return fmod(flPitch + 90, 180) - 90;
}

float CAim::Normalize(float flAngle)
{
	return fmod(flAngle + 180, 360) - 180;
}

QAngle CAim::NormalizeAngle(QAngle angAngle)
{
	angAngle.x = Normalize(angAngle.x);
	angAngle.y = Normalize(angAngle.y);

	if (angAngle.x > 89)
		angAngle.x = 89;
	else if (-89 > angAngle.x)
		angAngle.x = -89;

	if (angAngle.y > 180)
		angAngle.y -= 360;
	else if (-180 > angAngle.y)
		angAngle.y += 360;

	angAngle.z = 0;
	return angAngle;
}

float CAim::AngleDifference(QAngle angAngleA, QAngle angAngleB)
{
	float flXDiff = NormalizePitch(angAngleA.x - angAngleB.x);
	float flYDiff = Normalize(angAngleA.y - angAngleB.y);

	if (0 > flXDiff)
		flXDiff = -flXDiff;

	if (0 > flYDiff)
		flYDiff = -flYDiff;

	float flDiff = flYDiff + flXDiff;
	return flDiff;
}

QAngle CAim::Smooth(QAngle angOriginal, QAngle angDestination, float flSmoothness)
{
	QAngle angSmooth;
	angSmooth.x = angDestination.x - angOriginal.x;
	angSmooth.y = angDestination.y - angOriginal.y;
	angSmooth = Aim->NormalizeAngle(angSmooth);

	angSmooth.x = angOriginal.x + angSmooth.x / 100 * flSmoothness;
	angSmooth.y = angOriginal.y + angSmooth.y / 100 * flSmoothness;
	angSmooth = Aim->NormalizeAngle(angSmooth);
	return angSmooth;
}

bool CAim::Trace(Vector pSource, Vector pDestination, CBaseEntity* pTarget = NULL)
{
	Ray_t rRay;
	trace_t trTrace;
	CTraceFilter trfTraceFilter;
	trfTraceFilter.pSkip = pLocalPlayer;
	rRay.Init(pSource, pDestination);
	
	unsigned int uiMask = 0x4600400B;
	Interfaces->EngineTrace->TraceRay(rRay, uiMask, &trfTraceFilter, &trTrace);
	return (trTrace.fraction == 1.0f || (pTarget != NULL && trTrace.m_pEnt == pTarget));
};

bool CAim::CanWallbang(CBaseEntity* pLocal, CBaseEntity* pEntity, Vector vecStart, Vector vecEnd)
{
	CTraceFilterSkipTwoEntities TraceFilter(pLocal, pEntity);
	Ray_t ray;
	ray.Init(vecStart, vecEnd);

	trace_t trace;
	Interfaces->EngineTrace->TraceRay(ray, MASK_SOLID, (CTraceFilter*)&TraceFilter, &trace);

	Vector vecEndPos[3];
	vecEndPos[0] = trace.endpos;

	ray.Init(vecEnd, vecStart);
	Interfaces->EngineTrace->TraceRay(ray, MASK_SOLID, (CTraceFilter*)&TraceFilter, &trace);

	vecEndPos[1] = trace.endpos;
	VectorSubtract(vecEndPos[0], vecEndPos[1], vecEndPos[2]);

	return (vecEndPos[2].Length() < 17.5f) ? true : false;
}

QAngle CAim::CalcAngle(Vector vecSource, Vector vecDestination)
{
	QAngle vecDelta;

	vecDelta.x = vecSource.x - vecDestination.x;
	vecDelta.y = vecSource.y - vecDestination.y;

	Vector vecViewOrigin = pLocalPlayer->m_vecViewOffset;

	vecDelta.z = vecSource.z + vecViewOrigin.z - vecDestination.z;

	float flHyp = sqrt(vecDelta.x * vecDelta.x + vecDelta.y * vecDelta.y);

	QAngle angAngle;
	angAngle.x = atan(vecDelta.z / flHyp) * 57.295779513082;
	angAngle.y = atan(vecDelta.y / vecDelta.x) * 57.295779513082;
	angAngle.z = 0;

	if (vecDelta.x >= 0.0)
		angAngle.y += 180;

	return angAngle;
}

Vector CAim::Predict(CBaseEntity* pEntity, Vector vecPos)
{
	return vecPos + (pEntity->m_vecVelocity * 0.015);
}