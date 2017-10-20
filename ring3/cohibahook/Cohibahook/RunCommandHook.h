#pragma once
extern Base::Utils::CVMTHookManager g_pRunCommand;
void __stdcall RunCommandHook(CBaseEntity* pEntity, ValveSDK::CInput::CUserCmd* pCmd, void* moveHelper);

class CRunCommand
{
public:
	void RunCommand(ValveSDK::CInput::CUserCmd *pUserCmd);
private:
};

extern CRunCommand g_RunCommand;