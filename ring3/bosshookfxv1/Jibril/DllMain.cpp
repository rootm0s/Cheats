#include "Includes.h"

unsigned long Font = NULL;
IMaterial* ShaderMat = NULL;
IMaterial* ShaderMatNoZ = NULL;
IMaterial* TexturedMat = NULL;
IMaterial* TexturedMatNoZ = NULL;
DWORD dwGameResources = NULL;
DWORD dwGlowBase = NULL;
DWORD dwInput = NULL;
BYTE bSendPacket = NULL;
DWORD dwOrigCreateMove = NULL;
ScreenSize_t ScreenSize;

//read the knifechanger in client.cpp for knife values
int iKnife = 4;

//info for materials and "example code" stolen from kiro
IMaterial* CreateMaterial(bool bLit, bool bIgnoreZ)
{
	static int created = 0;

	std::string type = bLit ? "VertexLitGeneric" : "UnlitGeneric";

	char materialBuffer[2048];

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"VGUI/white\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$halflambert\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"0\"\
		\n}\n\0"
	};

	sprintf(materialBuffer, tmp, type.c_str(), (bIgnoreZ) ? 1 : 0);

	char materialName[512];
	sprintf(materialName, "custom_material_%i.vmt", created);
	++created;

	KeyValues* keyValues = new KeyValues(type.c_str());
	keyValues->LoadFromBuffer(materialName, materialBuffer);

	if (keyValues == NULL)
		return 0;

	IMaterial* createdMaterial = Interface.MaterialSystem->CreateMaterial(materialName, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void RecvProxy_Sequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	int iSequence = pData->m_Value.m_Int;

	CEntity* Weapon = (CEntity*)pStruct;

	if (Weapon && Weapon->GetModel())
	{
		std::string strModelName = Interface.ModelInfo->GetModelName(Weapon->GetModel());

		static bool bWasOpen = false;

		if (strModelName.find("v_knife_push.mdl") != std::string::npos)
		{
			if (iSequence == 2)//random stab fix
				iSequence = 1;

			if (iSequence == 12)//fix inspect animation
				iSequence = 15;
		}

		if (strModelName.find("v_knife_falchion_advanced.mdl") != std::string::npos)
		{
			srand(GetTickCount());

			int iBalanceChance = rand() % 100;

			if (iSequence == 12 && iBalanceChance > 70)//balance animation
				iSequence = 13;

			if (iSequence == 2)//random stab fix
				iSequence = 1;
		}

		if (strModelName.find("v_knife_butterfly.mdl") != std::string::npos)
		{
			srand(GetTickCount());

			int iOpenChance = rand() % 100;

			if (iSequence == 0 && iOpenChance >= 70)
				iSequence = 1;

			bWasOpen = ((iSequence == 1) || (iSequence == 0));

			int iBalanceChance = rand() % 100;

			if (iSequence == 12 && iBalanceChance <= 33)//inspect animation
				iSequence = 13;

			if (iSequence == 12 && iBalanceChance <= 66 && iBalanceChance > 33)//inspect animation
				iSequence = 14;

			if (iSequence == 12 && iBalanceChance > 66)//inspect animation
				iSequence = 15;

			if (iSequence == 1 && bWasOpen)//random stab fix
				iSequence = 2;

			if (iSequence == 8 || iSequence == 9)
				iSequence = 10;
		}
		else
		{
			bWasOpen = false;
		}
	}

	*(int*)pOut = iSequence;
}

void RecvProxy_Viewmodel(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	int iModel = pData->m_Value.m_Int;

	int iCTDefault = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iTDefault = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int iBayonet = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGunGame = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");
	int iGut = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interface.ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");

	if (Me())
	{
		CEntity* Weapon = Me()->GetBaseCombatWeapon();

		if (Weapon)
		{
			if (iModel == iBayonet
				|| iModel == iButterfly
				|| iModel == iCTDefault
				|| iModel == iTDefault
				|| iModel == iFlip
				|| iModel == iGunGame
				|| iModel == iGut
				|| iModel == iKarambit
				|| iModel == iM9Bayonet
				|| iModel == iHuntsman
				|| iModel == iFalchion)
			{
				if (iKnife == 1)
				{
					iModel = iBayonet;
				}
				else if (iKnife == 2)
				{
					iModel = iFlip;
				}
				else if (iKnife == 3)
				{
					iModel = iGut;
				}
				else if (iKnife == 4)
				{
					iModel = iKarambit;
				}
				else if (iKnife == 5)
				{
					iModel = iM9Bayonet;
				}
				else if (iKnife == 6)
				{
					iModel = iHuntsman;
				}
				else if (iKnife == 7)
				{
					iModel = iButterfly;
				}
				else if (iKnife == 8)
				{
					iModel = iFalchion;
				}
				else if (iKnife == 9)
				{
					iModel = iDagger;
				}
			}
		}
	}

	*(int*)pOut = iModel;
}

void __stdcall new_RunCommand(CEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
	typedef void(__stdcall* RunCommand_t)(CEntity*, CUserCmd*, IMoveHelper*);
	Interface.PredictionVMT->Function<RunCommand_t>(19)(pPlayer, pCmd, pMoveHelper);

	if (Interface.MoveHelper == NULL && pMoveHelper != NULL)
		Interface.MoveHelper = pMoveHelper;
}

void __declspec(naked) __fastcall hkCreateMove(void* thisptr, void* eax, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		mov     bSendPacket, bl
		push    ebp
		mov     ebp, esp
		sub     esp, 8
		pusha
		push[ebp + 10h]
		push[ebp + 0Ch]
		push[ebp + 08h]
		call    dwOrigCreateMove
		push[ebp + 10h]
		push[ebp + 0Ch]
		push[ebp + 08h]
		call    new_CreateMove
		popa
		mov     bl, bSendPacket
		mov     esp, ebp
		pop     ebp
		retn    0Ch
	}
}

template<typename T> T FindInterface(std::string szModuleName, std::string szInterfaceName)
{
	HMODULE hMod = GetModuleHandleA(szModuleName.c_str());

	if (!hMod)
		return nullptr;

	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(hMod, "CreateInterface");

	if (!CreateInterface)
		return nullptr;

	void* pReturn = nullptr;

	char* szInterface = new char[1024];

	for (int i = 0; i <= 999; ++i)
	{
		sprintf(szInterface, "%s%03i", szInterfaceName.c_str(), i);
		pReturn = CreateInterface(szInterface, NULL);

		if (pReturn)
		{
#ifdef DEBUG
			Utilities::DebugPrint("Found Interface %s at 0x%x.", szInterface, (DWORD)pReturn);
#endif
			break;
		}
	}

	delete[] szInterface;

#ifdef DEBUG
	if (pReturn == nullptr)
		Utilities::DebugPrint("Found no interface for %s.", szInterfaceName.c_str());
#endif

	return reinterpret_cast<T>(pReturn);
}

void Init(void)
{
	for (HMODULE hEngine = NULL; hEngine == NULL; Sleep(100))
		hEngine = GetModuleHandleW(L"engine.dll");

	for (HMODULE hClient = NULL; hClient == NULL; Sleep(100))
		hClient = GetModuleHandleW(L"client.dll");

	Interface.BaseClient = FindInterface<IBaseClient*>("client.dll", "VClient");
	DWORD dwBaseClient = (DWORD)Interface.BaseClient;
	Interface.BaseClientVMT = new CVMTHook(&dwBaseClient);

	DWORD dwInputPointer = Utilities::FindPattern(Interface.BaseClientVMT->Function<DWORD>(21), 0x100, (BYTE*)"\x24\xB9", "xx");

	if (dwInputPointer != NULL)
	{
		dwInputPointer += 0x2;
		Interface.Input = *(CInput**)dwInputPointer;
	}

	Interface.EntityList = FindInterface<IClientEntityList*>("client.dll", "VClientEntityList");
	Interface.Prediction = FindInterface<CPrediction*>("client.dll", "VClientPrediction");
	Interface.GameMovement = FindInterface<CGameMovement*>("client.dll", "GameMovement");
	DWORD dwPrediction = (DWORD)Interface.Prediction;
	Interface.PredictionVMT = new CVMTHook(&dwPrediction);

	Interface.EngineClient = FindInterface<IVEngineClient*>("engine.dll", "VEngineClient");
	Interface.EngineTrace = FindInterface<IEngineTrace*>("engine.dll", "EngineTraceClient");
	Interface.ModelInfo = FindInterface<IVModelInfo*>("engine.dll", "VModelInfoClient");
	Interface.ModelRender = FindInterface<IVModelRender*>("engine.dll", "VEngineModel");
	Interface.RenderView = FindInterface<IVRenderView*>("engine.dll", "VEngineRenderView");
	DWORD dwModelRender = (DWORD)Interface.ModelRender;
	Interface.ModelRenderVMT = new CVMTHook(&dwModelRender);

	Interface.MaterialSystem = FindInterface<IMaterialSystem*>("MaterialSystem.dll", "VMaterialSystem");
	DWORD dwMaterialSystem = (DWORD)Interface.MaterialSystem;
	Interface.MaterialSystemVMT = new CVMTHook(&dwMaterialSystem);

	void* PhysicsSurfaceProps = FindInterface<void*>("vphysics.dll", "VPhysicsSurfaceProps");

	Interface.Surface = FindInterface<ISurface*>("vguimatsurface.dll", "VGUI_Surface");

	Interface.Panel = FindInterface<IPanel*>("vgui2.dll", "VGUI_Panel");
	DWORD dwPanelVMT = (DWORD)Interface.Panel;
	Interface.PanelVMT = new CVMTHook(&dwPanelVMT);

	void* Cvar = FindInterface<void*>("vstdlib.dll", "VEngineCvar");

	Interface.GlobalVars = (CGlobalVarsBase*)(**(DWORD**)(Utilities::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0x7FFFFF, (BYTE*)"\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04", "x????xxxxxx") + 0x1));
#ifdef DEBUG
	Utilities::DebugPrint("Found GlobalVars at 0x%x", (DWORD)Interface.GlobalVars);
#endif

	dwGameResources = *(DWORD*)(Utilities::FindPattern((DWORD)GetModuleHandleA("client.dll") + 0x500000, 0xFFFFF, (BYTE*)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\xD4\x02\x00\x00\x81\xC7\xD0\x09\x00\x00", "xx????xxxxxxxxxxxxxx") + 0x2);
#ifdef DEBUG
	Utilities::DebugPrint("Found GameResources at 0x%x", dwGameResources);
#endif

	dwGlowBase = Utilities::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0xFFFFFF, (BYTE*)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E\x0F\x57\xC0\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xF3\x0F\x7F\x05", "x????xxxxxxxxx????xxxxxxxx");
#ifdef DEBUG
	Utilities::DebugPrint("Found GlowBase at 0x%x", dwGlowBase);
#endif

	Interface.NetVars->Init();
	Interface.NetVars->GetOffsets();
	//Interface.NetVars->HookNetProp("DT_BaseViewModel", "m_nModelIndex", RecvProxy_Viewmodel);
	//Interface.NetVars->HookNetProp("DT_BaseViewModel", "m_nSequence", RecvProxy_Sequence);

	Interface.Settings->ReadSettings();

	Font = Interface.Surface->CreateFont();
	//Interface.Surface->SetFontGlyphSet(Font, "Visitor TT2 BRK", 11, 0, 0, 0, FONTFLAG_OUTLINE);
	Interface.Surface->SetFontGlyphSet(Font, "Lucida Console", 11, 0, 0, 0, FONTFLAG_OUTLINE);

	Interface.EngineClient->GetScreenSize(ScreenSize.Width, ScreenSize.Height);

	ShaderMat = CreateMaterial(false, false);
	ShaderMatNoZ = CreateMaterial(false, true);
	TexturedMat = CreateMaterial(true, false);
	TexturedMatNoZ = CreateMaterial(true, true);

	Interface.PredictionVMT->Hook((DWORD)new_RunCommand, 19);
	Interface.ModelRenderVMT->Hook((DWORD)new_DrawModelExecute, 21);
	dwOrigCreateMove = Interface.BaseClientVMT->Function<DWORD>(21);
	Interface.BaseClientVMT->Hook((DWORD)hkCreateMove, 21);
	//Interface.BaseClientVMT->Hook((DWORD)new_FrameStageNotify, 36);
	Interface.PanelVMT->Hook((DWORD)new_PaintTraverse, 41);

	g_pConsole->AddVar("m_aimbot_active", false);
	g_pConsole->AddVar("m_aimbot_fov", 0);
	g_pConsole->AddVar("m_esp_active", false);
	g_pConsole->AddVar("m_misc_bhop", false);

	ExitThread(1);
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#ifdef DEBUG
		if (AllocConsole())
		{
			freopen("CONOUT$", "wt", stdout);
			SetConsoleTitleA("");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
#endif
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		//TODO: Cleanup
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return 1;
}