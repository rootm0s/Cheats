#include "stdafx.h"
CEngine cEng;

bool CEngine::IsVisible(IClientEntity* pLocalBaseEntity, IClientEntity* pBaseEntity, Vector vStart, Vector vEnd)
{	
	return true;	
}
Vector CEngine::GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast< Vector* >(reinterpret_cast< int >( pEntity ) + 0x104 );

	return pEntity->GetAbsOrigin( ) + vecViewOffset;
}
bool CEngine::GetBonePosition(IClientEntity* pPlayer, Vector& Hitbox, int Bone)
{
	matrix3x4_t MatrixArray[128];

	if(!pPlayer->SetupBones( MatrixArray, 128, 0x00000100, pEngine->Time() ) )
		return FALSE;

	matrix3x4_t HitboxMatrix = MatrixArray[Bone];

	Hitbox = Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

	return TRUE;
}

///\\\\\\\\\\\\\\\\\\ Needefull stuff

int CEngine::GetHealth(IClientEntity* pEntity)
{
	return *reinterpret_cast< int* >(reinterpret_cast< int >( pEntity ) + 0xFC);
}

bool CEngine::IsAlive(IClientEntity* pEntity)
{
	char szLifeState = *(char*)((DWORD)pEntity + 0x25B);
	return (szLifeState == 0);
}