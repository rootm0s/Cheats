#pragma once

class CDraw
{
public:
	VOID initFont();
	std::wstring stringToWide(const std::string& text);
	VOID DrawString(unsigned long font, int x, int y, int r, int g, int b, const wchar_t *pszText);
	INT getWidht(unsigned long font, const char* input);
	VOID DrawStringA(unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...);
	VOID FillRGBA(float x, float y, float w, float h, int r, int g, int b, int a);
	VOID RealBorder(int x, int y, int w, int h, int r, int g, int b, int a);
	VOID DrawHeader(INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth);
	VOID DrawHealthBox(int x, int y, int r, int g, int b, int a, int CurHealth, int MaxHealth);
	VOID boxESP(float x, float y, float radius, int R, int G, int B);
	VOID DrawBox(int x, int y, int w, int h, int lw, int r, int g, int b, int a);
	VOID DrawBoxHealth(int x, int y, int w, int h, int lw, int r, int g, int b, int a, int targetId, int damageDealt, int animTimer, int index);
	VOID DrawOutlinedRect(int x0, int y0, int x1, int y1, int R, int G, int B);
	VOID Rect(int x, int y, int w, int h, ValveSDK::Color color);
	VOID RectOutlined(int x, int y, int w, int h, int thickness, ValveSDK::Color color, ValveSDK::Color outlined);
	VOID BorderBox(int x, int y, int w, int h, int thickness, ValveSDK::Color color);
	VOID doESP( void );
	VOID nameESP(CBaseEntity* pEnt);
	VOID DrawPlayer(CBaseEntity* pLocalEntity, CBaseEntity* pBaseEntity);
	VOID DrawHealthBar(CBaseEntity* pLocalEntity, CBaseEntity* pBaseEntity);
	VOID DrawGlow(CBaseEntity* pBaseEntity, int r, int g, int b, int a);
	ValveSDK::ISurface* g_pSurface = NULL;
	unsigned long m_ESPFont, m_DrawFont;
private:
};

extern CDraw g_Draw;