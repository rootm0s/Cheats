enum
{
	deagle = 1,
	p2000 = 32,
	fiveseven = 3,
	glock = 4,
	dualelites = 2,
	ak = 7,
	aug = 8,
	awp = 9,
	famas = 10,
	g3sg1 = 11,
	galil = 13,
	p250 = 36,
	tec9 = 30,
	m249 = 14,
	m4 = 16,
	mac10 = 17,
	p90 = 19,
	ump45 = 24,
	ppbizon = 26,
	negev = 28,
	mp7 = 33,
	mp9 = 34,
	scar30 = 38,
	sg553 = 39,
	ssg08 = 40,
	goldknife = 41,
	knife = 42,
	flashgren = 43,
	hegren = 44,
	smoke = 45,
	molotov = 46,
	incendiary = 48,
	decoygren = 47,
	bomb = 49,
	nova = 35,
	xm1014 = 25,
	sawedoff = 29,
	mag7 = 27,
	zeus = 31
};

namespace ValveSDK
{
	class CBaseCombatWeapon
	{
	public:
		float GetNextPrimaryAttack()
		{
			static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
			return *(float*)((DWORD)this + iOffset);
		}

		int Clip1()
		{
			static int iOffset = g_NetworkedVariableManager.GetOffset("DT_BaseCombatWeapon","m_iClip1");
			return *(PINT)((DWORD)this + iOffset);
		}

		bool HasAmmo()
		{
			return (Clip1() > 0);
		}

		float GetSpread()
		{
			typedef float(__thiscall *GetSpread_t)(PVOID);
			return ((GetSpread_t)(*(PDWORD)(*(PDWORD)(this) + GETSPREADOFFSET)))(this);
		}

		float GetCone()
		{
			typedef float(__thiscall *GetCone_t)(PVOID);
			return ((GetCone_t)(*(PDWORD)(*(PDWORD)(this) + GETCONEOFFSET)))(this);
		}

		void UpdateAccuracyPenalty()
		{
			typedef void(__thiscall *UpdateAccuracyPenalty_t)(PVOID);
			((UpdateAccuracyPenalty_t)(*(PDWORD)(*(PDWORD)(this) + UPDATEACCURACYPENALTYOFFSET)))(this);
		}

		/*
		int GetWeaponID()
		{
			typedef int(__thiscall *GetWeaponID_t)(PVOID);
			return ((GetWeaponID_t)(*(PDWORD)(*(PDWORD)(this) + WEAPONIDOFFSET)))(this);
		}
		*/

		int GetWeaponID()
		{
			return *(int*)((DWORD)this + 0x1148 + 0x40 + 0x194); //m_AttributeManager + m_Item + m_iItemDefinitionIndex
		}

		uintptr_t GetWeaponData()
		{
			typedef uintptr_t(__thiscall *GetWeaponData_t)(PVOID);
			return ((GetWeaponData_t)(*(PDWORD)(*(PDWORD)(this) + WEAPONDATAOFFSET)))(this);
		}

		PCHAR GetName()
		{
			typedef PCHAR(__thiscall *GetWeaponName_t)(PVOID);
			return ((GetWeaponName_t)(*(PDWORD)(*(PDWORD)(this) + GETNAMEOFFSET)))(this);
		}

		/*
		void GetDecayedRecoilAngle()
		{
			typedef void(__thiscall *GetDecayedRecoilAngle_t)(PVOID);
			return ((GetDecayedRecoilAngle_t)(*(PDWORD)(*(PDWORD)(this) + 0x76)))(this);
		}*/

		bool IsShotgun()
		{
			int iWeaponID = GetWeaponID();

			return (iWeaponID == zeus || iWeaponID == mag7 || iWeaponID == sawedoff || iWeaponID == xm1014 || iWeaponID == nova);
		}

		bool IsSniper()
		{
			int iWeaponID = GetWeaponID();

			return (iWeaponID == 38 || iWeaponID == 11 || iWeaponID == 9 || iWeaponID == 40);
		}

		bool IsPistol()
		{
			int iWeaponID = GetWeaponID();

			return (iWeaponID == glock || iWeaponID == p2000 
				|| iWeaponID == p250 || iWeaponID == deagle 
				|| iWeaponID == dualelites || iWeaponID == tec9 
				|| iWeaponID == fiveseven);
		}

		bool IsMiscWeapon()
		{
			int iWeaponID = GetWeaponID();
			return (iWeaponID == knife
				|| iWeaponID == goldknife || iWeaponID == bomb
				|| iWeaponID == hegren || iWeaponID == decoygren
				|| iWeaponID == flashgren || iWeaponID == molotov
				|| iWeaponID == smoke || iWeaponID == incendiary || iWeaponID == 59
				|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
				|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
				|| iWeaponID == 515);
		}

		bool IsKnife()
		{
			int iWeaponID = GetWeaponID();
			return (iWeaponID == knife || iWeaponID == 59 || iWeaponID == 41
				|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
				|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
				|| iWeaponID == 515);
		}
	};
}