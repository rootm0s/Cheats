#define INDEX_PLAYERGLOW 0x1DCC  //1DB8
#define INDEX_DYNAMICPROPGLOW 0xD68
#define INDEX_WEAPONGLOW 0x1648
#define INDEX_C4GLOW 0xD7C
#define INDEX_GRENADE 0xD64
#define M_PI	3.14159265358979323846f

#include "main.h"

CDraw g_Draw;

VOID CDraw::initFont()
{
	m_DrawFont = g_Valve.pSurface->CreateFontA();
	g_Valve.pSurface->SetFontGlyphSet(m_DrawFont,"Visitor TT2 -BRK-", 18, FW_DONTCARE, 0, 0, 0x200);
}

VOID CDraw::DrawString(unsigned long font, int x, int y, int r, int g, int b, const wchar_t *pszText)
{
	if (pszText == NULL)
		return;

	g_Valve.pSurface->DrawSetTextPos(x, y);
	g_Valve.pSurface->DrawSetTextFont(font);
	g_Valve.pSurface->DrawSetTextColor(r, g, b, 255);
	g_Valve.pSurface->DrawPrintText(pszText, wcslen(pszText));
}

std::wstring CDraw::stringToWide(const std::string& text)
{
	std::wstring wide(text.length(), L' ');
	std::copy(text.begin(), text.end(), wide.begin());

	return wide;
}

int CDraw::getWidht(unsigned long font, const char* input)
{	
	INT iWide = 0;
	INT iTall = 0;
	INT iBufSize = MultiByteToWideChar(CP_UTF8, 0x0, input, -1, NULL, 0);

	wchar_t* pszUnicode = new wchar_t[iBufSize];

	MultiByteToWideChar(CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize);

	g_Valve.pSurface->GetTextSize(m_DrawFont, pszUnicode, iWide, iTall);
	
	delete[] pszUnicode;

	return iWide;
}

VOID CDraw::DrawBox(int x, int y, int w, int h, int lw, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, lw, r, g, b, a); // top
	FillRGBA(x, y + lw, lw, h - lw, r, g, b, a); // left
	FillRGBA(x + w - lw, y + lw, lw, h - lw, r, g, b, a); // right
	FillRGBA(x + lw, y + h - lw, w - lw * 2, lw, r, g, b, a); // bottom
}

VOID CDraw::DrawStringA(unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...)
{	
	CHAR szBuffer[MAX_PATH];

	if (!input)
		return;

	vsprintf(szBuffer, input, (char*)&input + _INTSIZEOF(input));

	if (center)
		x -= getWidht(font, szBuffer) / 2;

	y += 4;

	g_Valve.pSurface->DrawSetTextColor(r, g, b, a);
	g_Valve.pSurface->DrawSetTextFont(font);
	g_Valve.pSurface->DrawSetTextPos(x, y);
	std::wstring wide = stringToWide(std::string(szBuffer));
	g_Valve.pSurface->DrawPrintText(wide.c_str(), wide.length());
}

void CDraw::FillRGBA(float x, float y, float w, float h, int r, int g, int b, int a)
{
	g_Valve.pSurface->DrawSetColor(r, g, b, a);
	g_Valve.pSurface->DrawFilledRect(x, y, x + w, y + h);
}

VOID CDraw::Rect(int x, int y, int w, int h, ValveSDK::Color color)
{
	g_Valve.pSurface->DrawSetColor1(color);
	Base::Debug::LOG("16");
	g_Valve.pSurface->DrawFilledRect(x, y, x + w, y + h);
	Base::Debug::LOG("17");
}

VOID CDraw::RectOutlined(int x, int y, int w, int h, int thickness, ValveSDK::Color color, ValveSDK::Color outlined)
{
	Rect(x, y, w, h, color);
	Base::Debug::LOG("10");
	BorderBox(x - thickness, y - thickness, w + thickness, h + thickness, thickness, outlined);
	Base::Debug::LOG("11");
}

VOID CDraw::BorderBox(int x, int y, int w, int h, int thickness, ValveSDK::Color color)
{
	Rect(x, y, w, thickness, color);
	Base::Debug::LOG("12");
	Rect(x, y, thickness, h, color);
	Base::Debug::LOG("13");
	Rect(x + w, y, thickness, h, color);
	Base::Debug::LOG("14");
	Rect(x, y + h, w + thickness, thickness, color);
	Base::Debug::LOG("15");
}

VOID CDraw::DrawPlayer(CBaseEntity* pLocalEntity, CBaseEntity* pBaseEntity)
{
	Vector vecFootScreen, vFoot;
	if (!g_Valve.WorldToScreen(pBaseEntity->GetAbsOrigin(), vecFootScreen))
		return;

	Vector vecHead = pBaseEntity->GetAbsOrigin() + Vector(0.f, 0.f, 40.f);
	if (pBaseEntity->GetFlags() & FL_DUCKING)
		vecHead = pBaseEntity->GetAbsOrigin() + Vector(0.f, 0.f, 20.f);

	Vector vecHeadScreen;
	if (!g_Valve.WorldToScreen(vecHead, vecHeadScreen))
		return;

	
	float h = vecFootScreen.y - vecHeadScreen.y;
	float w = (h / 4.5f) * 2.f;
	float x = vecHeadScreen.x - w / 2;
	float y = vecHeadScreen.y;
	
	if (g_pConsole->getValue("esp_box") == 1 || g_pConsole->getValue("esp_glow") == 1)
	{

		int r = 0, g = 0, b = 0;
		
		if (g_Valve.g_pEngineTraceClient->IsVisible(pLocalEntity, pLocalEntity->GetEyePosition(), pBaseEntity->GetEyePosition(), pBaseEntity))
		{
			r = 255;
			g = 255;
			b = 255;		
		}
		else
		{
			r = 255;
			g = 0;
			b = 0;
		}
			
		
		if (g_pConsole->getValue("esp_box") == 1)
			boxESP(x, y, 6, r, g, b);

		if (g_pConsole->getValue("esp_glow") == 1)
			DrawGlow(pBaseEntity, r, g, b,180);
		
		int iHealth = pBaseEntity->GetHealth();

		
		int hr = 0, hg = 0, hb = 0;

		if (iHealth >= 60)
			hr = 0, hg = 255, hb = 0;
		if (iHealth > 25 && iHealth < 59)
			hr = 255, hg = 165, hb = 0;
		if (iHealth < 25)
			hr = 255, hg = 0, hb = 0;

		g_Draw.FillRGBA(x, y, 50, 10, 105, 105, 105, 255);
		g_Draw.FillRGBA(x, y, iHealth / 2, 10, hr, hg, hb, 255);
		
		
		/*if (g_Valve.g_pEngineTraceClient->IsVisible(pLocalEntity, pLocalEntity->GetEyePosition(), pBaseEntity->GetEyePosition(), pBaseEntity))
		{
			if (g_pConsole->getValue("esp_box") == 1)
				boxESP(x, y, 6, 255, 255, 0);
			if (g_pConsole->getValue("esp_glow") == 1)
				DrawGlow(pBaseEntity, 255, 255, 0, 180);
		}
		else 
		{
			if (g_pConsole->getValue("esp_box") == 1)
				boxESP(x, y, 6, 255, 0, 0);
			if (g_pConsole->getValue("esp_glow") == 1)
				DrawGlow(pBaseEntity, 255, 0, 0, 180);
		}*/
	}
}

void CDraw::boxESP(float x, float y, float radius, int R, int G, int B)
{
	float iStep = radius; //*2.5
	int rad = radius / 2;
	//if (iStep >= 3)
		//iStep = 3;

	for (int i = 0; i < rad; i++){

		//Background
		FillRGBA((x - iStep + i) - 1, (y - iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x - iStep) - 1, (y - iStep + i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x + iStep - i) - 1, (y - iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x + iStep) - 1, (y - iStep + i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x - iStep + i) - 1, (y + iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x - iStep) - 1, (y + iStep - i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x + iStep - i) - 1, (y + iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x + iStep) - 1, (y + iStep - i) - 1, 3, 3, 0, 0, 0, 255);
	}
	for (int i = 0; i < rad; i++){

		//Simple
		FillRGBA(x - iStep + i, y - iStep, 1.0f, 1.0f, R, G, B, 255);
		FillRGBA(x - iStep, y - iStep + i, 1.0f, 1.0f, R, G, B, 255);

		FillRGBA(x + iStep - i, y - iStep, 1.0f, 1.0f, R, G, B, 255);
		FillRGBA(x + iStep, y - iStep + i, 1.0f, 1.0f, R, G, B, 255);

		FillRGBA(x - iStep + i, y + iStep, 1.0f, 1.0f, R, G, B, 255);
		FillRGBA(x - iStep, y + iStep - i, 1.0f, 1.0f, R, G, B, 255);

		FillRGBA(x + iStep - i, y + iStep, 1.0f, 1.0f, R, G, B, 255);
		FillRGBA(x + iStep, y + iStep - i, 1.0f, 1.0f, R, G, B, 255);

	}
}

VOID CDraw::DrawHealthBar(CBaseEntity* pLocalEntity, CBaseEntity* pBaseEntity)
{
	Vector vecFootScreen;
	if (!g_Valve.WorldToScreen(pBaseEntity->GetVecOrigin(), vecFootScreen))
		return;
	Base::Debug::LOG("1");
	Vector vecHead = pBaseEntity->GetVecOrigin() + Vector(0.f, 0.f, 72.f);
	if (pBaseEntity->GetFlags() & FL_DUCKING)
		vecHead = pBaseEntity->GetVecOrigin() + Vector(0.f, 0.f, 52.f);
	Base::Debug::LOG("2");
	Vector vecHeadScreen;
	if (!g_Valve.WorldToScreen(vecHead, vecHeadScreen))
		return;
	Base::Debug::LOG("3");
	ValveSDK::Color color = pBaseEntity->GetHealthColor();
	Base::Debug::LOG("4");
	float h = vecFootScreen.y - vecHeadScreen.y;
	float w = (h / 4.5f) * 2.f;
	float x = vecHeadScreen.x - w / 2;
	float y = vecHeadScreen.y;
	Base::Debug::LOG("5");

	RectOutlined(x, y + h + 3, w / 100 * pBaseEntity->GetHealth(), 2.f, 1, pBaseEntity->GetHealthColor(), ValveSDK::Color(0, 0, 0, 255));
}

VOID CDraw::doESP( void )
{
	if (!g_Valve.pEngine->IsConnected() || !g_Valve.pEngine->IsInGame())
		return;

	CBaseEntity* pLocalEntity = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());
	if( !pLocalEntity )
		return;

	for (int i = 1; i < g_Valve.pEngine->GetMaxClients(); i++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(i);

		if( !pBaseEntity )
			continue;

		if( !pBaseEntity->isValidPlayer() )
			continue;

		if( pBaseEntity->GetIndex() == pLocalEntity->GetIndex() )
			continue;
		
		Vector vecScreen;
		Vector vecOrigin;
		vecOrigin = pBaseEntity->GetAbsOrigin() + Vector(0.f, 0.f, 72.f);
		if (pBaseEntity->GetFlags() & FL_DUCKING)
			vecOrigin = pBaseEntity->GetAbsOrigin() + Vector(0.f, 0.f, 52.f);

		if (g_Valve.WorldToScreen(vecOrigin, vecScreen))
		{
			if ( pBaseEntity->GetTeam() != pLocalEntity->GetTeam() )
			{
				DrawPlayer(pLocalEntity, pBaseEntity);
			}
			else
			{
				//DrawPlayer(pLocalEntity, pBaseEntity);
				//DrawGlow( pBaseEntity, INDEX_PLAYERGLOW, Vector( 0.47f, 0.588f, 1.f ) );
			}
		}
	}
}

VOID CDraw::DrawGlow(CBaseEntity* pBaseEntity, int r, int g, int b, int a)
{
	UINT nGlowIndex = *(UINT32*)((DWORD)pBaseEntity + INDEX_PLAYERGLOW);

	ValveSDK::CGlowObjectManager::GlowObjectDefinition_t* pGlowObjectDefinitions = *(ValveSDK::CGlowObjectManager::GlowObjectDefinition_t**)g_pGlowObjectManager;
	if (IsNullPtr(pGlowObjectDefinitions))
		return;

	ValveSDK::CGlowObjectManager::GlowObjectDefinition_t* pCurrentGlowObject = &pGlowObjectDefinitions[nGlowIndex];
	if (IsNullPtr(pCurrentGlowObject))
		return;

	static float flDiff = 1 / 255.f;

	pCurrentGlowObject->m_bShouldGlow = true;
	pCurrentGlowObject->m_bUnknown = false;
	pCurrentGlowObject->m_bFullBloomRender = false;
	pCurrentGlowObject->m_vecGlowColor = Vector(flDiff * r, flDiff * g, flDiff * b);
	pCurrentGlowObject->m_flGlowAlpha = float(flDiff * a);
}