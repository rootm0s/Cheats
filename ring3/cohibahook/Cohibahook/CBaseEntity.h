#pragma once
#include "main.h"

class CBaseEntity {
public:
	virtual void init(){};

	Vector GetAbsOrigin()
	{
		//0xAC interpolated getAbsOrigin
		//0xA0 normal abs
		return *(Vector*)((DWORD) this + 0xA0);
	}

	Vector GetVecOrigin()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_vecOrigin");
		return *(Vector*)((DWORD)this + 0x134);
	}

	Vector GetVecViewOffset()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		return *(Vector*)((DWORD)this + iOffset);
	}

	Vector GetEyePosition()
	{
		return GetVecOrigin() + GetVecViewOffset();
	}

	Vector GetEyeAngles()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		return *(Vector*)((DWORD)this + iOffset);
	}

	Vector GetAbsAngles()
	{
		__asm
		{
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}

	bool IsDormant()
	{
		__asm
		{
			MOV EDI, this
			LEA ECX, [EDI + 0x8]
			MOV EDX, DWORD PTR DS : [ecx]
			CALL[EDX + 0x24]
		}
	}

	int GetIndex()
	{
		int iEntIndex;

		PVOID pEnt = (PVOID)this;

		__asm
		{
			MOV ECX, this
				MOV EAX, DWORD PTR DS : [ECX + 0x8]
				MOV EDX, DWORD PTR DS : [EAX + 0x28]
				LEA ESI, DWORD PTR DS : [ECX + 0x8]
				MOV ECX, ESI
				CALL EDX
				MOV iEntIndex, EAX
		}

		return iEntIndex;
	}

	int GetTickBase(void)
	{
		return *(int*)((DWORD)this + 0x17CC);
	}

	bool IsPlayer()
	{
		typedef bool(__thiscall *IsPlayer_t)(PVOID);
		return ((IsPlayer_t)(*(PDWORD)(*(PDWORD)(this) + 0x260)))(this);
	}

	int GetHealth()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_iHealth");
		return *(int*)((DWORD)this + iOffset);
	}

	char GetLifeState()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_LifeState");
		return *(char*)((DWORD)this + 0x25B);
	}
	
	int GetFlags()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_fFlags");
		return *(int*)((DWORD)this + iOffset);
	}

	int GetTeam()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_iTeamNum");
		return *(int*)((DWORD)this + iOffset);
	}

	bool isValidPlayer()
	{		
		return (GetLifeState() == LIFE_ALIVE && !IsDormant() && GetHealth() > 0);
	}

	bool isValidPlayer1()
	{

		//if (pBaseEntity  && !pBaseEntity->IsDormant() && pBaseEntity->GetLifeState() == LIFE_ALIVE && pBaseEntity->GetHealth() > 0)
		//{
		//	return true;
		//}
		//return false;
		return (GetLifeState() == LIFE_ALIVE && !IsDormant() );
	}

	std::string GetClientClassName()
	{
		uintptr_t pNext = *(uintptr_t*)((DWORD)this + 0x8);
		pNext = *(uintptr_t*)((DWORD)pNext + 0x8);
		pNext = *(uintptr_t*)((DWORD)pNext + 0x1);
		return std::string(*(char**)((DWORD)pNext + 0x8));
	}

	Vector GetVecVelocity()
	{
		return *(Vector*)((DWORD)this + 0x110);
	}

	Vector GetPunchAngle()
	{
		return *(Vector*)((DWORD)this + 0x1378 + 0x70);
	}

	int SetFov(int fovvalue)
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_iDefaultFOV");
		*(int*)((DWORD)this + iOffset) = fovvalue;
		static int iOffset2 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_bIsScoped");
		bool scopecheck = *(bool*)((DWORD)this + iOffset2);
		if (!scopecheck)
		{
			static int iOffset3 = g_NetworkedVariableManager.GetOffset("DT_BasePlayer", "m_iFOV");
			*(int*)((DWORD)this + iOffset3) = fovvalue;
		}
		return true;
	}

	bool SetNoVisRecoil()
	{


		return true;
	}

	bool GetScope()
	{
		static int iOffset2 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_bIsScoped");
		bool scopecheck = *(bool*)((DWORD)this + iOffset2);
		
		if (scopecheck)
		{
			return true;
		}

		return false;
	}

	int GetiShots()
	{
		static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_iShotsFired");
		return *(int*)((DWORD)this + iOffset);
	}

	ValveSDK::Color GetHealthColor()
	{
		int nHealth = GetHealth();
		float r = 255.f - float(nHealth) * 2.55f;
		float g = float(nHealth) * 2.55f;
		return ValveSDK::Color(int(r), int(g), 0, 255);
	}

	ValveSDK::CBaseCombatWeapon* GetActiveBaseCombatWeapon();
};