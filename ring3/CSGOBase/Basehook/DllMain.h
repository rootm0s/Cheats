#pragma once
#pragma warning(disable: 4244) //float -> int
#pragma warning(disable: 4409) //Unzulässige Anweisungsgröße
#pragma warning(disable: 4800) //Variable wird auf booleschen Wert gesetzt
#pragma warning(disable: 4018) //Konflikt zwischen signed und unsigned
#pragma warning(disable: 4305) //Verkürzung double in float
#pragma warning(disable: 4127) //Addresse einer lokalen Variablen oder eines temporären Werts wird zurückgegeben

#define _CRT_SECURE_NO_WARNINGS

#define HACK_CSGO 1

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>

#include <process.h>
#include <vector>
#include <map>
#include <ostream>
#include <Shlobj.h>
#include <math.h>
#include <timeapi.h>

#include "Vector.h"
#include "checksum_crc.h"
#include "SDK.h"
#include "Utils.h"
#include "drawing.h"
#include "netvars.h"
#include "CNoSpread.h"
#include "CTablefont.h"
#include "DllMain.h"
#include "CAimbot.h"
#include "Console.h"
#include "cvar.h"
#include "CSpread.h"
#include "gui.h"
#include "CTimer.h"
#include "cRadar.h"
#include "cGameEvent.h"

inline int flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;
	int m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0)
		return 1.0;

	return m_fDistance;

}

extern ValveSDK::HLCLient* g_pClient;
extern ValveSDK::CEngineClient* g_pEngine;
extern ValveSDK::ISurface* g_pSurface;
extern ValveSDK::IPanel* g_pPanel;
extern ValveSDK::CEntityList* g_pEntList;
extern ValveSDK::CInput* g_pInput;
extern ValveSDK::CPrediction* g_pPred;
extern ValveSDK::CGameMovement* g_pGameMovement;
extern ValveSDK::CGlobalVars* g_pGlobalVars;
extern ValveSDK::CModelInfo* g_pModel;
extern ValveSDK::CDebugOverlay* g_pDebugOverlay;
extern ValveSDK::IGameEventManager* g_pGameEventManager;
extern ValveSDK::CTrace* g_pEngineTrace;
extern ValveSDK::CGlowObjectManager* g_pGlowObjectManager;

extern CDraw g_Draw,g_ConsoleDraw;
extern CAimbot g_Aimbot;
extern cTableFont g_TableFont;
extern CNoSpread g_NoSpread;

