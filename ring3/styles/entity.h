#include "info.h"

struct Class
{
	char pad1[8];
	char* name;
	void* table;
	Class* next;
	int id;
};

struct Hitbox
{
	int bone;
	int group;
	Vector min;
	Vector max;
	int pad[9];
};

class Entity : public Info
{
public:
	inline void* GetRenderable() { return this + 0x4; }
	inline void* GetNetworkable() { return this + 0x8; }

	inline bool SetupBones(void* mtx)
	{
		void* p = this->GetRenderable();

		typedef bool (__thiscall* dfn)(void*,void*,int,long,float);
		return ((dfn)(*(long**)p)[13])(p,mtx,128,0x100,0);
	}
	inline Vector& GetRenderOrigin()
	{
		void* p = this->GetRenderable();

		typedef Vector& (__thiscall* dfn)(void*);
		return ((dfn)(*(long**)p)[1])(p);
	}
	inline void* GetModel()
	{
		void* p = this->GetRenderable();

		typedef void* (__thiscall* dfn)(void*);
		return ((dfn)(*(long**)p)[8])(p);
	}
	inline Class* GetClass()
	{
		void* p = this->GetNetworkable();

		typedef Class* (__thiscall* dfn)(void*);
		return ((dfn)(*(long**)p)[2])(p);
	}
	inline bool IsDormant()
	{
		void* p = this->GetNetworkable();

		typedef bool (__thiscall* dfn)(void*);
		return ((dfn)(*(long**)p)[9])(p);
	}
	inline int GetIndex()
	{
		void* p = this->GetNetworkable();

		typedef int (__thiscall* dfn)(void*);
		return ((dfn)(*(long**)p)[10])(p);
	}
	inline void DrawModel()
	{
		void* p = this->GetRenderable();

		typedef int (__thiscall* dfn)(void*,int,void*);
		((dfn)(*(long**)p)[9])(p,1,0);
	}
	inline bool IsAlive()
	{
		if (GetLifeState() != 0)
			return false;

		if (GetHealth() <= 0)
			return false;

		return true;
	}
	inline Vector GetHitboxPosition(int i,void* studio,matrix3x4* matrix)
	{
		if (i < 0)
			return Vector(0);

		// get our hitbox data from the studio model

		int setindex = *(int*)((long)studio + 176);
		void* set = (void*)((long)studio + setindex);
		int boxindex = *(int*)((long)set + 8);
		Hitbox* box = (Hitbox*)((long)set + boxindex);

		// css has the neck hitbox,
		// since all hitboxes are consistent on models we'll do it normally
		box += i;

		Vector min = box->min;
		Vector max = box->max;

		matrix3x4& mtx = matrix[box->bone];

		Vector a = Vector(mtx[0][0],mtx[0][1],mtx[0][2]);
		Vector b = Vector(mtx[1][0],mtx[1][1],mtx[1][2]);
		Vector c = Vector(mtx[2][0],mtx[2][1],mtx[2][2]);

		min = Vector(min.Dot(a) + mtx[0][3],min.Dot(b) + mtx[1][3],min.Dot(c) + mtx[2][3]);
		max = Vector(max.Dot(a) + mtx[0][3],max.Dot(b) + mtx[1][3],max.Dot(c) + mtx[2][3]);

		return (min + max) * 0.5;
	}
	inline Vector GetCenter()
	{
		return GetRenderOrigin() + Vector(0,0,GetMaxs().z * 0.5);
	}
	inline void* GetWeaponData()
	{
		typedef void* (__thiscall* dfn)(void*);
		return ((dfn)Virtual(this,450))(this);
	}
	inline float GetCycleTime()
	{
		return *(float*)((long)GetWeaponData() + 0x788);
	}
	inline void UpdateAccuracyPenalty()
	{
		typedef void (__thiscall* dfn)(void*);
		((dfn)Virtual(this,480))(this);
	}
	inline float GetInaccuracy()
	{
		typedef float (__thiscall* dfn)(void*);
		return ((dfn)Virtual(this,478))(this);
	}
	inline float GetSpread()
	{
		typedef float (__thiscall* dfn)(void*);
		return ((dfn)Virtual(this,479))(this);
	}
};