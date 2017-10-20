typedef void (__thiscall* DrawModelFn)(void*,void*,void*,RenderInfo*,void*);
typedef void (__thiscall* SceneEndFn)(void*);

class BaseEngine
{
public:
	BaseEngine()
	{
		list = new InterfaceList();

		list->Init("engine.dll");

		base = list->GetInterface("VEngineClient");
		trace = list->GetInterface("EngineTraceClient");
		render = list->GetInterface("VEngineModel");
		model = list->GetInterface("VModelInfoClient");
		sound = list->GetInterface("IEngineSoundClient");
		view = list->GetInterface("VEngineRenderView");

		list->Init("materialsystem.dll");

		mat = list->GetInterface("VMaterialSystem");

		list->Init("vstdlib.dll");

		cvar = list->GetInterface("VEngineCvar");

		context = 0;
	}

	// base engine

	inline void GetScreenSize(int& w,int& h)
	{
		typedef void (__thiscall* dfn)(void*,int&,int&);
		((dfn)Virtual(base,5))(base,w,h);
	}
	inline bool GetPlayerInfo(int i,PlayerInfo& info)
	{
		typedef bool (__thiscall* dfn)(void*,int,PlayerInfo&);
		return ((dfn)Virtual(base,8))(base,i,info);
	}
	inline int GetLocalPlayer()
	{
		typedef int (__thiscall* dfn)(void*);
		return ((dfn)Virtual(base,12))(base);
	}
	inline void SetViewAngles(Vector& ang)
	{
		typedef void (__thiscall* dfn)(void*,Vector&);
		((dfn)Virtual(base,19))(base,ang);
	}
	inline matrix3x4& GetScreenMatrix()
	{
		typedef matrix3x4& (__thiscall* dfn)(void*);
		return ((dfn)Virtual(base,37))(base);
	}
	inline void ConCommand(char* cmd)
	{
		typedef void (__thiscall* dfn)(void*,char*);
		((dfn)Virtual(base,106))(base,cmd);
	}

	// traces

	int GetPointContents(Vector& pos)
	{
		typedef int (__thiscall* dfn)(void*,Vector&,int);
		return ((dfn)Virtual(trace,0))(trace,pos,0);
	}
	void TraceRay(TraceResult& tr,RayData& ray,TraceFilter& filter)
	{
		typedef void (__thiscall* dfn)(void*,RayData&,int,TraceFilter&,TraceResult&);
		((dfn)Virtual(trace,5))(trace,ray,0x46004003,filter,tr);
	}

	// render

	void MaterialOverride(Material* mat)
	{
		typedef void (__thiscall* dfn)(void*,Material*,int);
		((dfn)Virtual(render,1))(render,mat,0);
	}
	
	// model

	char* GetModelName(void* mdl)
	{
		typedef char* (__thiscall* dfn)(void*,void*);
		return ((dfn)Virtual(model,3))(model,mdl);
	}
	void* GetStudioModel(void* mdl)
	{
		typedef void* (__thiscall* dfn)(void*,void*);
		return ((dfn)Virtual(model,30))(model,mdl);
	}

	// sound

	void EmitSound(char* str)
	{
		typedef void (__thiscall* dfn)(void*,char*,float,int,int,float);
		((dfn)Virtual(sound,11))(sound,str,1,100,0,0);
	}

	// view

	void SetBlend(float a)
	{
		typedef void (__thiscall* dfn)(void*,float);
		((dfn)Virtual(view,4))(view,a);
	}
	void SetColor(float r,float g,float b)
	{
		float color[3];
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = 1;

		typedef void (__thiscall* dfn)(void*,float*);
		((dfn)Virtual(view,6))(view,color);
	}

	// material

	Material* FindMaterial(char* name)
	{
		typedef Material* (__thiscall* dfn)(void*,char*,const char*,bool,void*);
		return ((dfn)Virtual(mat,84))(mat,name,"Model Textures",1,0);
	}

	// cvar

	ConVar* FindVar(char* var)
	{
		typedef ConVar* (__thiscall* dfn)(void*,char*);
		return ((dfn)Virtual(cvar,15))(cvar,var);
	}

	// context

	void SetStencilState(StencilState& sten)
	{
		typedef void (__thiscall* dfn)(void*,StencilState&);
		((dfn)Virtual(context,127))(context,sten);
	}

	void* context;

	InterfaceList* list;
	long* base;
	long* trace;
	long* render;
	long* model;
	long* sound;
	long* view;
	long* mat;
	long* cvar;

	DrawModelFn DrawModelPtr;
	SceneEndFn SceneEndPtr;

	inline void DrawModel(void* context,void* state,RenderInfo* info,void* mtx) { DrawModelPtr(render,context,state,info,mtx); }
	inline void SceneEnd() { SceneEndPtr(view); }
};