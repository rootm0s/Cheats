#include "DLLMain.h"
void LOG4(const char* output, ...)
{
	printf(output);
	//printf("\n");
}
#pragma region NetVar Manager
int GetOffset(char *szClassName, char *szVariable)
{
	ValveSDK::ClientClass *pClass = g_pClient->GetAllClasses();

	for (; pClass; pClass = pClass->NextClass())
	{
		RecvTable *pTable = pClass->GetTable();

	if (pTable->GetNumProps() <= 1) continue;

		for (int i = 0; i < pTable->GetNumProps(); i++)
		{
			RecvProp *pProp = pTable->GetProp(i);

			if (!pProp) continue;

			if (!strcmp(pTable->GetName(), szClassName) && !strcmp(pProp->GetName(), szVariable))
			{
				LOG4("%s: 0x%.4X (%s) \n", pProp->GetName(), pProp->GetOffset(), pTable->GetName());

				return pProp->GetOffset();
			}
			if (pProp->GetDataTable())
			{
				RecvTable *pTable = pProp->GetDataTable();
				for (int i = 0; i < pTable->GetNumProps(); i++)
				{
					RecvProp *pProp = pTable->GetProp(i);

					if (!pProp) continue;

					if (!strcmp(pTable->GetName(), szClassName) && !strcmp(pProp->GetName(), szVariable))
					{
						LOG4("%s: 0x%.4X (%s) \n", pProp->GetName(), pProp->GetOffset(), pTable->GetName());


						return pProp->GetOffset();
					}
				}
			}
		}
	}
	return 0;
}

namespace NetVars
{

	void Init(void)
	{
		Player::m_iHealth = GetOffset("DT_BasePlayer", "m_iHealth");
		Player::m_ArmorValue = GetOffset("DT_CSPlayer", "m_ArmorValue");
		Player::m_iTeamNum = GetOffset("DT_BaseEntity", "m_iTeamNum");
		Player::m_iShotsFired = GetOffset("DT_CSLocalPlayerExclusive", "m_iShotsFired");
		Player::m_fFlags = GetOffset("DT_BasePlayer", "m_fFlags");
		Player::m_lifeState = GetOffset("DT_BasePlayer", "m_lifeState");
		Player::m_Local = GetOffset("DT_LocalPlayerExclusive", "m_Local");
		Player::m_vecEyeAngles = GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Player::m_bGunGameImmunity = GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		Player::m_aimPunchAngle = 0x70;
		Player::m_vecOrigin = GetOffset("DT_CSLocalPlayerExclusive", "m_vecOrigin");
		Player::m_vecViewOffset = GetOffset("DT_LocalPlayerExclusive", "m_vecViewOffset[0]");
		Player::m_nTickBase = GetOffset("DT_LocalPlayerExclusive", "m_nTickBase");
		Player::m_hActiveWeapon = GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		Player::m_flFlashDuration = GetOffset("DT_CSPlayer", "m_flFlashDuration");
		Player::m_flFlashMaxAlpha = GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		Player::m_vecVelocity = GetOffset("DT_LocalPlayerExclusive", "m_vecVelocity[0]");
		Player::m_angEyeAngles = GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Player::m_bHasHelmet = GetOffset("DT_CSPlayer", "m_bHasHelmet");
		Player::m_nHitboxSet = GetOffset("DT_BaseAnimating", "m_nHitboxSet");
		Weapon::m_flNextPrimaryAttack = GetOffset("DT_LocalActiveWeaponData", "m_flNextPrimaryAttack");
		Weapon::m_iClip1 = Weapon::m_iState = GetOffset("DT_BaseCombatWeapon", "m_iClip1");//0x15C0;
		Weapon::m_iState = GetOffset("DT_BaseCombatWeapon", "m_iState");
		
	}

	namespace Player
	{
		 DWORD	m_iHealth = NULL;
		 DWORD	m_ArmorValue = NULL;
		 DWORD	m_iTeamNum = NULL;
		 DWORD	m_fFlags = NULL;
		 DWORD	m_lifeState = NULL;
		 DWORD	m_flFlashDuration = NULL;
		 DWORD	m_flFlashMaxAlpha = NULL;
		 DWORD	m_Local = NULL;
		 DWORD	m_bGunGameImmunity = NULL;
		 DWORD	m_vecPunchAngle = NULL;
		 DWORD	m_aimPunchAngle = NULL;
		 DWORD  m_viewPunchAngle = NULL;
		 DWORD	m_vecOrigin = NULL;
		 DWORD	m_vecViewOffset = NULL;
		 DWORD	m_nTickBase = NULL;
		 DWORD	m_hActiveWeapon = NULL;
		 DWORD	m_vecVelocity = NULL;
		 DWORD	m_vecEyeAngles = NULL;
		 DWORD	m_angEyeAngles = NULL;
		 DWORD	m_bHasHelmet = NULL;
		 DWORD	m_nHitboxSet = NULL;
		 DWORD	m_clrRender = NULL;
		 DWORD	m_iShotsFired = NULL;
	}

	namespace Weapon
	{
		DWORD	m_iClip1 = NULL;
		DWORD	m_flNextPrimaryAttack = NULL;
		DWORD	m_iState = NULL;
		
	}

}  
#pragma endregion
