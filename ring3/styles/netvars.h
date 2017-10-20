class NetvarManager
{
public:
	NetvarManager() { Init(); }
	void Init();
	int GetOffset(char*,char*);
	int LookupNetvar(void*,char*);

	int m_lifeState;
	int m_iHealth;
	int m_fFlags;
	int m_nTickBase;
	int m_pCurrentCommand;
	int m_aimPunchAngle;
	int m_viewPunchAngle;
	int m_vecViewOffset;
	int m_vecVelocity;
	int m_angEyeAngles;
	int m_clrRender;
	int m_iTeamNum;
	int m_vecOrigin;
	int m_vecMins;
	int m_vecMaxs;
	int m_hActiveWeapon;
	int m_flNextAttack;
	int m_hOwner;
	int m_iClip1;
	int m_flNextPrimaryAttack;
};