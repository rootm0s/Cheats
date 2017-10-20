class Entity;

struct StencilState
{
	bool enable;
	int fail;
	int zfail;
	int pass;
	int compare;
	int ref;
	int mask1;
	int mask2;
};

class GameEvent
{
public:
	bool GetBool(char* name)
	{
		typedef bool (__thiscall* dfn)(void*,char*,int);
		return ((dfn)(*(long**)this)[5])(this,name,0);
	}
	int GetInt(char* name)
	{
		typedef int (__thiscall* dfn)(void*,char*,int);
		return ((dfn)(*(long**)this)[6])(this,name,0);
	}
	char* GetString(char* name)
	{
		typedef char* (__thiscall* dfn)(void*,char*,int);
		return ((dfn)(*(long**)this)[8])(this,name,0);
	}
};

class Material
{
public:
	void AddReference()
	{
		typedef void (__thiscall* dfn)(void*);
		((dfn)Virtual(this,12))(this);
	}
	void SetVarFlag(int flag,bool setting)
	{
		typedef void (__thiscall* dfn)(void*,int,bool);
		((dfn)Virtual(this,29))(this,flag,setting);
	}
};

struct ConVar
{
	char pad1[19];
	int flags;
	char pad4[13];
	char* svalue;
	char pad2[4];
	float fvalue;
	int ivalue;
	char pad3[14];
	int callback;
};

struct GlobalVars
{
	char pad[16];
	float curtime;
	float frametime;
	int clients;
	int ticks;
	float interval;
};

struct PlayerInfo
{
	char pad[16];
	char name[32];
	int userid;
	char guid[33];
	int pad1;
	char pad2[32];
	bool bot;
	char pad3[256];
};

struct UserCmd
{
	int vmt;
	int index;
	int ticks;
	Vector ang;
	Vector dir;
	Vector move;
	int	buttons;
};

struct TraceResult
{
	char pad1[12];
	Vector hitpos;
	char pad2[19];
	float fraction;
	char pad3[16];
	short props;
	char pad4[10];
	Entity* ent;
	int hitbox;
};

struct RayData
{
	RayData(Vector x,Vector y)
	{
		base = x;
		delta = y - x;
		isray = true;
		hasdelta = true;
	}

	Vector base;
	int pad1;
	Vector delta;
	int pad2;
	Vector offset;
	int pad3;
	Vector extents;
	int pad4;
	int pad5;
	bool isray;
	bool hasdelta;
};

struct TraceFilter
{
	virtual bool ShouldHitEntity(Entity* ent,int) { return ent != local && ent != ignore; }
	virtual int GetTraceType() { return 0; }

	char pad[8];
	Entity* local;
	Entity* ignore;
};

struct RenderInfo
{
	char pad1[28];
	long* model;
	char pad2[16];
	int index;
};

class fisherman
{
private:
	int ivirtual_function_index;
	PDWORD pvtable;
	PDWORD pfunction_pointer;
	DWORD dnew_function;
	DWORD dold_function;
	DWORD dpadding00 = 500;
	DWORD dpadding01 = 504;
public:
	DWORD fish(PDWORD* _pvtable, DWORD _dnewfunction, int _ivirtual_function_index)
	{
		pvtable = *(PDWORD*)_pvtable;
		ivirtual_function_index = _ivirtual_function_index;
		dnew_function = _dnewfunction;
		dold_function = pvtable[ivirtual_function_index];
		pfunction_pointer = &pvtable[ivirtual_function_index];

		MEMORY_BASIC_INFORMATION mdata;
		VirtualQuery((LPVOID)pfunction_pointer, &mdata, sizeof(mdata));
		VirtualProtect((LPVOID)mdata.BaseAddress, mdata.RegionSize, PAGE_EXECUTE_READWRITE, &mdata.Protect);
		pvtable[ivirtual_function_index] = dnew_function;
		VirtualProtect((LPVOID)mdata.BaseAddress, mdata.RegionSize, mdata.Protect, NULL);
		FlushInstructionCache(GetCurrentProcess(), mdata.BaseAddress, sizeof(DWORD));

		return dold_function;
	}
};