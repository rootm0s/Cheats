#include "stdafx.h"

void tLocalPlayerEntity::tLocalPlayerEntityFunctions::SetViewAngle(DWORD BaseAddress, DWORD Offset, D3DXVECTOR3 ViewAngle)
{
    pIO.Functions.Write<D3DXVECTOR3>(&ViewAngle, BaseAddress + Offset);
}

bool tLocalPlayerEntity::tLocalPlayerEntityFunctions::IsJumping(byte Flags)
{
	if (Flags == 1 || Flags == 3 || Flags == 5 || Flags == 7)
	{
		return false;
	}
	return true;
}

void tLocalPlayerEntity::tLocalPlayerEntityFunctions::GetLocalPlayerEntityInfo(tLocalPlayerEntityInfo* LocalPlayerEntityInfo)
{
	if (LocalPlayerEntityInfo)
	{
	LocalPlayerEntityInfo->Valid = false;
		if (pIO.ClientModuleBaseAddress)
		{
			pIO.Functions.Read<DWORD>(&LocalPlayerEntityInfo->BaseAddress, pIO.ClientModuleBaseAddress + pGlobalVars.Offsets.m_dwLocalPlayer);
			if (LocalPlayerEntityInfo->BaseAddress && pIO.EngineModuleBaseAddress)
			{
				ZeroMemory(&LocalPlayerEntityInfo->DataChunk, sizeof(LocalPlayerEntityInfo->DataChunk));
				pIO.Functions.Read<tDataStructs::tDataChunk>(&LocalPlayerEntityInfo->DataChunk, LocalPlayerEntityInfo->BaseAddress);
				pIO.Functions.Read<DWORD>(&LocalPlayerEntityInfo->ClientStateBaseAddress, pIO.EngineModuleBaseAddress + pGlobalVars.Offsets.m_dwClientState);
				if (LocalPlayerEntityInfo->ClientStateBaseAddress)
				{
					pIO.Functions.Read<int>(&LocalPlayerEntityInfo->Index, LocalPlayerEntityInfo->ClientStateBaseAddress + pGlobalVars.Offsets.m_dwLocalPlayerIndex);
					memmove_s(&LocalPlayerEntityInfo->Flags, sizeof(LocalPlayerEntityInfo->Flags), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_fFlags], sizeof(LocalPlayerEntityInfo->Flags));
					memmove_s(&LocalPlayerEntityInfo->CrosshairEntityIndex, sizeof(LocalPlayerEntityInfo->CrosshairEntityIndex), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_iCrossHairID], sizeof(LocalPlayerEntityInfo->CrosshairEntityIndex));
					memmove_s(&LocalPlayerEntityInfo->LifeState, sizeof(LocalPlayerEntityInfo->LifeState), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_lifeState], sizeof(LocalPlayerEntityInfo->LifeState));
					memmove_s(&LocalPlayerEntityInfo->Origin, sizeof(LocalPlayerEntityInfo->Origin), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_vecOrigin], sizeof(LocalPlayerEntityInfo->Origin));
					memmove_s(&LocalPlayerEntityInfo->PunchAngle, sizeof(LocalPlayerEntityInfo->PunchAngle), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_vecPunch], sizeof(LocalPlayerEntityInfo->PunchAngle));
					memmove_s(&LocalPlayerEntityInfo->ShotsFired, sizeof(LocalPlayerEntityInfo->ShotsFired), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_iShotsFired], sizeof(LocalPlayerEntityInfo->ShotsFired));
					memmove_s(&LocalPlayerEntityInfo->TeamNumber, sizeof(LocalPlayerEntityInfo->TeamNumber), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_iTeamNum], sizeof(LocalPlayerEntityInfo->TeamNumber));
					memmove_s(&LocalPlayerEntityInfo->Velocity, sizeof(LocalPlayerEntityInfo->Velocity), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_vecVelocity], sizeof(LocalPlayerEntityInfo->Velocity));
					pIO.Functions.Read<D3DXVECTOR3>(&LocalPlayerEntityInfo->ViewAngle, LocalPlayerEntityInfo->ClientStateBaseAddress + pGlobalVars.Offsets.m_dwViewAngle);
					memmove_s(&LocalPlayerEntityInfo->ViewOffset, sizeof(LocalPlayerEntityInfo->ViewOffset), &LocalPlayerEntityInfo->DataChunk.Data[pGlobalVars.Offsets.m_vecViewOffset], sizeof(LocalPlayerEntityInfo->ViewOffset));
					pIO.Functions.Read<tDataStructs::tViewMatrix>(&LocalPlayerEntityInfo->ViewMatrix, pIO.ClientModuleBaseAddress + pGlobalVars.Offsets.m_dwViewMatrix);
					LocalPlayerEntityInfo->Valid = true;
				}

			}

		}
	}
}