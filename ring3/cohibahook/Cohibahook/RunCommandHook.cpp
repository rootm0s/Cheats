#include "main.h"

Base::Utils::CVMTHookManager g_pRunCommand;
CRunCommand g_RunCommand;

void CRunCommand::RunCommand(ValveSDK::CInput::CUserCmd *pUserCmd)
{
	if (g_Valve.pEngine->IsInGame() && g_Valve.pEngine->IsConnected())
	{
		CBaseEntity *pEntity = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());

		if (!pEntity)
			return;

		if (!pEntity->isValidPlayer())
			return;

		if (pEntity->GetLifeState() != LIFE_ALIVE)
			return;

		if (!g_Valve.g_pMoveHelper)
			return;

		float flOldCurtime = g_Valve.pGlobalVars->curtime;
		float flOldFrametime = g_Valve.pGlobalVars->frametime;

		g_Valve.pGlobalVars->curtime = pEntity->GetTickBase() * g_Valve.pGlobalVars->interval_per_tick;
		g_Valve.pGlobalVars->frametime = g_Valve.pGlobalVars->interval_per_tick;

		DWORD dwOldUserCmd = *(DWORD *)((DWORD)pEntity + 0x16E8);
		*(DWORD *)((DWORD)pEntity + 0x16E8) = (DWORD)pUserCmd;

		BYTE bMoveData[0x200];
		g_Valve.g_pMoveHelper->SetHost(pEntity);
		g_Valve.g_pPred->SetupMove(pEntity, pUserCmd, NULL, &bMoveData);
		g_Valve.g_pGameMovement->ProcessMovement(pEntity, &bMoveData);
		g_Valve.g_pPred->FinishMove(pEntity, pUserCmd, &bMoveData);
		*(DWORD *)((DWORD)pEntity + 0x16E8) = dwOldUserCmd;

		g_Valve.g_pMoveHelper->SetHost(NULL);

		g_Valve.pGlobalVars->curtime = flOldCurtime;
		g_Valve.pGlobalVars->frametime = flOldFrametime;
	}
}

void __stdcall RunCommandHook(CBaseEntity* pEntity, ValveSDK::CInput::CUserCmd* pCmd, void* moveHelper)
{
	static DWORD dwRunCommand = g_pRunCommand.dwGetMethodAddress(19);

	__asm
	{
		push moveHelper;
		push pCmd;
		push pEntity;
		call dwRunCommand;
	}

	g_Valve.g_pMoveHelper = (ValveSDK::IMoveHelper*)moveHelper;
}