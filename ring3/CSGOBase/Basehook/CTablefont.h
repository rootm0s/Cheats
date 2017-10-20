//==============================================================================
// cTableFont.h
//==============================================================================

//==============================================================================
#ifndef CTABLEFONT_H
#define CTABLEFONT_H
//==============================================================================

//==============================================================================
class cTableFont
{
public:
	void cTableFont::DrawString(bool bCenter, int x, int y, int r, int g, int b, int a, const char *pInput, ...);
	int iGetLength(const char *pInput, ...);
	int iGetWidth(const char *pInput, ...);
	int iGetFontSize( );

private:
	void Pixel(int x, int y, int w, int h, int r, int g, int b, int a, bool bBlend=false);
	void DrawChar(const char *pChar);
	bool bParseTableFont(const char *pFileName);
	bool bTable[255][10][15];
};

extern cTableFont gTableFont;
//==============================================================================


//==============================================================================
#endif
//==============================================================================
