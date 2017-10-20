#include "dll.h"

MsgFn Msg;

long clientbase,enginebase;

Entity* local;
Entity* weapon;

BaseClient* client;
BaseEngine* engine;
BaseVGUI* vgui;

NetvarManager* netvars;

GameUtil* game;
AimbotManager* aimbot;
PredictionManager* prediction;
ESPManager* esp;
GlowManager* glow;
CvarManager* cvars;
GUIManager* gui;

GlobalVars* globals;

/*
void* fn = GetProcAddress(GetModuleHandleA("engine.dll"),"CreateInterface");

		long jmp = (long)fn + 5;

		long jmploc = *(long*)jmp;

		long fn2 = (long)(jmploc + jmp + 5);

		void* plist = **(void***)((long)fn2 + 5);

		5772ABC0   55               PUSH EBP
5772ABC1   8BEC             MOV EBP,ESP
5772ABC3   56               PUSH ESI
5772ABC4   8B35 8895A257    MOV ESI,DWORD PTR DS:[57A29588]          ; client.579FA584
5772ABCA   57               PUSH EDI
5772ABCB   85F6             TEST ESI,ESI


*/

long HookVirtual(long* vmt,long* fn,int index)
{
	vmt = *(long**)vmt;

	long origin = vmt[index];

	void* adr = &vmt[index];

	long prot;

	VirtualProtect(&vmt[index],4,0x40,(DWORD*)&prot);
	
	vmt[index] = (long)fn;

	VirtualProtect(&vmt[index],4,(DWORD)prot,(DWORD*)&prot);

	return origin;
}

typedef void*(__thiscall* t_paint_traverse)(BaseVGUI* pthis, unsigned int, bool, bool);
t_paint_traverse opaint_traverse;
void __fastcall paint_traverse(BaseVGUI* ppanel, void* pedx, unsigned int ivguipanel, bool bforce_rp, bool ballow_force)
{
	opaint_traverse(ppanel, ivguipanel, bforce_rp, ballow_force);

	if (strstr(ppanel->GetPanelName(ivguipanel), "FocusOverlayPanel"))
	{
		static int call = 0;

		++call;

		if (call % 2)
			return;

		static int menufont = 0, espfont = 0;

		if (!menufont)
			menufont = vgui->CreateFont("System", 11, 800, 0x200);

		if (!espfont)
			espfont = vgui->CreateFont("Verdana", 12, 800, 0x200);

		engine->GetScreenSize(game->scrw, game->scrh);

	//	vgui->SetFont(5);

		//vgui->SetTextColor(255, 255, 255);

		gui->DrawMenu();

	//	vgui->SetFont(espfont);
//
	//	esp->EntThink();

	//	esp->Think();
	}

}

void Init()
{
	Msg = (MsgFn)GetProcAddress(GetModuleHandleA("tier0.dll"),"Warning");

	Msg("%s\n",date);

	clientbase = (long)GetModuleHandleA("client.dll");
	enginebase = (long)GetModuleHandleA("engine.dll");

	client = new BaseClient();
	engine = new BaseEngine();
	vgui = new BaseVGUI();

	netvars = new NetvarManager();

	game = new GameUtil();
	aimbot = new AimbotManager();
	prediction = new PredictionManager();
	esp = new ESPManager();
	glow = new GlowManager();

	cvars = new CvarManager();
	gui = new GUIManager();

//	Msg("%X\n",Virtual(client->move,15));
	//return;

	globals = *(GlobalVars**)(clientbase + 0xA36E80);

	client->CreateMovePtr = (CreateMoveFn)HookVirtual(client->mode,(long*)&CreateMove,24);
	//client->RunCommandPtr = (RunCommandFn)HookVirtual(client->prediction,(long*)&RunCommand,19);
	//HookVirtual(client->prediction,(long*)&InPrediction,14);

	//engine->DrawModelPtr = (DrawModelFn)HookVirtual(engine->render,(long*)&DrawModel,21);
	//engine->SceneEndPtr = (SceneEndFn)HookVirtual(engine->view,(long*)&SceneEnd,9);

	//vgui->PaintTraversePtr = (PaintTraverseFn)HookVirtual(vgui->panel,(long*)&PaintTraverse,41);
	//vgui->KeyEventPtr = (KeyEventFn)HookVirtual(vgui->enginevgui,(long*)&KeyEvent,10);

	fisherman* fpaint_traverse = new fisherman();
	opaint_traverse = (t_paint_traverse)fpaint_traverse->fish((DWORD**)vgui->panel, (DWORD)paint_traverse, 41);

	Msg("well we injected\n");
}

int __stdcall DllMain(void* i, DWORD r, void* res)
{
	if (r == 1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, NULL, 0, NULL);
		return 1;
	}

	return true;
}