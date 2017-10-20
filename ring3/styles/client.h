typedef void (__thiscall* CreateMoveFn)(void*,float,UserCmd*);
typedef void (__thiscall* RunCommandFn)(void*,Entity*,UserCmd*,void*);

class BaseClient
{
public:
	BaseClient()
	{
		list = new InterfaceList();

		list->Init("client.dll");

		base = list->GetInterface("VClient");
		ents = list->GetInterface("VClientEntityList");
		prediction = list->GetInterface("VClientPrediction");
		move = list->GetInterface("GameMovement");

		long hudproc = Virtual(base,10);
		mode = **(long***)(hudproc + 5);
	}

	// base client

	inline Class* GetClientClasses()
	{
		typedef Class* (__thiscall* dfn)(void*);
		return ((dfn)Virtual(base,8))(base);
	}
	
	// entity list

	inline Entity* GetEntity(int i,bool handle = false)
	{
		typedef Entity* (__thiscall* dfn)(void*,int);
		return ((dfn)Virtual(ents,handle ? 4 : 3))(ents,i);
	}
	inline int GetHighestIndex()
	{
		typedef int (__thiscall* dfn)(void*);
		return ((dfn)Virtual(ents,6))(ents);
	}

	// prediction

	inline void SetupMove(void* player,UserCmd* cmd)
	{
		void* data = *(void**)((long)move + 0x8);

		typedef void (__thiscall* dfn)(void*,void*,UserCmd*,int,void*);
		return ((dfn)Virtual(prediction,20))(prediction,player,cmd,0,data);
	}
	inline void FinishMove(void* player,UserCmd* cmd)
	{
		void* data = *(void**)((long)move + 0x8);

		typedef void (__thiscall* dfn)(void*,void*,UserCmd*,void*);
		return ((dfn)Virtual(prediction,21))(prediction,player,cmd,data);
	}

	// movement

	inline void ProcessMovement(void* player)
	{
		void* data = *(void**)((long)move + 0x8);

		typedef void (__thiscall* dfn)(void*,void*,void*);
		((dfn)Virtual(move,1))(move,player,data);
	}
	inline void DecayPunchAngle()
	{
		typedef void (__thiscall* dfn)(void*);
		((dfn)Virtual(move,15))(move);
	}
	inline void FullWalkMove()
	{
		typedef void (__thiscall* dfn)(void*);
		((dfn)Virtual(move,30))(move);
	}

	// movehelper

	inline void SetHost(void* player)
	{
		typedef void (__thiscall* dfn)(void*,void*);
		((dfn)Virtual(movehelper,1))(movehelper,player);
	}

	InterfaceList* list;
	long* base;
	long* ents;
	long* prediction;
	long* move;
	long* mode;
	long* movehelper;

	CreateMoveFn CreateMovePtr;
	RunCommandFn RunCommandPtr;

	inline void CreateMove(float time,UserCmd* cmd) { CreateMovePtr(mode,time,cmd); }
	inline void RunCommand(Entity* player,UserCmd* cmd,void* helper) { RunCommandPtr(prediction,player,cmd,helper); }
};