#include "main.h"

Base::Utils::CVMTHookManager g_pClientVMT;

void movePacket(int sequence_number, bool &SendPacket)
{

	ValveSDK::CInput::CUserCmd *pUserCmd = &(*(ValveSDK::CInput::CUserCmd**)((DWORD)g_Valve.pInput + USERCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
	ValveSDK::CInput::CVerifiedUserCmd *pVerifiedCmd = &(*(ValveSDK::CInput::CVerifiedUserCmd**)((DWORD)g_Valve.pInput + VERIFIEDCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];

	CBaseEntity* pMe = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());

	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		pUserCmd->buttons |= IN_JUMP;
	}
	else if (pUserCmd->buttons & IN_JUMP)
	{
		if (pMe->GetFlags() & FL_ONGROUND)
		{
			bLastJumped = true;
			bShouldFake = true;
		}
		else
		{
			pUserCmd->buttons &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
	else
	{
		bLastJumped = false;
		bShouldFake = false;
	}
	/*
	if (pMe->GetLifeState() == LIFE_ALIVE && !pMe->GetActiveBaseCombatWeapon()->IsMiscWeapon())
	{
		Vector vMove(pUserCmd->forwardmove, pUserCmd->sidemove, pUserCmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		Vector vRealView(pUserCmd->viewangles);
		g_Math.VectorAngles3D(vMove, vMove2);
		bool bEdgeAA = true;
		g_Antiaim.EdgeAntiAim1(pMe, pUserCmd->viewangles.y);
		if (bEdgeAA)
		{
			flYaw = DEG2RAD(pUserCmd->viewangles.y - vRealView.y + vMove2.y);
			pUserCmd->forwardmove = cos(flYaw) * flSpeed;
			pUserCmd->sidemove = sin(flYaw) * flSpeed;
		}
	}
	*/


	g_RunCommand.RunCommand(pUserCmd);

	ValveSDK::CBaseCombatWeapon* pWeapon = pMe->GetActiveBaseCombatWeapon();

	if (pMe->GetLifeState() == LIFE_ALIVE && g_pConsole->getValue("aimbot_active") == 1 && !pMe->GetActiveBaseCombatWeapon()->IsMiscWeapon() && !pMe->GetActiveBaseCombatWeapon()->Clip1() == 0 && ((pUserCmd->buttons & IN_ATTACK) || g_pConsole->getValue("aimbot_auto") == 1))
	{
		g_Aimbot.doAim(pUserCmd, SendPacket, pWeapon);
	}

	static int iChokedTicks = -1;
	iChokedTicks++;

	if (iChokedTicks <= 6 && pMe->GetLifeState() == LIFE_ALIVE && pUserCmd->buttons & IN_ATTACK && g_Aimbot.IsAbleToShoot(pMe) && !pMe->GetActiveBaseCombatWeapon()->IsMiscWeapon() && !pMe->GetActiveBaseCombatWeapon()->Clip1() == 0) //pFix 
	{
		SendPacket = false;
	}
	else
	{
		if (pMe->GetLifeState() == LIFE_ALIVE && !pMe->GetActiveBaseCombatWeapon()->IsMiscWeapon() && g_pConsole->getValue("antiaim_fake") != 0)
		{
			g_Antiaim.doAntiaim(pMe, pUserCmd, SendPacket);
		}
		iChokedTicks = 0;
	}

	if (pMe->GetLifeState() == LIFE_ALIVE && pUserCmd->buttons & IN_ATTACK && g_pConsole->getValue("autopistol") == 1 && !pMe->GetActiveBaseCombatWeapon()->IsMiscWeapon() && pMe->GetActiveBaseCombatWeapon()->IsPistol())
	{
		static bool bFlip = false;
		if (bFlip)
			pUserCmd->buttons &= ~IN_ATTACK;
		bFlip = !bFlip;
	}

	g_Aimbot.NormalizeVector(pUserCmd->viewangles);
	g_Aimbot.ClampAngles(pUserCmd->viewangles);
	pVerifiedCmd->m_cmd = *pUserCmd;
	pVerifiedCmd->m_crc = pUserCmd->GetChecksum();
}

void AAARotateProxy(const CRecvProxyData* pData, void* pStruct, void* pOut){
	float p = pData->m_Value.m_Float;

	if (p > 179.0f)
		p -= 360.0f;
	if (p < -179.0f)
		p += 360.0f;

	
	if (p > 90.0f || p < -90.0f)
	{
		p = 90.0f;	
	}

	*(float*)pOut = p;
}
void __stdcall hkdHudUpdate(bool active)
{
	static DWORD dwHudUpdateVMT = g_pClientVMT.dwGetMethodAddress(11); // Client::HudUpate( bool ) @ idx 11
	__asm
	{
		PUSH active
		call dwHudUpdateVMT
	}
	
	//	bool HookProp(const char *tableName, const char *propName, RecvVarProxyFn function);
	g_NetworkedVariableManager.HookProp("DT_CSPlayer", "m_angEyeAngles[0]", AAARotateProxy);
}

DWORD dwOriginCreateMove;
bool bSendThisPacket;
void __declspec(naked) __stdcall hkdCreateMove(int sequence_number, float input_sample_frametime, bool active)
{

	dwOriginCreateMove = g_pClientVMT.dwGetMethodAddress(21);
	__asm
	{
		MOV bSendThisPacket, BL
			PUSH EBP
			MOV EBP, ESP
			SUB ESP, 8
			PUSHAD
			PUSH active
			PUSH input_sample_frametime
			PUSH sequence_number
			CALL dwOriginCreateMove
	}

	movePacket(sequence_number, bSendThisPacket);


	__asm
	{
		POPAD
			MOV BL, bSendThisPacket
			MOV ESP, EBP
			POP EBP
			RETN 0x0C
	}

	
}