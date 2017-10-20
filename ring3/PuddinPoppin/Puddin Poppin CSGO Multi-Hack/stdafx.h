// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <string>
#include <TlHelp32.h>
#include <random>
#include <sstream>
#include <fstream>
#include <io.h>
#include <iostream>
#include <algorithm>
#include <future>
#include "GlobalVars.h"
#include "OverlayWindow.h"
#include "D3D.h"
#include "GeneralPlayerEntity.h"
#include "LocalPlayerEntity.h"
#include "Bhop.h"
#include "Aimbot.h"
#include "Triggerbot.h"
#include "ESP.h"
#include "IO.h"
// TODO: reference additional headers your program requires here
