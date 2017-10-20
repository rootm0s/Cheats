
// organize the entity's info in a parent struct
struct Info
{
	inline long* GetCurrentCommand() { return (long*)(this + netvars->m_pCurrentCommand); }

	inline char* GetColor() { return (char*)(this + netvars->m_clrRender); }

	inline int GetLifeState() { return *(char*)(this + netvars->m_lifeState); }
	inline int GetHealth() { return *(int*)(this + netvars->m_iHealth); }
	inline int GetFlags() { return *(int*)(this + netvars->m_fFlags); }
	inline int& GetTickBase() { return *(int*)(this + netvars->m_nTickBase); }
	inline int GetTeam() { return *(int*)(this + netvars->m_iTeamNum); }
	inline int GetActiveWeapon() { return *(int*)(this + netvars->m_hActiveWeapon); }
	inline int GetOwner() { return *(int*)(this + netvars->m_hOwner); }
	inline int GetPrimaryClip() { return *(int*)(this + netvars->m_iClip1); }

	inline float GetNextAttack() { return *(float*)(this + netvars->m_flNextAttack); }
	inline float GetNextPrimaryAttack() { return *(float*)(this + netvars->m_flNextPrimaryAttack); }

	inline Vector GetOrigin() { return *(Vector*)(this + netvars->m_vecOrigin); }
	inline Vector GetMins() { return *(Vector*)(this + netvars->m_vecMins); }
	inline Vector GetMaxs() { return *(Vector*)(this + netvars->m_vecMaxs); }
	inline Vector GetAimPunch() { return *(Vector*)(this + netvars->m_aimPunchAngle); }
	inline Vector GetViewPunch() { return *(Vector*)(this + netvars->m_viewPunchAngle); }
	inline Vector GetViewOffset() { return *(Vector*)(this + netvars->m_vecViewOffset); }
	inline Vector& GetVelocity() { return *(Vector*)(this + netvars->m_vecVelocity); }
	inline Vector& GetEyeAngles() { return *(Vector*)(this + netvars->m_angEyeAngles); }
};