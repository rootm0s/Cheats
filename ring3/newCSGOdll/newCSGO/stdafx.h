#pragma once

#pragma warning(disable: 4530)

#define WIN32_LEAN_AND_MEAN
#define NOCOMM

#include <Windows.h>

// libc
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include <string>
#include <hash_map>

#pragma comment	( lib, "d3d9.lib" )
#pragma comment	( lib, "d3dx9.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "Shlwapi.lib" )
#pragma comment ( lib, "winmm.lib" )


//Included Headers
#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	<Tlhelp32.h>
#include	<Shlwapi.h>
#include	<Shlobj.h>
#include	<time.h>
#include	<vector>
#include	<ddraw.h>
#include	<d3dx9.h>
#include	<d3d9.h>
#include	<D3dx9tex.h>
#include	<D3dx9shape.h>

using namespace std;
#include "Math.h"
#include "checksum_crc.h"
#include "CSGOStructs.h"
#include "CVMTHooking.h"
#include "CEngine.h"
#include "CDrawing.h"
#include "CHackEsp.h"

#include "HackMain.h"
#include "cfn.h"

extern DWORD WINAPI HackThread( PVOID );
extern DWORD HookAndStart(void);
