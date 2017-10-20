#include "stdafx.h"
bool bInit = false;
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
CreateInterfaceFn CaptureFactory(char *pszFactoryModule)
{
	CreateInterfaceFn fn = NULL;
	while ( fn == NULL)
	{
		HMODULE hFactoryModule = GetModuleHandleA(pszFactoryModule);
		if (hFactoryModule)
		{
			fn = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hFactoryModule, "CreateInterface"));
		}
		Sleep(10);
	}
	add_log("%s(CreateInterface) = %x",pszFactoryModule,fn);
	return fn;
}

void *CaptureInterface(CreateInterfaceFn fn, char *pszInterfaceName)
{
	unsigned long *ptr = NULL;
	add_log("starting %s(CaptureInterface) = %x",pszInterfaceName,ptr);
	while ( ptr == NULL)
	{
		ptr = reinterpret_cast<unsigned long*>(fn(pszInterfaceName, NULL));
		Sleep(10);
	}
	add_log("ending %s(CaptureInterface) = %x",pszInterfaceName,ptr);
	return ptr;
}
static bool bOnce = false;

void iWannaCheat( )
{
	IClientEntity* pLocalEntity = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
	if(!pLocalEntity)
		return;

	player_info_t pi;
	static int iLocked = -1;
	if ( cEng.IsAlive( pLocalEntity ) )
	{
		vEyePos = cEng.GetEyePosition( pLocalEntity );

		for( INT i = 0; i <= pClientEntList->GetHighestEntityIndex( ); i++ )
		{
			IClientEntity* pEntity = pClientEntList->GetClientEntity( i );

			if ( pEntity == NULL )
				continue;

			if ( pEntity->IsDormant( ) )
				continue;

			if ( !cEng.IsAlive( pEntity ) )
				continue;

			if ( pEntity->Index() == pLocalEntity->Index() )
				continue;

			cEsp.DrawPlayers(i, pLocalEntity, pEntity );
		}
	}
}
typedef void ( __stdcall *PaintTraverse_ )( unsigned int, bool, bool );
PaintTraverse_ oPaintTraverse;

/*---------------------------------HOOK ONE----------------------------------------------*/
VOID WINAPI PaintTraverse_Hooked1(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse( vguiPanel, forceRepaint, allowForce );

	if (!bOnce)
	{
		espfont = pSurface->FontCreate( );
		pSurface->SetFontGlyphSet( espfont, "Arial",12, 400, 0, 0, FONTFLAG_OUTLINE );

		bOnce = TRUE;
	}

	PCHAR szPanelName = (PCHAR)pPanel->GetName(vguiPanel);
	if (strstr(szPanelName, "MatSystemTopPanel"))
	{
		cDraw.DrawString(espfont, 0, 100, 100, Color( 0, 255, 0, 255 ), "PaintTraverse" );

		if ( pEngine->IsInGame( ) && pEngine->IsConnected( ) )
		{
			iWannaCheat( );
		}
	}
}
/*----------------------------------------------------------------------------------------*/

/*---------------------------------HOOK TWO----------------------------------------------*/
DWORD g_dwOrgPaintTraverse;
static unsigned int vguiMatSystemTopPanel = 0;

VOID WINAPI PaintTraverse_Hooked2(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{

//	if (vguiMatSystemTopPanel == vguiPanel) //if the panel is set
//	{
//			pSurface->DrawOutlinedRect(100,100,300,300);
//	}
	_asm
	{
		PUSH allowForce
		PUSH forceRepaint
		PUSH vguiPanel
		MOV ECX, pPanel
		CALL g_dwOrgPaintTraverse
	}

//	if (vguiMatSystemTopPanel == NULL) //make sure the panel isnt set yet
//	{
//			const char* szName = pPanel->GetName(vguiPanel); // Get the name of the panel
//			if (_stricmp(szName, "MatSystemTopPanel") == 0) // Actually get the right panel .....
//			{
//				vguiMatSystemTopPanel = vguiPanel; //set the panel
//			}
//	}


	if (!bOnce)
	{
		espfont = pSurface->FontCreate( );
		pSurface->SetFontGlyphSet( espfont, "Arial",12, 400, 0, 0, FONTFLAG_OUTLINE );

		bOnce = TRUE;
	}

	PCHAR szPanelName = (PCHAR)pPanel->GetName(vguiPanel);
	if (strstr(szPanelName, "MatSystemTopPanel"))
	{
		cDraw.DrawString(espfont, 0, 100, 100, Color( 0, 255, 0, 255 ), "PaintTraverse" );
		//cDraw.OutlinedRect(100, 100, 100, 100, Color(255,0,0,255));

		pSurface->DrawSetColor(Color(255,0,0,255));
		pSurface->DrawOutlinedRect(100,500,300,600);
		pSurface->DrawLine(0,0,200,200);

		if ( pEngine->IsInGame( ) && pEngine->IsConnected( ) )
		{
			iWannaCheat();
		}
	}
}
/*----------------------------------------------------------------------------------------*/
void test1()
{
	const D3DXMATRIX& worldToScreen = pEngine->WorldToScreenMatrix();
	const D3DXMATRIX& worldToScreen2 = cfn.WorldToScreenMatrix();
}
MEMORY_BASIC_INFORMATION mbi;
void InitInterfaces( )
{
	CreateInterfaceFn fnClient			= CaptureFactory("client.dll");
	CreateInterfaceFn fnEngine			= CaptureFactory("engine.dll");
	CreateInterfaceFn fnVguiMatSurface	= CaptureFactory("vguimatsurface.dll");
	CreateInterfaceFn fnVgui2			= CaptureFactory("vgui2.dll");
	CreateInterfaceFn fnMatSystem		= CaptureFactory("materialsystem.dll");

	//pClient         = reinterpret_cast<IBaseClientDLL*>(CaptureInterface(fnClient, "VClient016"));
	pClient         = reinterpret_cast<IBaseClientDLL*>(CaptureInterface(fnClient, "VClient017"));
	pEngine         = reinterpret_cast<IVEngineClient*>(CaptureInterface(fnEngine, "VEngineClient013"));
	pSurface		= reinterpret_cast<ISurface*>(CaptureInterface(fnVguiMatSurface, "VGUI_Surface031"));
	pClientEntList	= reinterpret_cast<IClientEntityList*>(CaptureInterface(fnClient, "VClientEntityList003"));
	pPanel			= (IPanel*)fnVgui2("VGUI_Panel009", NULL);
	pEngineTrace	= reinterpret_cast<IEngineTrace*>(CaptureInterface(fnEngine, "EngineTraceClient004"));
	PDWORD pdwClientVMT = *reinterpret_cast< PDWORD_PTR* >( pClient );
    DWORD dwInitAddr = ( DWORD )( pdwClientVMT[ 0 ] );
    for ( DWORD dwIter = 0; dwIter <= 0xFF; dwIter++ )
	{
		if ( *( PBYTE )( dwInitAddr + dwIter - 1 ) == 0x51 && *( PBYTE )( dwInitAddr + dwIter ) == 0xA3 )
		{
			pGlobals = ( CGlobalVarsBase* )*( PDWORD )*( PDWORD )( dwInitAddr + dwIter + 1 );
			break;
		}
	}

	add_log("pClient = %x",pClient);
	add_log("pEngine = %x",pEngine);

//	LPVOID dw1=(LPVOID)(pEngine->WorldToScreenMatrix);
    PDWORD* VTablePointer = ( PDWORD* )pEngine;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[ 37 ];

	add_log("pEngine->WorldToScreenMatrix = %x",dwAddress);
	add_log("test1 =%x",test1);
	cfn_init();

	//HOOK ONE Can be done VAC Undetected with right modifications into actual hook and hooking below, figure yourself

	//pPanelHook = new CVMTHookManager( ( PDWORD* ) pPanel );
	//oPaintTraverse = ( PaintTraverse_ )pPanelHook->dwHookMethod( ( DWORD )PaintTraverse_Hooked1, 41 );

	//HOOKING TWO 100% sure VAC Detected	
	PDWORD pdwPanelVMT = *reinterpret_cast< PDWORD_PTR* >( pPanel );
	if(VirtualQuery((LPVOID)&pdwPanelVMT[41], &mbi, sizeof(mbi)))
	{
		VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);
		g_dwOrgPaintTraverse = pdwPanelVMT[41];
		pdwPanelVMT[41] = (DWORD)&PaintTraverse_Hooked2;
		VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, mbi.Protect, 0);
		FlushInstructionCache(GetCurrentProcess(), (LPVOID)&pdwPanelVMT[41], sizeof(DWORD));
		SecureZeroMemory(&mbi, sizeof(mbi));
	}
}
DWORD WINAPI HackThread(PVOID pThreadParameter)
{
	HMODULE hModEngine = NULL;
	HMODULE hModClient = NULL;
	do 
	{
		hModEngine = GetModuleHandleA("engine.dll");
		hModClient = GetModuleHandleA("client.dll");
		Sleep(1000);
	}
	while ( !hModEngine || !hModClient );

	add_log("hModEngine = %x",hModEngine);
	add_log("hModClient = %x",hModClient);

	InitInterfaces( );

	ExitThread(0);
	
	return 0;
}
char logfile[MAX_PATH];

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	int len=0;
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			GetModuleFileNameA(hinstDLL,logfile,255);
			len=strlen(logfile);
			logfile[len-3]='l';logfile[len-2]='o';logfile[len-1]='g';
			DeleteFileA(logfile);

			CreateThread(NULL, 0, ( LPTHREAD_START_ROUTINE )HackThread, hinstDLL, 0, NULL);
		break;
	}
	return true;
}
void add_log(char* format, ...)
{
	HANDLE filehandle;
	DWORD dwReadBytes;
	char buffer[2048];
	char writebuffer[2048];
	va_list args;
	va_start(args, format);
	vsprintf (buffer, format, args);
	filehandle = CreateFileA(logfile, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
	SetFilePointer(filehandle, 0, 0, FILE_END);
	char date[18];
	_strdate(date);
	date[8] = ' ';
	_strtime(date+9);
	sprintf(writebuffer,  "[%s]: %s\r\n", date, buffer);
	WriteFile(filehandle, writebuffer, strlen(writebuffer), &dwReadBytes, 0);
	CloseHandle(filehandle);
}
//=========== END ===