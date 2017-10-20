// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

tIO pIO;
tD3D pD3D;
tESP pESP;
tBhop pBhop;
tAimbot pAimbot;
tTriggerbot pTriggerbot;
tGlobalVars pGlobalVars;
tOverlayWindow pOverlayWindow;
tGeneralPlayerEntity pGeneralPlayerEntity;
tLocalPlayerEntity pLocalPlayerEntity;

DWORD WINAPI Thead5(LPVOID PARAMS)
{
	pIO.Thead5Active = true;
	while (!pGlobalVars.KILLALL)
	{
		pBhop.Functions.Tick();
		Sleep(1);
	}
	pIO.Thead5Active = false;
	return 0;
}

DWORD WINAPI Thead4(LPVOID PARAMS)
{
	pIO.Thead4Active = true;
	CreateThread(0, 0, &Thead5, 0, 0, 0);
	while (!pGlobalVars.KILLALL)
	{
		pTriggerbot.Functions.Tick();
		Sleep(1);
	}
	pIO.Thead4Active = false;
	return 0;
}

DWORD WINAPI Thead3(LPVOID PARAMS)
{
	pIO.Thead3Active = true;
	CreateThread(0, 0, &Thead4, 0, 0, 0);
	while (!pGlobalVars.KILLALL)
	{
		pAimbot.Functions.Tick();
		Sleep(1);
	}
	pIO.Thead3Active = false;
	return 0;
}

DWORD WINAPI Thead2(LPVOID PARAMS)
{
	pIO.Thead2Active = true;
	CreateThread(0, 0, &Thead3, 0, 0, 0);
    while (!pGlobalVars.KILLALL)
        {
			pOverlayWindow.Functions.Refresh();
			Sleep(1);
        }
	pD3D.Functions.Destruct();
	pOverlayWindow.Functions.Destruct();
	pIO.Thead2Active = false;
    return 0;
}

DWORD WINAPI Thead1(LPVOID PARAMS)
{
	MessageBoxW(NULL, L"Injection successful.", L"PP-Multi", MB_OK);
	pIO.Thead2Active = false;
	pIO.Thead3Active = false;
	pIO.Thead4Active = false;
	pIO.Thead5Active = false;
    while (!pIO.Functions.Attach())
        {
            Sleep(100);
        }
	CreateThread(0, 0, &Thead2, 0, 0, 0);
    while (!pGlobalVars.KILLALL)
        {
            if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F2) < 0)
                {
                    while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F2) < 0)
                        {
                            Sleep(100);
                        }
                    pGlobalVars.KILLALL = true;
                }
            if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F1) < 0)
                {
                    while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F1) < 0)
                        {
                            Sleep(100);
                        }
                    pIO.Functions.LoadSettings();
                }
			if (!pIO.Functions.IsProcessActive())
			{
				pIO.Functions.Detach();
				while (!pIO.Functions.Attach())
				{
					Sleep(100);
				}
			}
            Sleep(100);
        }
	while (pIO.Thead2Active || pIO.Thead3Active || pIO.Thead4Active || pIO.Thead5Active)
	{
		Sleep(100);
	}
    pIO.Functions.Detach();
	MessageBoxW(NULL, L"Hack terminated successfully.", L"PP-Multi", MB_OK);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
        {
            CreateThread(0, 0, &Thead1, 0, 0, 0);
        }
        }
    return TRUE;
}

