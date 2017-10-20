#include "Offsets.h"


const DWORD m_dwLocalPlayer = 0xAABFFC; // Changes
const DWORD m_dwEntityList = 0x4A88564; // Changes
const DWORD m_dwClientState = 0x005C32C4; // Changes
const DWORD m_dwGlowObject = 0x04FD91C4; // Changes
const DWORD m_dwForceJump = 0x04F5782C; // Changes
const DWORD m_dwForceAttack = 0x02F00DF4; // Changes
const DWORD m_dwInput = 0x04F0B7F0; // Changes
const DWORD m_dwViewMatrix = 0x04AB2844; //Changes
const DWORD m_vecPunch = 0x0000301C;// Changes (sometimes)

const DWORD m_lifeState = 0x0000025B;
const DWORD m_entityLoop = 0x10;
const DWORD m_iTeamNum = 0xF0;
const DWORD m_iShotsFired = 0x0000A2B0;
const DWORD m_bDormant = 0x000000E9;
const DWORD m_iHealth = 0xFC;
const DWORD m_dwBoneMatrix = 0x00002698;
const DWORD m_dwViewAngles = 0x00004D0C;
const DWORD m_vecViewOffset = 0x00000104;
const DWORD m_vecOrigin = 0x00000134;
const DWORD m_iGlowIndex = 0x0000A310;
const DWORD m_fFlags = 0x100;
const DWORD m_bSpotted = 0x00000939;
const DWORD m_iCrossHairID = 0xAA44;
const DWORD m_flFlashMaxAlpha = 0x0000A2F4;
const DWORD m_vecVelocity = 0x00000110;
const DWORD m_iItemDefinitionIndex = 0x2F88;
const DWORD m_hActiveWeapon = 0x2EE8;
const DWORD m_iItemIDHigh = 0x2FA0;
const DWORD m_iItemIDLow = 0x2FA4;
const DWORD m_OriginalOwnerXuidLow = 0x3160;
const DWORD m_OriginalOwnerXuidHigh = 0x3164;
const DWORD m_nFallbackPaintKit = 0x3168;
const DWORD m_nFallbackSeed = 0x316C;
const DWORD m_flFallbackWear = 0x3170;
const DWORD m_nFallbackStatTrak = 0x3174;

const DWORD forward = 0x02E97F18; // HARD CHANGES
const DWORD backward = 0x02E97EF4; // HARD CHANGES
const DWORD moveleft = 0x02E97F00; // HARD CHANGES
const DWORD moveright = 0x02E97EA0; // HARD CHANGES
const DWORD chokedcommandnr = 0x4CAC; // HARD CHANGES
const DWORD commandnr = 0x4CA8; // HARD CHANGES

bool AudioESPOption = true;

bool realAimOption = true;

int activeAimbotFOV = 20; // Change to liking, 20 for testing purposes

const int ScreenWidth = 1280;
const int ScreenHeight = 1024;

ProcMem b;