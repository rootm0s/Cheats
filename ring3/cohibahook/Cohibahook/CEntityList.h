#pragma once
#include "main.h"

namespace ValveSDK
{
	class CEntityList
	{
	public:
		CBaseEntity* GetClientEntity(int entnum)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 3)(this, entnum);
		}
		CBaseEntity* GetClientEntityFromHandle(ULONG hEnt)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, ULONG);
			return getvfunc<OriginalFn>(this, 4)(this, hEnt);
		}
		int GetHighestEntityIndex(void)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 6)(this);
		}
		template< class T > T* GetActiveWeapon(CBaseEntity* pEntity)
		{
			int hEntity = *(int*)((DWORD)pEntity + 0x12C0);
			return (T*)g_pBaseEntityList->GetClientEntityFromHandle(hEntity);
		}
		template< class CEntityList > CEntityList* GetClientEntityFromHandle(ULONG hEnt)
		{
			return (CEntityList*)GetBaseEntityPtrFromHandle(hEnt);
		}
	};
}