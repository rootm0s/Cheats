#pragma once
#include "ProcMem.h"
#include <Windows.h>

#define M_RADPI 57.295779513082f
#define M_PI 3.14159265358979323846f

const extern DWORD m_dwLocalPlayer;
const extern DWORD m_lifeState;
const extern DWORD m_dwEntityList;
const extern DWORD m_entityLoop;
const extern DWORD m_iShotsFired;
const extern DWORD m_iTeamNum;
const extern DWORD m_bDormant;
const extern DWORD m_iHealth;
const extern DWORD m_dwBoneMatrix;
const extern DWORD m_vecPunch;
const extern DWORD m_dwViewAngles;
const extern DWORD m_dwViewMatrix;
const extern DWORD m_dwClientState;
const extern DWORD m_vecViewOffset;
const extern DWORD m_iGlowIndex;
const extern DWORD m_dwGlowObject;
const extern DWORD m_fFlags;
const extern DWORD m_dwForceJump;
const extern DWORD m_bSpotted;
const extern DWORD m_dwForceAttack;
const extern DWORD m_crosshairID;
const extern DWORD m_flFlashMaxAlpha;
const extern DWORD m_vecVelocity;
const extern DWORD m_vecOrigin;
const extern DWORD m_iItemDefinitionIndex;
const extern DWORD m_hActiveWeapon;
const extern DWORD m_iItemDefinitionIndex;
const extern DWORD m_hActiveWeapon;
const extern DWORD m_iItemIDHigh;
const extern DWORD m_iItemIDLow;
const extern DWORD m_OriginalOwnerXuidLow;
const extern DWORD m_OriginalOwnerXuidHigh;
const extern DWORD m_nFallbackPaintKit;
const extern DWORD m_nFallbackSeed;
const extern DWORD m_flFallbackWear;
const extern DWORD m_nFallbackStatTrak;
const extern DWORD forward;
const extern DWORD backward;
const extern DWORD moveleft;
const extern DWORD moveright;
const extern DWORD chokedcommandnr;
const extern  DWORD commandnr;
const extern DWORD m_dwInput;

extern bool AudioESPOption;

extern bool realAimOption;

extern int RIFLEFOV;
extern int RIFLESMOOTH;

extern int PISTOLFOV;
extern int PISTOLSMOOTH;

extern int SNIPERFOV;
extern int SNIPERSMOOTH;

extern int activeAimbotFOV;

const extern int ScreenWidth;
const extern int ScreenHeight;


extern ProcMem b;


// Thanks de_stroyer for this class idea!