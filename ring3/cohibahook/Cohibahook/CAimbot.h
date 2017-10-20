#pragma once

typedef struct
{
	int iDamage;
	float fMaxRange;
	float fRangeModifier;
	float fPenetrationPower;
} WeaponInfo_t;

class CAimbot
{
public:
	void NormalizeVector(Vector& vec);
	void ClampAngles(Vector& vecAngles);
	void StopMovement(ValveSDK::CInput::CUserCmd* pUserCmd);
	bool CanWallbang(CBaseEntity* pLocal, CBaseEntity* pEntity, Vector vecStart, Vector vecEnd);
	bool SetupBones(CBaseEntity *pAnimating, matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	bool GetHitbox(Vector &vec, int hitbox, int index);
	bool IsAbleToShoot(CBaseEntity* pLocalClientBaseEntity);
	float GetFov(Vector& vecAngle, Vector& vecOrigin, Vector& vecOther, bool bDistancebased);
	Vector HitScans(CBaseEntity *pLocal, CBaseEntity *pTarget);
	VOID doAim(ValveSDK::CInput::CUserCmd* pCmd, bool &SendPacket, ValveSDK::CBaseCombatWeapon *pWeapon);
	inline void VectorTransform(const Vector& in1, const matrix3x4 &in2, Vector &out);
	bool bIsPointPenetrable(WeaponInfo_t wiWeaponInfo, Vector vStart, Vector vEnd);
	ValveSDK::CTrace::CTraceFilterNoPlayer tfNoPlayers;
	WeaponInfo_t wiWeaponInfo;
private:
};

extern CAimbot g_Aimbot;