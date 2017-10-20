#include "main.h"

DWORD WINAPI MenuThread(LPVOID threadParam)
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT))
		{
			bMenuOpen = !bMenuOpen;
			Sleep(300);
		}


		Sleep(1);
	}

	return TRUE;
}

DWORD WINAPI CheatThread(LPVOID threadParam)
{
	hwCSGO = FindWindow(NULL, "Counter-Strike: Global Offensive");
	Interfaces->CaptureInterfaces();
	pConfig->Load();

	oPaintTraverse = (PaintTraverseFn)VMTManager.Hook(Interfaces->VPanel, 41, PaintTraverse);
	oCreateMove = (CreateMoveFn)VMTManager.Hook(Interfaces->ClientMode, 24, CreateMove);
	oOverrideView = (OverrideViewFn)VMTManager.Hook(Interfaces->ClientMode, 18, OverrideView);
	//AntiLeak->ErasePE();

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason != DLL_PROCESS_ATTACH)
		return TRUE;

	Msg("%s\n", "Creating thread...");
	CreateThread(NULL, NULL, CheatThread, NULL, NULL, NULL);
	CreateThread(NULL, NULL, MenuThread, NULL, NULL, NULL);
	return TRUE;
}