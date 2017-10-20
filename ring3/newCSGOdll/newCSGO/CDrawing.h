#pragma once

class CDrawing
{
public:
	void TextW(bool cent, int font, int x, int y, Color color, wchar_t *pszString);
	void DrawString(int FONT, bool bCenter, int x, int y, Color c, const char *fmt, ...);
	void OutlinedRect(int x, int y, int w, int h, Color& color);
	void DrawHealthBar(int x, int y, float health, int w, int h, Color Draw);
	void FillRGBA( int x, int y, int w, int h, Color colour );
};

extern CDrawing cDraw;