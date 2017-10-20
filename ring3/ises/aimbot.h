#include "main.h"

#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define MAX_RANDOM_RANGE 0x7FFFFFFFUL

#define AM (1.0/IM)
#define EPS 1.2e-7
#define RNMX (1.0-EPS) 

// --- (Skapad av ViKiNG & Steady) ---
void AngleVectorsx(const Vector angles, Vector* forward) {
	float sp, sy, cp, cy;
	sp = sin(DEG2RAD(angles.x));
	sy = sin(DEG2RAD(angles.y));
	cp = cos(DEG2RAD(angles.x));
	cy = cos(DEG2RAD(angles.y));
	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

float GetFOV(Vector CmdViewAngle, Vector LocalEyePos, Vector Destination) {
	Vector ang, aim;
	CalcAngle(LocalEyePos, Destination, ang);
	AngleVectorsx(CmdViewAngle, &aim);
	AngleVectorsx(ang, &ang);

	float mag_s = std::sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = mag_s;
	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
	float fov = std::acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI_F);
	return fov;
}

void NormaliseViewAngle(Vector &angle)
{
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89.0) angle.x = 89;
	if (angle.x < -89.0) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;
}

// END --- (Skapad av ViKiNG & Steady) ---


bool IsVisible(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
{
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = pEntList->GetClientEntity(pEngine->GetLocalPlayer());

	ray.Init(pLocal->GetEyePos(), pEntity->GetEyePos());
	pEngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

	return (tr.m_pEnt == pEntity || tr.fraction > 0.99f);
}

bool BoneTrace(C_BaseEntity* pLocalClientBaseEntity, Vector vecOrigin, Vector vecFinal, C_BaseEntity* pClientBaseEntity)
{
	Ray_t ray;
	ray.Init(vecOrigin, vecFinal);

	CTraceFilter TraceFilter;

	trace_t trace;
	pEngineTrace->TraceRay(ray, 0x4600400B, &TraceFilter, &trace);

	return(trace.m_pEnt == pClientBaseEntity || trace.fraction > 0.99f);
}

bool GetBonePosition(C_BaseEntity* pEntity, Vector &Hitbox, int Bone) {
	matrix3x4 MatrixArray[128];
	if (!pEntity->SetupBones(MatrixArray, 128, 0x100, 0)) {
		return false;
	}
	Hitbox = Vector(MatrixArray[Bone][0][3], MatrixArray[Bone][1][3], MatrixArray[Bone][2][3]);
	return true;
}

Vector Hitscans(C_BaseEntity* pEntity, C_BaseEntity* pLocal, Vector& Hitbox) {

	int hitboxarray[18] = { 6, 7, 8, 9, 10, 11, 1, 4, 2, 5, 3, 6, 12, 15, 13, 16, 14, 17 };

	for (int i = 0; i < 18; i++) {


		GetBonePosition(pEntity, Hitbox, hitboxarray[i]);
		if (BoneTrace(pLocal, pLocal->GetEyePos(), Hitbox, pEntity)) {
			return Hitbox;
		}
	}

}

void NormalizeVector(Vector& angle)
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

Vector Normalized(Vector& vec) {
	if (vec.x > 180)  vec.x -= 360;
	if (vec.y > 180)  vec.y -= 360;
	if (vec.x < -180) vec.x += 360;
	if (vec.y < -180) vec.y += 360;
	vec.z = 0;
	return vec;
}

void SetSmoothAngles(CUserCmd* pCmd, Vector& dst) {
	if (aim_smooth.getValve()) {
		if (aim_smoothBar.getValue() > 0) {
			Vector delta = pCmd->viewangles - dst;
			Normalized(delta);
			int smoothx = aim_smoothBar.getValue() * 3;
			dst = pCmd->viewangles - delta / smoothx;
			dst.z = 0, delta.z = 0;
		}
	}
}

bool AutoWall(Vector vPos, C_BaseEntity* pLocal, C_BaseEntity* pEntity) {
	trace_t tr;
	Ray_t ray;
	Vector vStart, vEnd, vEndPos[3];
	vStart = pLocal->GetEyePos();
	vEnd = vPos;
	CTraceFilter filter;

	if (IsVisible(pEntity, pLocal))
		return true;


	filter.pSkip = pLocal;
	ray.Init(vStart, vEnd);

	pEngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_GRATE, &filter, &tr);

	vEndPos[0] = tr.endpos;

	ray.Init(vEnd, vStart);
	filter.pSkip = pEntity;
	pEngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_GRATE, &filter, &tr);

	vEndPos[1] = tr.endpos;

	VectorSubtract(vEndPos[0], vEndPos[1], vEndPos[2]);

	float flLength = VectorLength(vEndPos[2]);

	if (tr.fraction != 0)
	if (flLength < 14)
		return true;

	return false;
}

void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	//side/forward move correction
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}


void doAim(CUserCmd* pCmd, bool aim_active, bool aim_silent, bool aim_autoshoote, bool aim_psilent) {

	Vector oldView = pCmd->viewangles;
	float oldForwardmove = pCmd->forwardmove;
	float oldSidemove = pCmd->sidemove;

	if (aim_active) {
		C_BaseEntity* pLocal = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
		C_BaseCombatWeapon* pWeapon = pLocal->GetActive();
		int BestID = -1;
		float maxDistance = 99999;
		for (int i = 1; i < pEntList->GetHighestEntityIndex(); i++) {
			C_BaseEntity* pEntity = pEntList->GetClientEntity(i);

			if (pEntity && pEntity->Health() > 0 && pLocal->Health() > 0 && !pEntity->IsDormant() && pEntity->Team() != pLocal->Team() && !pEntity->IsImmune()) {
				Vector BoneFrom, BoneTo;

				GetBonePosition(pEntity, BoneFrom, aim_bone.getValue());
				//BoneFrom = Hitscans(pEntity, pLocal, BoneFrom);
				CalcAngle(pLocal->GetEyePos(), BoneFrom, BoneTo);
				SetSmoothAngles(pCmd, BoneTo);
				ClampAngles(BoneTo);
				NormalizeVector(BoneTo);
			//	CorrectMovement(oldView, pCmd, oldForwardmove, oldSidemove);
				if (IsVisible(pEntity, pLocal) && !aim_autowall.getValve()) {
					if (!pWeapon->IsKnife()) {
						if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < aim_fov.getValue()) {
							if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < maxDistance) {
								maxDistance = GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom);
								BestID = i;
							}
						}
					}
				}
				else if (AutoWall(BoneFrom, pLocal, pEntity) && aim_autowall.getValve()) {
					if (!pWeapon->IsKnife()) {
						if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < aim_fov.getValue()) {
							if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < maxDistance) {
								maxDistance = GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom);
								BestID = i;
							}
						}
					}
				}
			}
		}

		C_BaseEntity* pEntity = pEntList->GetClientEntity(BestID);

		if (pEntity && pEntity->Health() > 0 && pLocal->Health() > 0 && !pEntity->IsDormant() && pEntity->Team() != pLocal->Team() && !pEntity->IsImmune()) {
			Vector BoneFrom, BoneTo;

			GetBonePosition(pEntity, BoneFrom, aim_bone.getValue());
			//BoneFrom = Hitscans(pEntity, pLocal, BoneFrom);
			CalcAngle(pLocal->GetEyePos(), BoneFrom, BoneTo);
			SetSmoothAngles(pCmd, BoneTo);
			//	NormalizeVector(BoneTo);
			ClampAngles(BoneTo);
			NormalizeVector(BoneTo);
			aimbotAngle = BoneTo;
			if (IsVisible(pEntity, pLocal) && !aim_autowall.getValve()) {
				if (!pWeapon->IsKnife()) {
					if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < aim_fov.getValue()) {


						if (aim_autoshoote) { // To work with.
							pCmd->buttons |= IN_ATTACK;
						}
						if (pCmd->buttons & IN_ATTACK) {
							pCmd->viewangles = BoneTo;
						}
				//		CorrectMovement(oldView, pCmd, oldForwardmove, oldSidemove);
						if (!aim_silent) {
							pEngine->SetViewAngles(BoneTo);
						}
						
					}
				}
			}
			else if (AutoWall(BoneFrom, pLocal, pEntity) && aim_autowall.getValve()) {
				if (!pWeapon->IsKnife()) {
					if (GetFOV(pCmd->viewangles, pLocal->GetEyePos(), BoneFrom) < aim_fov.getValue()) {


						if (aim_autoshoote) { // To work with.
							pCmd->buttons |= IN_ATTACK;
						}
						if (pCmd->buttons & IN_ATTACK) {
						pCmd->viewangles = BoneTo;
						}
						if (!aim_silent) {
							pEngine->SetViewAngles(BoneTo);
						}
						
					}
				}
			}
		}
	}
}
