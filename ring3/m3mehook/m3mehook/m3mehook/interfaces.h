// thanks kujo

#define MODULE_CLIENT		  "client.dll"
#define MODULE_ENGINE		  "engine.dll"
#define MODULE_VPHYSICS		  "vphysics.dll"
#define MODULE_STUDIORENDER	  "StudioRender.dll"
#define MODULE_MATERIALSYSTEM "MaterialSystem.dll"
#define MODULE_VGUIMATSURFACE "vguimatsurface.dll"
#define MODULE_VGUI2		  "vgui2.dll"
#define MODULE_VSTDLIB        "vstdlib.dll"

class CInterfaceManager
{
private:
	void* CaptureInterface(const char*, const char*);
public:
	void CaptureInterfaces();
	IMaterialSystem*					Material = NULL;
	IClientEntityList*					ClientEntList = NULL;
	IVEngineClient*						Engine = NULL;
	IFileSystem*						FileSystem = NULL;
	ICvar*								Cvar = NULL;
	IBaseClientDLL*						Client = NULL;
	IClientDLLSharedAppSystems*			ClientInterfaces = NULL;
	IPrediction*						Prediction = NULL;
	IEngineSound*						Sound = NULL;
	IGameEvent*							GameEventManager = NULL;
	IVModelRender*						ModelRender = NULL;
	IVRenderView*						RenderView = NULL;
	IEngineTrace*						EngineTrace = NULL;
	IEngineVGui*						EngineVGui = NULL;
	IVEfx*								Effects = NULL;
	IVModelInfoClient*					ModelInfo = NULL;
	IVDebugOverlay*						DebugOverlay = NULL;
	IStudioRender*						Studio = NULL;
	IPhysics*							Physics = NULL;
	IPhysicsSurfaceProps*				PhysicsSurfaceProps = NULL;
	vgui::ISurface*						Surface = NULL;
	vgui::IPanel*						VPanel = NULL;
	IUniformRandomStream*				Random = NULL;
	CGlobalVarsBase*					Globals = NULL;
	IClientMode*						ClientMode = NULL;
	CInput*								Input = NULL;
	INetChannelInfo*					Net = NULL;
	IInputSystem*						InputSystem = NULL;

}; CInterfaceManager *Interfaces = new CInterfaceManager;

void* CInterfaceManager::CaptureInterface(const char* moduleName, const char* interfaceName)
{
	void* foundInterface = nullptr;
	char possibleInterfaceName[512];
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(moduleName), "CreateInterface");
	for (int i = 0; i < 100; i++)
	{
		sprintf(possibleInterfaceName, "%s0%i", interfaceName, i);
		foundInterface = (void*)CreateInterface(possibleInterfaceName, NULL);
		if (foundInterface != nullptr)
		{
			break;
		}
		sprintf(possibleInterfaceName, "%s00%i", interfaceName, i);
		foundInterface = (void*)CreateInterface(possibleInterfaceName, NULL);
		if (foundInterface != nullptr)
		{
			break;
		}
	}

	Msg("Found interface %s\n", possibleInterfaceName);
	return foundInterface;
}

void CInterfaceManager::CaptureInterfaces()
{
	Material = (IMaterialSystem*)CaptureInterface(MODULE_MATERIALSYSTEM, "VMaterialSystem");
	Studio = (IStudioRender*)CaptureInterface(MODULE_STUDIORENDER, "VStudioRender");
	Surface = (vgui::ISurface*)CaptureInterface(MODULE_VGUIMATSURFACE, "VGUI_Surface");
	Physics = (IPhysics*)CaptureInterface(MODULE_VPHYSICS, "VPhysics");
	PhysicsSurfaceProps = (IPhysicsSurfaceProps*)CaptureInterface(MODULE_VPHYSICS, "VPhysicsSurfaceProps");
	VPanel = (vgui::IPanel*)CaptureInterface(MODULE_VGUI2, "VGUI_Panel");
	Engine = (IVEngineClient*)CaptureInterface(MODULE_ENGINE, "VEngineClient");
	Random = (IUniformRandomStream*)CaptureInterface(MODULE_ENGINE, "VEngineRandom");
	Sound = (IEngineSound*)CaptureInterface(MODULE_ENGINE, "IEngineSoundClient");
	GameEventManager = (IGameEvent*)CaptureInterface(MODULE_ENGINE, "GAMEEVENTSMANAGER");
	ModelRender = (IVModelRender*)CaptureInterface(MODULE_ENGINE, "VEngineModel");
	RenderView = (IVRenderView*)CaptureInterface(MODULE_ENGINE, "VEngineRenderView");
	EngineTrace = (IEngineTrace*)CaptureInterface(MODULE_ENGINE, "EngineTraceClient");
	EngineVGui = (IEngineVGui*)CaptureInterface(MODULE_ENGINE, "VEngineVGui");
	Effects = (IVEfx*)CaptureInterface(MODULE_ENGINE, "VEngineEffects");
	ModelInfo = (IVModelInfoClient*)CaptureInterface(MODULE_ENGINE, "VModelInfoClient");
	DebugOverlay = (IVDebugOverlay*)CaptureInterface(MODULE_ENGINE, "VDebugOverlay");
	ClientEntList = (IClientEntityList*)CaptureInterface(MODULE_CLIENT, "VClientEntityList");
	ClientInterfaces = (IClientDLLSharedAppSystems*)CaptureInterface(MODULE_CLIENT, "VClientDllSharedAppSystems");
	Prediction = (IPrediction*)CaptureInterface(MODULE_CLIENT, "VClientPrediction");
	Client = (IBaseClientDLL*)CaptureInterface(MODULE_CLIENT, "VClient");
	InputSystem = (IInputSystem*)CaptureInterface(MODULE_ENGINE, "InputSystemVersion");
	Cvar = (ICvar*)CaptureInterface(MODULE_VSTDLIB, "VEngineCvar");
	Input = *(CInput**)((*(DWORD**)Client)[15] + 0x1);
	ClientMode = **(ClientModeShared***)((*(DWORD**)Client)[10] + 0x5);
	Globals = **(CGlobalVarsBase***)((*(DWORD**)Client)[0] + 0x53);
}