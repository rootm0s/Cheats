#include "main.h"

Base::Utils::CVMTHookManager g_pPanelVMT;

void __stdcall hkdPaintTraverse(unsigned vguiPanel, bool forceRepaint, bool allowForce)
{
	static DWORD dwOriginPaintTraverse = g_pPanelVMT.dwGetMethodAddress(41);

	__asm
	{
		PUSH allowForce;
		PUSH forceRepaint;
		PUSH vguiPanel;
		CALL dwOriginPaintTraverse;
	}

	static bool bDidOnce = false;

	if (!bDidOnce)
	{
		g_pConsole->initCommands();
		g_pConsole->readConfig("default.cfg");
		g_Draw.initFont();
		bDidOnce = true;
		Base::Debug::LOG("Font initialized");
	}

	const char* pszPanelName = g_Valve.pPanel->GetName(vguiPanel);
	bool isValidPanel = false;

	if (pszPanelName && pszPanelName[0] == 'M' && pszPanelName[3] == 'S' &&
		pszPanelName[9] == 'T' && pszPanelName[12] == 'P')
		isValidPanel = true;


	if (!isValidPanel)
		return;

	if (g_pConsole->getValue("esp_watermark") == 1)
		g_Draw.DrawStringA(g_Draw.m_DrawFont, true, 250, 20, 255, 255, 255, 255, ":^)");

	g_pConsole->RenderConsole();

	unsigned short HighOrderBit = 1 << 15; // 1 = KeyDown; 0 = KeyUp;
	BOOL bIsBackspaceKeyDown = FALSE;
	GetKeyState(VK_BACK) & HighOrderBit ? bIsBackspaceKeyDown = TRUE : bIsBackspaceKeyDown = FALSE;

	if (g_pConsole->IsActive())
	{
		static int iFadeColor = 0;
		iFadeColor += 2;
		if (iFadeColor >= 220)
			iFadeColor = 0;

		g_Draw.FillRGBA(606 + g_Draw.getWidht(g_Draw.m_DrawFont, g_pConsole->command.c_str()), 575, 4, 20, 255, 255, 255, iFadeColor);

		static DWORD oldTick = GetTickCount();
		if (((DWORD)oldTick + 100) <= GetTickCount() && bIsBackspaceKeyDown)
		{
			oldTick = (DWORD)GetTickCount();
			g_pConsole->HandleKeys(66);
		}
	}

	if (g_Valve.pEngine->IsInGame() || g_Valve.pEngine->IsConnected())
	{
		g_Draw.doESP();
		//g_pMisc->Crosshair( SourceSDK::Color( 163, 73, 164 ) );
	}
}