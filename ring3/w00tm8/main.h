#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <string>
#include <conio.h>
#include <vector>
#include <Psapi.h>
#include "sdk.h"
#include "CVARS.h"
using namespace std;


IPanel * pPanel;
ISurface * pSurface;
IBaseClientDll** ClientMode;
IVEngineClient * pEngine;
IClientEntityList * pClientEntityList;
IClientEntityList* pEntList;
IBaseClientDll * pClient;
IEngineTrace* pEngineTrace;
CGlobalVarsBase* pGlobals;
CInput* pInput;
IVModelInfo* pModelInfo;
IVModelRender* pModelRender;
IVRenderView* pRenderView;
IMaterialSystem* pMatSystem;
ICvar* pCvar;

CHook * PanelHook;
CHook * CreateMoveHook;
CHook * ClientHooked;
CHook * pModelHook;


typedef bool(__thiscall *CreateMove)(void*, float, CUserCmd*);

CreateMove _CreateMove;
tPaintTraverse pPaintTraverse;
FrameStageNotify_t oFrameStage;
tDrawModelExecute oDrawModelExecute;

POINT Cursor;

/* FONTS */
unsigned long pFont;

Vector aimbotAngle;

int ScreenW, ScreenH, selectedAimbotBone;

int AimbotFov;

#define TAB_AIMBOT 0
#define TAB_VISUALS 1
#define TAB_MISC 3
#define M_PI	3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( M_PI ) / 180.0f ) )

//Color themeColor = Color(63, 152, 137, 255);
Color themeColor = Color(153, 153, 0, 255);
//Color evenDarker = Color(49, 114, 101, 255);
Color evenDarker = Color(153 - 77, 153 - 77, 0, 255);

Color esp_t = Color(255, 0, 0, 255);
Color esp_ct = Color(0, 150, 255, 255);

Vector CalcAngle(Vector& src, Vector& dst, Vector& angles) // Bytte ut din kinda kopia av CalcAngle till en som är tusen gånger bättre.
{

	Vector delta;
	delta.x = (src.x - dst.x);
	delta.y = (src.y - dst.y);
	delta.z = (src.z - dst.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);


	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

Vector ClampAngles(Vector& AngleToNormalize)
{
	Vector vec = AngleToNormalize;
	if (vec.x > 89.0f && vec.x <= 180.0f)
	{
		vec.x = 89.0f;
	}
	while (vec.x > 180.f)
	{
		vec.x -= 360.f;
	}
	while (vec.x < -89.0f)
	{
		vec.x = -89.0f;
	}
	while (vec.y > 180.f)
	{
		vec.y -= 360.f;
	}
	while (vec.y < -180.f)
	{
		vec.y += 360.f;
	}

	vec.z = 0;
	return vec;
}


void VectorAngles3D(const Vector&vecForward, Vector&vecAngles)
{
	Vector vecView;
	if (vecForward.y == 0.f && vecForward.x == 0.f)
	{
		vecView.x = 0.f;
		vecView.y = 0.f;
	}
	else
	{
		vecView.y = atan2(vecForward.y, vecForward.x) * 180.f / M_PI;

		if (vecView.y < 0.f)
			vecView.y += 360;

		vecView.z = sqrt(vecForward.x * vecForward.x + vecForward.y * vecForward.y);

		vecView.x = atan2(vecForward.z, vecView.z) * 180.f / M_PI;
	}

	vecAngles.x = -vecView.x;
	vecAngles.y = vecView.y;
	vecAngles.z = 0.f;
}

int CalcPos(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	int iRealXStart = ((1920 / 2) - (1920 / 2)) + iXStart;
	return Cursor.x - iRealXStart;
}

int width = 1920, height = 1080;
bool WorldToScreen(Vector& vOrigin, Vector& vScreen) {
	const matrix3x4& W2S = pEngine->GetWorldToScreenMatrix();
	float w = W2S[3][0] * vOrigin.x + W2S[3][1] * vOrigin.y + W2S[3][2] * vOrigin.z + W2S[3][3];

	if (w > 0.001) {
		float fl1DBw = 1 / w;
		vScreen.x = (width / 2) + (0.5 * ((W2S[0][0] * vOrigin.x + W2S[0][1] * vOrigin.y + W2S[0][2] * vOrigin.z + W2S[0][3]) * fl1DBw) * width + 0.5);
		vScreen.y = (height / 2) - (0.5 * ((W2S[1][0] * vOrigin.x + W2S[1][1] * vOrigin.y + W2S[1][2] * vOrigin.z + W2S[1][3]) * fl1DBw) * height + 0.5);
		vScreen.z = 0;
		return true;
	}
	return false;
}


bool CursorInArea(int x, int y, int w, int h)
{
	if (Cursor.x > x)
	{
	}
	else
		return false;

	if (Cursor.x <  x + w)
	{
	}
	else
		return false;

	if (Cursor.y > y)
	{
	}
	else
		return false;

	if (y + h > Cursor.y)
	{
	}
	else
		return false;
	return true;
}

void DrawString(int x, int y, Color clr, unsigned long font, const char *pszText)
{
	if (pszText == NULL)
		return;
	wchar_t szString[1024] = { '\0' };
	wsprintfW(szString, L"%S", pszText);
	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawSetTextFont(font);
	pSurface->DrawSetTextColor(clr);
	pSurface->DrawPrintText(szString, wcslen(szString));
}

void TextW(bool cent, int font, int x, int y, Color color, wchar_t *pszString)
{
	if (cent)
	{
		int wide, tall;
		pSurface->GetTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	pSurface->DrawSetTextColor(color);
	pSurface->DrawSetTextFont(font);
	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}

void DrawStringMiddle(unsigned long FONT, bool bCenter, int x, int y, Color c, const char *fmt)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	TextW(bCenter, FONT, x, y, c, pszStringWide);

	free(pszStringWide);
}

void DrawLine(int x, int y, int xx, int yy, Color col) {
	pSurface->DrawSetColor(col);
	pSurface->DrawLine(x, y, xx, yy);
}

void DrawBox(int x, int y, int w, int h, Color col) {
	pSurface->DrawSetColor(col);
	pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawOutlinedBox(int x, int y, int w, int h, Color col, Color outlineColor) {
	pSurface->DrawSetColor(outlineColor);
	pSurface->DrawOutlinedRect(x - 1, y - 1, x + w + 1, y + h + 1);
	pSurface->DrawSetColor(col);
	pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawOutLineLine(int x, int y, int xx, int yy, Color col)
{
	DrawLine(x, y, xx, yy, col);
	DrawLine(x, y - 1, x, y - 1, col);
	DrawLine(x, y + 1, x, y + 1, col);
}

void FillRGB(int x, int y, int w, int h, Color col) {
	pSurface->DrawSetColor(col);
	pSurface->DrawFilledRect(x, y, x + w, y + h);
}

void FilledRGBOutlined(int x, int y, int w, int h, Color color)
{
	FillRGB(x, y, w, h, color);
	DrawBox(x - 1, y - 1, w + 1, h + 1, Color(0, 0, 0, 255));
}

void CustomOutlineFill(int x, int y, int w, int h, Color color, Color outline)
{
	FillRGB(x, y, w, h, color);
	DrawBox(x - 1, y - 1, w + 1, h + 1, outline);
}

void CornerBox(int x, int y, int w, int h, Color Draw)
{
	int bWidth = w;
	int bHeight = h;
	DrawLine(x, y, x, y + (bHeight / 5), Draw);
	DrawLine(x, y, x + (bWidth / 5), y, Draw);
	DrawLine(x + bWidth, y, x + bWidth - (bWidth / 5), y, Draw);
	DrawLine(x + bWidth, y, x + bWidth, y + (bHeight / 5), Draw);
	DrawLine(x, y + bHeight, x + (bWidth / 5), y + bHeight, Draw);
	DrawLine(x, y + bHeight, x, y + bHeight - (bHeight / 5), Draw);
	DrawLine(x + bWidth, y + bHeight, x + bWidth - (bWidth / 5), y + bHeight, Draw);
	DrawLine(x + bWidth, y + bHeight, x + bWidth, y + bHeight - (bHeight / 5), Draw);
}

void EdgedOutline(int x, int y, int w, int h, Color Draw) {
	CornerBox(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 255));
	CornerBox(x, y, w, h, Draw);
}

void DrawHeader(INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth)
{

	if (HealthBarWidth > 100)
		HealthBarWidth = 100;

	INT i = 0;
	INT xCoord = x;
	INT yCoord = y;

	UINT hw = (((w)* HealthBarWidth) / 100);
	for (i = 0; i < 5; i++)
	{
		FillRGB(x - i, y + i, w, 1, Color(0, 0, 0, 255));
		FillRGB(x - i, y + i, hw + 1, 1, Color(r, g, b, 255));

		FillRGB(x - 3, y + 3, hw + 1, 1, Color((INT)(r / 1.5f),
			(INT)(g / 1.5f), (INT)(b / 1.5f), 255));

		FillRGB(x - 4, y + 4, hw + 1, 1, Color((INT)(r / 1.5f),
			(INT)(g / 1.5f), (INT)(b / 1.5f), 255));
	}

	FillRGB(x, y, w, 1, Color(255, 255, 255, 255));

	FillRGB((x + 1) - 5, y + 5, w, 1, Color(255, 255, 255, 255));

	for (i = 0; i < 5; i++)
	{
		FillRGB(x, y, 1, 1, Color(255, 255, 255, 255));
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for (i = 0; i < 5; i++)
	{
		if (i != 0)
			FillRGB(x + w, y, 1, 1, Color(255, 255, 255, 255));

		x--;
		y++;
	}


}


#include "UICheckBox.h"
#include "UISlider.h"