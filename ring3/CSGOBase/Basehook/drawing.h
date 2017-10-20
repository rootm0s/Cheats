#pragma once

class CDraw
{
public:
	VOID init(int size);
	INT getWidht(const char* input);
	std::wstring stringToWide(const std::string& text);
	VOID DrawString(int x,int y,int r,int g,int b,const wchar_t *pszText);
	VOID DrawStringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...);
	VOID FillRGBA ( int x, int y, int w, int h, int r, int g, int b, int a );
	VOID DrawHeader ( INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth );
	VOID DrawHealthBox( int x, int y, int r, int g, int b, int a, int CurHealth, int MaxHealth );
	VOID boxESP(int x, int y, int radius, int R, int G, int B);
	VOID drawCrosshair ( int x, int y, int r, int g, int b );
	VOID DrawBox( int x, int y, int w, int h, int lw, int r, int g, int b, int a );		
	VOID DrawOutlinedRect(int x0,int y0,int x1,int y1, int R, int G, int B, int A);	
	void LineRGBA(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
	unsigned long m_Font, font_menu,font_Tab, font_build, font_GUI2, font_GUI, Font_CHECK;
private:
};