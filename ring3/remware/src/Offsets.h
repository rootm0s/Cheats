#ifndef OFFSETS_H
#define OFFSETS_H
#include "PMemory.h"
#include <string>
#include <sstream>
#include "Decrypt.h"

class COffsets {
public:
	DWORD dwLocalPlayer = 0xAADFFC;   // local player
	DWORD dwEntityList = 0x4A8A684;   // entity list
	DWORD dwAttack = 0x2ECCA08;       // force attack
	DWORD dwEnginePointer = 0x5A3284; // client state
	DWORD dwSetViewAngle = 0x4D10;    // client state(set view angle)
	DWORD dwGlow = 0x4FA7690;         // glow object manager
	DWORD dwJump = 0x4F21618;         // force jump
	
	DWORD dwActiveWeapon = 0x2EE8; //m_hActiveWeapon
	DWORD dwWeaponId = 0x32EC; //m_iWeaponID (m_fAccuracyPenalty + ???)
	DWORD dwInCross = 0xAA70; //m_iCrossHairID
	DWORD dwSpotted = 0x939; //m_bSpotted
	DWORD dwId = 0x64; //m_dwIndex
	DWORD dwHealth = 0xFC; //m_iHealth
	DWORD dwTeam = 0xF0; //m_iTeamNum
	DWORD dwAmmo = 0x3204; //m_iClip1
	DWORD dwVecPunch = 0x301C; //m_vecPunch (m_aimPunchAngle + m_local)
	DWORD dwIShotsFired = 0xA2C0; //m_iShotsFired
	DWORD dwDormant = 0xE9; //bDormant
	DWORD dwFlashMaxAlpha = 0xA304; //m_flFlashMaxAlpha
	DWORD dwFlashDuration = 0xA308; //m_flFlashDuration
	DWORD dwFlags = 0x100; //m_fFlags

	std::string toHex(DWORD offset);
	void getOffsets(PMemory* m);
private:
	PMemory* mem;
	PModule modClient, modEngine;
};

#endif