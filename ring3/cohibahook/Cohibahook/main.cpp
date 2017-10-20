#include "main.h"

CValve g_Valve;

void unhook()
{
	g_pPanelVMT.UnHook();
	g_pClientVMT.UnHook();
	g_pInKeyEventVMT.UnHook();
	g_pFrameStageNotify.UnHook();
	g_pRunCommand.UnHook();
}

VOID RemoveHooks(LPARAM lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;

	while (true)
	{
		Sleep(100);
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			Sleep(1000);
			unhook();
			Sleep(2000);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, 0);
		}
	}
}

VOID init(PVOID ptr)
{
	g_Valve.initSDK();

	g_NetworkedVariableManager.Init();

	g_pPanelVMT.bInitialize((PDWORD*)g_Valve.pPanel);
	g_pPanelVMT.dwHookMethod((DWORD)hkdPaintTraverse, 41);	

	g_pClientVMT.bInitialize((PDWORD*)g_Valve.pClient);	
	g_pClientVMT.dwHookMethod((DWORD)hkdCreateMove, 21);
	g_pClientVMT.dwHookMethod((DWORD)hkdHudUpdate, 11);


	g_pInKeyEventVMT.bInitialize((PDWORD*)g_Valve.pClient);
	g_pInKeyEventVMT.dwHookMethod((DWORD)hkdInKeyEvent, 20);

	g_pFrameStageNotify.bInitialize((PDWORD*)g_Valve.pClient);
	g_pFrameStageNotify.dwHookMethod((DWORD)new_FrameStageNotify, 36);

	g_pRunCommand.bInitialize((PDWORD*)g_Valve.g_pPred);
	g_pRunCommand.dwHookMethod((DWORD)RunCommandHook, 19);

	//g_Valve.pInput = *(ValveSDK::CInput**)(g_pClientVMT.dwGetMethodAddress(21) + INPUTOFFSET);
	PDWORD pdwClientVMT = *(PDWORD*)g_Valve.pClient;
	g_Valve.pInput = *(ValveSDK::CInput**)(pdwClientVMT[15] + 0x1);
}

INT WINAPI DllMain(_In_ HANDLE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		//Base::Debug::AttachDebugConsole();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, _HDllHandle, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RemoveHooks, _HDllHandle, NULL, NULL);
	}

	return TRUE;
}