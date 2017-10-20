#include "dll.h"

struct Netvar
{
	char* name;
	char pad1[33];
	void* table;
	int offset;
	char pad2[9];
};

void NetvarManager::Init()
{
	m_lifeState = GetOffset("DT_BasePlayer","m_lifeState");
	m_iHealth = GetOffset("DT_BasePlayer","m_iHealth");
	m_fFlags = GetOffset("DT_BasePlayer","m_fFlags");
	m_nTickBase = GetOffset("DT_BasePlayer","m_nTickBase");
	m_pCurrentCommand = GetOffset("DT_BasePlayer","m_hConstraintEntity") - 4;
	m_aimPunchAngle = GetOffset("DT_BasePlayer","m_aimPunchAngle");
	m_viewPunchAngle = GetOffset("DT_BasePlayer","m_viewPunchAngle");
	m_vecViewOffset = GetOffset("DT_BasePlayer","m_vecViewOffset[0]");
	m_vecVelocity = GetOffset("DT_BasePlayer","m_vecVelocity[0]");

	m_angEyeAngles = GetOffset("DT_CSPlayer","m_angEyeAngles[0]");
	
	m_clrRender = GetOffset("DT_BaseEntity","m_clrRender");
	m_iTeamNum = GetOffset("DT_BaseEntity","m_iTeamNum");
	m_vecOrigin = GetOffset("DT_BaseEntity","m_vecOrigin");
	m_vecMins = GetOffset("DT_BaseEntity","m_vecMins");
	m_vecMaxs = GetOffset("DT_BaseEntity","m_vecMaxs");

	m_hActiveWeapon = GetOffset("DT_BaseCombatCharacter","m_hActiveWeapon");
	m_flNextAttack = GetOffset("DT_BaseCombatCharacter","m_flNextAttack");

	m_hOwner = GetOffset("DT_BaseCombatWeapon","m_hOwner");
	m_iClip1 = GetOffset("DT_BaseCombatWeapon","m_iClip1");
	m_flNextPrimaryAttack = GetOffset("DT_BaseCombatWeapon","m_flNextPrimaryAttack");
}

int NetvarManager::GetOffset(char* table,char* var)
{
	Class* cl = client->GetClientClasses();

	while (cl)
	{
		if (cl->table)
		{
			char* name = *(char**)((long)cl->table + 12);

			if (cmp(name,table))
				return LookupNetvar(cl->table,var);
		}

		cl = cl->next;
	}

	return 0;
}

int NetvarManager::LookupNetvar(void* table,char* name)
{
	int count = *(int*)((long)table + 4);
	int i;

	Netvar* prop;

	Netvar* table2 = *(Netvar**)table;

	int add = 0;

	for (i=0; i < count; ++i)
	{
		prop = &table2[i];

		if (!prop)
			continue;

		if (prop->name[0] == '0')
			continue;

		if (prop->table)
		{
			int offset = LookupNetvar(prop->table,name);

			if (offset)
				return prop->offset + offset;
		}

		if (cmp(prop->name,name))
			return prop->offset;
	}

	return 0;
}