#pragma once
#include "DLLMain.h"
#pragma region NetVar Manager

namespace NetVars
{
	extern void				Init(void);

	namespace Player
	{
		extern DWORD	m_iHealth;
		extern DWORD	m_ArmorValue;
		extern DWORD	m_iTeamNum;
		extern DWORD	m_fFlags;
		extern DWORD	m_lifeState;
		extern DWORD	m_Local;
		extern DWORD	m_aimPunchAngle;
		extern DWORD	m_viewPunchAngle;
		extern DWORD	m_vecOrigin;
		extern DWORD	m_flFlashDuration;
		extern DWORD	m_flFlashMaxAlpha;
		extern DWORD	m_vecViewOffset;
		extern DWORD	m_bGunGameImmunity;
		extern DWORD	m_nTickBase;
		extern DWORD	m_hActiveWeapon;
		extern DWORD	m_iShotsFired;
		extern DWORD	m_vecVelocity;
		extern DWORD	m_angEyeAngles;
		extern DWORD	m_vecEyeAngles;
		extern DWORD	m_bHasHelmet;
		extern DWORD	m_nHitboxSet;
		extern DWORD	m_clrRender;
		extern DWORD	m_vecAngles;
	}

	namespace Weapon
	{
		extern DWORD	m_iClip1;
		extern DWORD	m_flNextPrimaryAttack;
		extern DWORD	m_iState;
		
	}
}

#pragma endregion