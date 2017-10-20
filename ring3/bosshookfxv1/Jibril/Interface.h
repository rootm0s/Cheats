#pragma once 

#include "SDK.h"
#include "Menu.h"

class CNetvars;
class CSettings;
class CMenu;

class CInterface
{
public:
	CInterface();
public:
	IBaseClient* BaseClient;
	CInput*	Input;
	IClientEntityList* EntityList;
	CPrediction* Prediction;
	CGameMovement* GameMovement;
	IMoveHelper* MoveHelper;
	IVEngineClient* EngineClient;
	IEngineTrace* EngineTrace;
	IVModelInfo* ModelInfo;
	IVModelRender* ModelRender;
	IVRenderView* RenderView;
	ISurface* Surface;
	IPanel* Panel;
	IMaterialSystem* MaterialSystem;
	CGlobalVarsBase* GlobalVars;
	COffsets* Offsets;
	CNetvars* NetVars;
	CSettings* Settings;
	CMenu* Menu;
public:
	CVMTHook* BaseClientVMT;
	CVMTHook* PredictionVMT;
	CVMTHook* ModelRenderVMT;
	CVMTHook* MaterialSystemVMT;
	CVMTHook* PanelVMT;
};

extern CInterface Interface;

class CNetvars
{
public:
	void Init()
	{
		m_tables.clear();

		ClientClass *clientClass = Interface.BaseClient->GetAllClasses();

		if (!clientClass)
			return;

		while (clientClass)
		{
			RecvTable *recvTable = clientClass->m_pRecvTable;

			m_tables.push_back(recvTable);

			clientClass = clientClass->m_pNext;
		}
	}

	int GetNetPropOffset(RecvTable* propTable, const char *propName)
	{
		int iOffset = 0;
		int iExtraOffset = 0;

		for (int i = 0; i < propTable->GetNumProps(); i++)
		{
			RecvProp* pProp = propTable->GetProp(i);

			if (pProp->m_pDataTable && pProp->m_pDataTable->GetNumProps() > 0)
			{
				int tmp = GetNetPropOffset(pProp->m_pDataTable, propName);

				if (tmp)
				{
					iExtraOffset += (pProp->GetOffset() + tmp);
				}
			}

			if (!strcmp(pProp->GetName(), propName))
			{
				iOffset = pProp->GetOffset();
				break;
			}
		}

		return iOffset + iExtraOffset;
	}

	int GetNetPropOffset(const char *tableName, const char *propName)
	{
		int iOffset = 0;
		int iExtraOffset = 0;

		for (unsigned int in = 0; in < m_tables.size(); ++in)
		{
			const char *pszName = m_tables[in]->GetName();

			if (!strcmp(pszName, tableName))
			{
				for (int i = 0; i < m_tables[in]->GetNumProps(); i++)
				{
					RecvProp* pProp = m_tables[in]->GetProp(i);

					if (pProp->m_pDataTable && pProp->m_pDataTable->GetNumProps() > 0)
					{
						int tmp = GetNetPropOffset(pProp->m_pDataTable, propName);

						if (tmp)
						{
							iExtraOffset += (pProp->GetOffset() + tmp);
						}
					}

					if (!strcmp(pProp->GetName(), propName))
					{
						iOffset = pProp->GetOffset();
						break;
					}
				}
			}

			if (iOffset > 0)
				break;
		}

		return iOffset + iExtraOffset;
	}

	void HookNetProp(const char *tableName, const char *propName, RecvVarProxyFn fn)
	{
		bool bBreak = false;
#ifdef DEBUG
		Utilities::DebugPrint("Trying to hook: %s, %s", tableName, propName);
#endif
		for (unsigned int in = 0; in < m_tables.size(); ++in)
		{
			const char *pszName = m_tables[in]->GetName();
#ifdef DEBUG
			//Utilities::DebugPrint("Table Index: %i, Name: %s", in, pszName);
#endif
			if (!strcmp(pszName, tableName))
			{
#ifdef DEBUG
				Utilities::DebugPrint("Found Hook Table");
#endif
				for (int i = 0; i < m_tables[in]->m_nProps; i++)
				{
					RecvProp *pProp = m_tables[in]->GetProp(i);
#ifdef DEBUG
					//Utilities::DebugPrint("NetProp Index: %i, Name: %s", i, pProp->GetName());
#endif
					if (!strcmp(pProp->GetName(), propName))
					{
						pProp->SetProxyFn(fn);
#ifdef DEBUG
						Utilities::DebugPrint("Hooked %s", pProp->GetName());
#endif
						bBreak = true;
						break;
					}
				}
			}

			if (bBreak)
				break;
		}
	}

	void GetOffsets(void)
	{
		Interface.Offsets->Flags = GetNetPropOffset("DT_BasePlayer", "m_fFlags");
		Interface.Offsets->Health = GetNetPropOffset("DT_BasePlayer", "m_iHealth");
		Interface.Offsets->LifeState = GetNetPropOffset("DT_BasePlayer", "m_lifeState");
		Interface.Offsets->PunchAngle = GetNetPropOffset("DT_BasePlayer", "m_vecPunchAngle");
		Interface.Offsets->TeamNum = GetNetPropOffset("DT_BasePlayer", "m_iTeamNum");
		Interface.Offsets->TickBase = GetNetPropOffset("DT_BasePlayer", "m_nTickBase");
		Interface.Offsets->Velocity = GetNetPropOffset("DT_BasePlayer", "m_vecVelocity[0]");
		Interface.Offsets->ViewOffset = GetNetPropOffset("DT_BasePlayer", "m_vecViewOffset[0]");
		Interface.Offsets->Origin = GetNetPropOffset("DT_BasePlayer", "m_vecOrigin");
		Interface.Offsets->HitboxSet = GetNetPropOffset("DT_BaseAnimating", "m_nHitboxSet");
		Interface.Offsets->ShotsFired = GetNetPropOffset("DT_CSPlayer", "m_iShotsFired");
		Interface.Offsets->Angles = GetNetPropOffset("DT_CSPlayer", "m_angEyeAngles");
		Interface.Offsets->IsDefusing = GetNetPropOffset("DT_CSPlayer", "m_bIsDefusing");
		Interface.Offsets->clrRender = GetNetPropOffset("DT_BaseEntity", "m_clrRender");
		Interface.Offsets->PunchAngle = GetNetPropOffset("DT_BasePlayer", "m_aimPunchAngle");
		Interface.Offsets->ViewPunchAngle = GetNetPropOffset("DT_BasePlayer", "m_viewPunchAngle");
		Interface.Offsets->OwnerXuidLow = GetNetPropOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		Interface.Offsets->OwnerXuidHigh = GetNetPropOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		Interface.Offsets->PaintKit = GetNetPropOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		Interface.Offsets->Wear = GetNetPropOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		Interface.Offsets->StatTrak = GetNetPropOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		Interface.Offsets->Seed = GetNetPropOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		Interface.Offsets->ItemIDHigh = GetNetPropOffset("DT_BaseAttributableItem", "m_iItemIDHigh");
		Interface.Offsets->CustomName = GetNetPropOffset("DT_BaseAttributableItem", "m_szCustomName");
		Interface.Offsets->ItemID = GetNetPropOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
		Interface.Offsets->ModelIndex = GetNetPropOffset("DT_BaseCombatWeapon", "m_nModelIndex");
		Interface.Offsets->ViewModelIndex = GetNetPropOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		Interface.Offsets->WorldModelIndex = GetNetPropOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
		Interface.Offsets->hOwnerEntity = GetNetPropOffset("DT_BaseEntity", "m_hOwnerEntity");
		Interface.Offsets->IsScoped = GetNetPropOffset("DT_CSPlayer", "m_bIsScoped");
		Interface.Offsets->GlowIndex = GetNetPropOffset("DT_CSPlayer", "m_flFlashDuration") + 0x18;
		Interface.Offsets->CompetitiveRank = GetNetPropOffset("DT_CSPlayerResource", "m_iCompetitiveRanking");
		Interface.Offsets->ActiveWeapon = GetNetPropOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		Interface.Offsets->Clip1 = GetNetPropOffset("DT_BaseCombatWeapon", "m_iClip1");
		Interface.Offsets->NextPrimaryAttack = GetNetPropOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		Interface.Offsets->Min = GetNetPropOffset("DT_BaseEntity", "m_vecMins");
		Interface.Offsets->Max = GetNetPropOffset("DT_BaseEntity", "m_vecMaxs");
		Interface.Offsets->Rotation = GetNetPropOffset("DT_BaseEntity", "m_angRotation");
		Interface.Offsets->Spotted = GetNetPropOffset("DT_BaseEntity", "m_bSpotted");
		Interface.Offsets->C4Blow = GetNetPropOffset("DT_PlantedC4", "m_flC4Blow");
#ifdef DEBUG
		Utilities::DebugPrint("Flags: 0x%x", Interface.Offsets->Flags);
		Utilities::DebugPrint("Health: 0x%x", Interface.Offsets->Health);
		Utilities::DebugPrint("LifeState: 0x%x", Interface.Offsets->LifeState);
		Utilities::DebugPrint("PunchAngle: 0x%x", Interface.Offsets->PunchAngle);
		Utilities::DebugPrint("TeamNum: 0x%x", Interface.Offsets->TeamNum);
		Utilities::DebugPrint("IsDefusing: 0x%x", Interface.Offsets->IsDefusing);
		Utilities::DebugPrint("TickBase: 0x%x", Interface.Offsets->TickBase);
		Utilities::DebugPrint("Velocity: 0x%x", Interface.Offsets->Velocity);
		Utilities::DebugPrint("ViewOffset: 0x%x", Interface.Offsets->ViewOffset);
		Utilities::DebugPrint("Origin: 0x%x", Interface.Offsets->Origin);
		Utilities::DebugPrint("Angles: 0x%x", Interface.Offsets->Angles);
		Utilities::DebugPrint("ShotsFired: 0x%x", Interface.Offsets->ShotsFired);
		Utilities::DebugPrint("Clip1: 0x%x", Interface.Offsets->Clip1);
		Utilities::DebugPrint("NextPrimaryAttack: 0x%x", Interface.Offsets->NextPrimaryAttack);
		Utilities::DebugPrint("ActiveWeapon: 0x%x", Interface.Offsets->ActiveWeapon);
		Utilities::DebugPrint("HitboxSet: 0x%x", Interface.Offsets->HitboxSet);
		Utilities::DebugPrint("ViewPunchAngle: 0x%x", Interface.Offsets->ViewPunchAngle);
		Utilities::DebugPrint("clrRender: 0x%x", Interface.Offsets->clrRender);
		Utilities::DebugPrint("OwnerXuidLow: 0x%x", Interface.Offsets->OwnerXuidLow);
		Utilities::DebugPrint("OwnerXuidHigh: 0x%x", Interface.Offsets->OwnerXuidHigh);
		Utilities::DebugPrint("FallbackPaintKit: 0x%x", Interface.Offsets->PaintKit);
		Utilities::DebugPrint("FallbackWear: 0x%x", Interface.Offsets->Wear);
		Utilities::DebugPrint("FallbackStatTrak: 0x%x", Interface.Offsets->StatTrak);
		Utilities::DebugPrint("FallbackSeed: 0x%x", Interface.Offsets->Seed);
		Utilities::DebugPrint("iItemIDHigh: 0x%x", Interface.Offsets->ItemIDHigh);
		Utilities::DebugPrint("szCustomName: 0x%x", Interface.Offsets->CustomName);
		Utilities::DebugPrint("iItemDefinitionIndex: 0x%x", Interface.Offsets->ItemID);
		Utilities::DebugPrint("ModelIndex: 0x%x", Interface.Offsets->ModelIndex);
		Utilities::DebugPrint("ViewModelIndex: 0x%x", Interface.Offsets->ViewModelIndex);
		Utilities::DebugPrint("WorldModelIndex: 0x%x", Interface.Offsets->WorldModelIndex);
		Utilities::DebugPrint("IsScoped: 0x%x", Interface.Offsets->IsScoped);
		Utilities::DebugPrint("GlowIndex: 0x%x", Interface.Offsets->GlowIndex);
		Utilities::DebugPrint("CompetitiveRank: 0x%x", Interface.Offsets->CompetitiveRank);
		Utilities::DebugPrint("Min: 0x%x", Interface.Offsets->Min);
		Utilities::DebugPrint("Max: 0x%x", Interface.Offsets->Max);
		Utilities::DebugPrint("Rotation: 0x%x", Interface.Offsets->Rotation);
		Utilities::DebugPrint("Spotted: 0x%x", Interface.Offsets->Spotted);
		Utilities::DebugPrint("C4Blow: 0x%x", Interface.Offsets->C4Blow);
#endif
	}

	std::vector<RecvTable*>	m_tables;
};

class CEntity
{
public:
	void* GetClientNetworkable()
	{
		return reinterpret_cast<void*>((DWORD)this + 0x8);
	}

	void* GetClientRenderable()
	{
		return reinterpret_cast<void*>((DWORD)this + 0x4);
	}

	const void*	GetModel()
	{
		typedef const void*(__thiscall* GetModel_t)(void*);
		return Utilities::GetVFunction<GetModel_t>(this->GetClientRenderable(), 8)(this->GetClientRenderable());
	}

	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* SetupBones_t)(void*, matrix3x4_t*, int, int, float);
		return Utilities::GetVFunction<SetupBones_t>(this->GetClientRenderable(), 13)(this->GetClientRenderable(), pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	ClientClass* GetClientClass(void)
	{
		typedef ClientClass*(__thiscall* GetClientClass_t)(void*);
		return Utilities::GetVFunction<GetClientClass_t>(this->GetClientNetworkable(), 2)(this->GetClientNetworkable());
	}

	bool IsDormant(void)
	{
		typedef bool(__thiscall* IsDormant_t)(void*);
		return Utilities::GetVFunction<IsDormant_t>(this->GetClientNetworkable(), 9)(this->GetClientNetworkable());
	}

	int	entindex(void)
	{
		typedef int(__thiscall* entindex_t)(void*);
		return Utilities::GetVFunction<entindex_t>(this->GetClientNetworkable(), 10)(this->GetClientNetworkable());
	}

	template<typename T> T ReadNetVar(DWORD dwNetVar)
	{
		return *(T*)((DWORD)this + dwNetVar);
	}

	template<typename T> void OverrideNetVar(DWORD dwNetVar, T value)
	{
		*(T*)((DWORD)this + dwNetVar) = value;
	}

	bool GetLifeState()
	{
		char szLifeState = this->ReadNetVar<char>(Interface.Offsets->LifeState);
		return (szLifeState == 0);
	}

	int GetFlags()
	{
		return this->ReadNetVar<int>(Interface.Offsets->Flags);
	}

	Vector GetPunchAngle()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->PunchAngle);
	}

	int GetTeamID()
	{
		return this->ReadNetVar<int>(Interface.Offsets->TeamNum);
	}

	bool IsDefusing()
	{
		return this->ReadNetVar<int>(Interface.Offsets->IsDefusing);
	}

	int GetTickBase()
	{
		return this->ReadNetVar<int>(Interface.Offsets->TickBase);
	}

	Vector GetVelocity()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->Velocity);
	}

	int GetHealth()
	{
		return this->ReadNetVar<int>(Interface.Offsets->Health);
	}

	MoveType_t GetMoveType()//hardcoded weil kein NetVar
	{
		return this->ReadNetVar<MoveType_t>(0x258);
	}

	CEntity* GetBaseCombatWeapon()
	{
		EHANDLE hActiveWeapon = this->ReadNetVar<EHANDLE>(Interface.Offsets->ActiveWeapon);
		return Interface.EntityList->GetClientEntityFromHandle(hActiveWeapon);
	}

	CEntity* GetOwnerEntity()
	{
		EHANDLE hOwner = this->ReadNetVar<EHANDLE>(Interface.Offsets->hOwnerEntity);
		return Interface.EntityList->GetClientEntityFromHandle(hOwner);
	}

	Vector GetEyePos()
	{
		Vector m_VecOrigin = this->ReadNetVar<Vector>(Interface.Offsets->Origin);
		Vector m_vecViewOffset = this->ReadNetVar<Vector>(Interface.Offsets->ViewOffset);

		return (m_VecOrigin + m_vecViewOffset);
	}

	Vector GetOrigin()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->Origin);
	}

	int GetShotsFired()
	{
		return this->ReadNetVar<int>(Interface.Offsets->ShotsFired);
	}

	int GetHitboxSet()
	{
		return this->ReadNetVar<int>(Interface.Offsets->HitboxSet);
	}

	bool IsShootableWeapon()
	{
		if (this->GetBaseCombatWeapon() == NULL) return false;

		int WeaponID = this->GetBaseCombatWeapon()->ReadNetVar<int>(Interface.Offsets->ItemID);

		if (WeaponID == weapon_none
			|| WeaponID == weapon_knife_t
			|| WeaponID == weapon_knife_ct
			|| WeaponID == weapon_bayonet
			|| WeaponID == weapon_gut
			|| WeaponID == weapon_huntsman
			|| WeaponID == weapon_karambit
			|| WeaponID == weapon_m9bayonet
			|| WeaponID == weapon_flip
			|| WeaponID == weapon_butterfly
			|| WeaponID == weapon_falchion
			|| WeaponID == weapon_pushdagger
			|| WeaponID == weapon_flashbang
			|| WeaponID == weapon_hegrenade
			|| WeaponID == weapon_smokegrenade
			|| WeaponID == weapon_molotov
			|| WeaponID == weapon_decoy
			|| WeaponID == weapon_incgrenade
			|| WeaponID == weapon_c4)
		{
			return false;
		}

		return true;
	}

	Vector GetMins()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->Min);
	}

	Vector GetMaxs()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->Max);
	}

	Vector GetRotation()
	{
		return this->ReadNetVar<Vector>(Interface.Offsets->Rotation);
	}

	bool HasAmmo()
	{
		if (this->GetBaseCombatWeapon() == NULL) return false;

		int iAmmo = this->GetBaseCombatWeapon()->ReadNetVar<int>(Interface.Offsets->Clip1);

		if (iAmmo == 0)
			return false;
		else
			return true;
	}

	float GetNextPrimaryAttack()
	{
		if (this->GetBaseCombatWeapon() == NULL) return 0.0f;

		return this->GetBaseCombatWeapon()->ReadNetVar<float>(Interface.Offsets->NextPrimaryAttack);
	}

	Color GetRenderColor()
	{
		return this->ReadNetVar<Color>(Interface.Offsets->clrRender);
	}

	bool IsScoped()
	{
		return this->ReadNetVar<bool>(Interface.Offsets->IsScoped);
	}

	int GetRank()
	{
		DWORD dwTemp = *(DWORD*)dwGameResources;

		if (dwTemp == NULL)
			return 0;
		else
			return *(int*)(dwTemp + Interface.Offsets->CompetitiveRank + this->entindex() * 4);
	}


	bool isSpotted()
	{
		return this->ReadNetVar<bool>(Interface.Offsets->Spotted);
	}

};

class CSettings
{
	
public:
	CHAR fov_temp[45], smooth_temp[45];
	float fov = 1.2f, smooth = 1.0f;
	int bone = 6, intens_x = 70, intens_y = 100, t_key = 6, a_key = 1;
	bool rcs = true,
		aim = true,
		trigger = true,
		bhop = true,
		weapon_esp = 0,
		esp = true,
		box = true,
		hitboxdot = true,
		rank = false,
		name = false,
		weapon = true,
		head = true,
		neck = true,
		body = true,
		arms = true,
		legs = false,
		weaponcfg = false,
		glow = true,
		legit = true,
		rdot = true,
		defuseglow = true,
		chams = true,
		healthbar = true,
		health = true,
		autoduck = true,
		autoshoot = true,
		silentaim = true,
		crosshair = true,
		autopistol = true,
		norecoil = true,
		skeleton = false;

	void ReadSettings()
	{
		aim = !!GetPrivateProfileInt("Settings", "Aimbot.Enable", 1, "C:\\settings.ini");
		a_key = GetPrivateProfileInt("Settings", "Aimbot.Key", 1, "C:\\Settings.ini");
		GetPrivateProfileString("Settings", "Aimbot.FOV", "1.5", fov_temp, 32, "C:\\settings.ini");
		fov = atof(fov_temp);
		GetPrivateProfileString("Settings", "Aimbot.Smooth", "1.0", smooth_temp, 32, "C:\\settings.ini");
		smooth = atof(smooth_temp);
		bone = GetPrivateProfileInt("Settings", "Aimbot.Bone", 0, "C:\\settings.ini");
		autoshoot = !!GetPrivateProfileInt("Settings", "Aimbot.AutoShoot", 0, "C:\\Settings.ini");
		silentaim = !!GetPrivateProfileInt("Settings", "Aimbot.SilentAim", 0, "C:\\Settings.ini");
		autopistol = !!GetPrivateProfileInt("Settings", "Aimbot.AutoPistol", 0, "C:\\Settings.ini");
		rcs = !!GetPrivateProfileInt("Settings", "Aimbot.RCS", 1, "C:\\settings.ini");
		intens_x = GetPrivateProfileInt("Settings", "Aimbot.RCS_X", 1, "C:\\settings.ini");
		intens_y = GetPrivateProfileInt("Settings", "Aimbot.RCS_Y", 1, "C:\\settings.ini");

		trigger = !!GetPrivateProfileInt("Settings", "Trigger.Enable", 1, "C:\\settings.ini");
		t_key = GetPrivateProfileInt("Settings", "Trigger.Key", 6, "C:\\settings.ini");
		head = !!GetPrivateProfileInt("Settings", "Trigger.Head", 1, "C:\\settings.ini");
		neck = !!GetPrivateProfileInt("Settings", "Trigger.Neck", 1, "C:\\settings.ini");
		body = !!GetPrivateProfileInt("Settings", "Trigger.Body", 1, "C:\\settings.ini");
		arms = !!GetPrivateProfileInt("Settings", "Trigger.Arms", 1, "C:\\settings.ini");
		legs = !!GetPrivateProfileInt("Settings", "Trigger.Legs", 0, "C:\\settings.ini");

		weapon_esp = !!GetPrivateProfileInt("Settings", "EntityESP.Enable", 0, "C:\\settings.ini");

		esp = !!GetPrivateProfileInt("Settings", "ESP.Enable", 1, "C:\\settings.ini");
		box = !!GetPrivateProfileInt("Settings", "ESP.Box", 1, "C:\\settings.ini");
		chams = !!GetPrivateProfileInt("Settings", "ESP.Chams", 1, "C:\\settings.ini");
		health = GetPrivateProfileInt("Settings", "ESP.Health", 1, "C:\\settings.ini");
		healthbar = GetPrivateProfileInt("Settings", "ESP.Healthbar", 1, "C:\\settings.ini");
		legit = !!GetPrivateProfileInt("Settings", "ESP.Legit", 1, "C:\\settings.ini");
		glow = !!GetPrivateProfileInt("Settings", "ESP.Glow", 1, "C:\\settings.ini");
		defuseglow = !!GetPrivateProfileInt("Settings", "ESP.DefuseGlow", 1, "C:\\settings.ini");
		hitboxdot = !!GetPrivateProfileInt("Settings", "ESP.HitboxDot", 1, "C:\\settings.ini");
		rank = !!GetPrivateProfileInt("Settings", "ESP.Wins", 0, "C:\\settings.ini");
		name = !!GetPrivateProfileInt("Settings", "ESP.Name", 0, "C:\\settings.ini");
		weapon = !!GetPrivateProfileInt("Settings", "ESP.Weapon", 1, "C:\\settings.ini");
		skeleton = !!GetPrivateProfileInt("Settings", "ESP.Skeleton", 0, "C:\\settings.ini");

		norecoil = !!GetPrivateProfileInt("Settings", "NoRecoil.Enable", 1, "C:\\settings.ini");
		bhop = !!GetPrivateProfileInt("Settings", "BHOP.Enable", 1, "C:\\settings.ini");
		rdot = !!GetPrivateProfileInt("Settings", "RDot.Enable", 1, "C:\\settings.ini");
		crosshair = !!GetPrivateProfileInt("Settings", "Crosshair.Enable", 0, "C:\\Settings.ini");
		autoduck = !!GetPrivateProfileInt("Settings", "AutoDuck.Enable", 0, "C:\\settings.ini");

		weaponcfg = !!GetPrivateProfileInt("Settings", "Weaponconfigs", 1, "C:\\settings.ini");
	}

	void ReadSettings(std::string weap)
	{
			aim = GetPrivateProfileInt(weap.c_str(), "Aimbot.Enable", 1, "C:\\settings.ini");
			a_key = GetPrivateProfileInt(weap.c_str(), "Aimbot.Key", 1, "C:\\settings.ini");
			fov = GetPrivateProfileString(weap.c_str(), "Aimbot.FOV", "1.5", fov_temp, 32, "C:\\settings.ini");
			fov = atof(fov_temp);
			GetPrivateProfileString(weap.c_str(), "Aimbot.Smooth", "1.0", smooth_temp, 32, "C:\\settings.ini");
			smooth = atof(smooth_temp);
			bone = GetPrivateProfileInt(weap.c_str(), "Aimbot.Bone", 0, "C:\\settings.ini");
			rcs = GetPrivateProfileInt(weap.c_str(), "Aimbot.RCS", 1, "C:\\settings.ini");
			intens_x = GetPrivateProfileInt(weap.c_str(), "Aimbot.RCS_X", 1, "C:\\settings.ini");
			intens_y = GetPrivateProfileInt(weap.c_str(), "Aimbot.RCS_Y", 1, "C:\\settings.ini");

			trigger = GetPrivateProfileInt(weap.c_str(), "Trigger.Enable", 1, "C:\\settings.ini");
			t_key = GetPrivateProfileInt(weap.c_str(), "Trigger.Key", 6, "C:\\settings.ini");
			head = GetPrivateProfileInt(weap.c_str(), "Trigger.Head", 1, "C:\\settings.ini");
			neck = GetPrivateProfileInt(weap.c_str(), "Trigger.Neck", 1, "C:\\settings.ini");
			body = GetPrivateProfileInt(weap.c_str(), "Trigger.Body", 1, "C:\\settings.ini");
			arms = GetPrivateProfileInt(weap.c_str(), "Trigger.Arms", 1, "C:\\settings.ini");
			legs = GetPrivateProfileInt(weap.c_str(), "Trigger.Legs", 0, "C:\\settings.ini.ini");
	}
};