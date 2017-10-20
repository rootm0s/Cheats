#pragma once

class CEngine
{
public:
	Vector GetEyePosition(IClientEntity* pEntity);
	bool GetBonePosition(IClientEntity* pPlayer, Vector& Hitbox, int Bone);
	bool IsVisible(IClientEntity* pLocalBaseEntity, IClientEntity* pBaseEntity, Vector vStart, Vector vEnd);
	bool IsAlive(IClientEntity* pEntity);
	int GetHealth(IClientEntity* pEntity);
};
extern CEngine cEng;