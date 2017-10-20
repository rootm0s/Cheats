#include "DLLMain.h"
#include <filesystem>
//===================================================================================
SYSTEMTIME newTime;
cVars gVars;
cGui gGui;
bool hasFocus;
static bool doitOnce = false;
int i;


bool bKeyPressed[256];
bool IsKeyPressed(int iKey){
	//used for menu toggle
	if (GetAsyncKeyState(iKey)){
		if (!bKeyPressed[iKey]){
			bKeyPressed[iKey] = true;
			return true;
		}
	}
	else
		bKeyPressed[iKey] = false;

	return false;
}
/*
__  __                           _____ _          __  __
|  \/  |                         / ____| |        / _|/ _|
| \  / | ___  _   _ ___  ___    | (___ | |_ _   _| |_| |_
| |\/| |/ _ \| | | / __|/ _ \    \___ \| __| | | |  _|  _|
| |  | | (_) | |_| \__ \  __/    ____) | |_| |_| | | | |
|_|  |_|\___/ \__,_|___/\___|   |_____/ \__|\__,_|_| |_|

*/

void cGui::DrawMouse()
{
	if (!cursoractive)
		return;

	DWORD color;

	g_Draw.FillRGBA(cursorpos[0] + 1, cursorpos[1], 1, 17, 3, 6, 26, 255);
	for (int i = 0; i < 11; i++)
		g_Draw.FillRGBA(cursorpos[0] + 2 + i, cursorpos[1] + 1 + i, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 8, cursorpos[1] + 12, 5, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 8, cursorpos[1] + 13, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 9, cursorpos[1] + 14, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 10, cursorpos[1] + 16, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 8, cursorpos[1] + 18, 2, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 7, cursorpos[1] + 16, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 6, cursorpos[1] + 14, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 5, cursorpos[1] + 13, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 4, cursorpos[1] + 14, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 3, cursorpos[1] + 15, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(cursorpos[0] + 2, cursorpos[1] + 16, 1, 1, 3, 6, 26, 255);
	for (int i = 0; i < 4; i++)
		g_Draw.FillRGBA(cursorpos[0] + 2 + i, cursorpos[1] + 2 + i, 1, 14 - (i * 2), 255 - (i * 4), 255 - (i * 4), 255 - (i * 4), 255);
	g_Draw.FillRGBA(cursorpos[0] + 6, cursorpos[1] + 6, 1, 8, 235, 235, 235, 255);
	g_Draw.FillRGBA(cursorpos[0] + 7, cursorpos[1] + 7, 1, 9, 231, 231, 231, 255);
	for (int i = 0; i < 4; i++)
	g_Draw.FillRGBA(cursorpos[0] + 8 + i, cursorpos[1] + 8 + i, 1, 4 - i, 227 - (i * 4), 227 - (i * 4), 227 - (i * 4), 255);
	g_Draw.FillRGBA(cursorpos[0] + 8, cursorpos[1] + 14, 1, 4, 207, 207, 207, 255);
	g_Draw.FillRGBA(cursorpos[0] + 9, cursorpos[1] + 16, 1, 2, 203, 203, 203, 255);
}

void cGui::GetMouseClic()
{
	if (cursoractive)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
			leftclic = true;
		else
		{
			if (leftclic)
				leftclicreleased = true;
			else
				leftclicreleased = false;

			leftclic = false;
		}
	}
}

bool cGui::MouseOver(int x, int y, int w, int h)
{
	if ((cursorpos[0] > x) && (cursorpos[0] < x + w) && (cursorpos[1] > y) && (cursorpos[1] < y + h))
		return true;

	return false;
}

bool cGui::MouseLeftClic(int x, int y, int w, int h)
{
	if (leftclicreleased && (cursorpos[0] > x) && (cursorpos[0] < x + w) && (cursorpos[1] > y) && (cursorpos[1] < y + h))
		return true;

	return false;
}

BOOL WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nInteger, LPCTSTR lpFileName){
	TCHAR lpString[1024];
	wsprintf(lpString, "%d", nInteger);
	return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}
void cGui::blackBorder(int x, int y, int w, int h)
{
	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 0, 0, 0, 254); //top
	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 0, 0, 0, 254);	//left
	g_Draw.FillRGBA(x + w, y, 1, h - 1, 0, 0, 0, 254);	//right
	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 0, 0, 0, 254); //bottom
}
void yellowBorder(int x, int y, int w, int h)
{
	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 193,1,0, 254); //top
	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 193,1,0, 254);	//left
	g_Draw.FillRGBA(x + w, y, 1, h - 1, 193,1,0, 254);	//right
	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 193,1,0, 254); //bottom
}
void yellowBorder2(int x, int y, int w, int h)
{
	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 193, 1 , 0, 254); //top
	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 133, 175, 192, 254);	//left
	g_Draw.FillRGBA(x + w, y, 1, h - 1, 133, 175, 192, 254);	//right
	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 133, 175, 192, 254); //bottom
}
void pinkborder(int x, int y, int w, int h)
{
	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 133, 175, 192, 254); //top
	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 133, 175, 192, 254);	//left
	g_Draw.FillRGBA(x + w, y, 1, h - 1, 133, 175, 192, 254);	//right
	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 133, 175, 192, 254); //bottom
}
void whiteborder(int x, int y, int w, int h)
{
	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 255, 255, 255, 200); //top
	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 255, 255, 255, 200);	//left
	g_Draw.FillRGBA(x + w, y, 1, h - 1, 255, 255, 255, 200);	//right
	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 255, 255, 255, 200); //bottom
}
void cGui::Title(int x, int y, int w, int h, char *title)
	{
		
	}
void Checkbox2(char *var, int x, int y, char *name, char *desc)
{
	if (gGui.MouseOver(x + 206, y, 15, 15)){
		g_Draw.FillRGBA(x + 205, y, 17, 17, 35, 35, 35, 255);
		g_Draw.FillRGBA(x + 206, y + 1, 15, 15, 60, 60, 60, 255);
		g_Draw.FillRGBA(x + 205, y, 17, 17, 35, 35, 35, 70);
	}
	else{
		g_Draw.FillRGBA(x + 205, y, 17, 17, 35, 35, 35, 255);
		g_Draw.FillRGBA(x + 206, y + 1, 15, 15, 60, 60, 60, 255);
	}
	if (g_Console.getValue(var) == 0)
	{
		if (gGui.MouseLeftClic(x + 207, y, 15, 15))
		{
			g_Console.setValue(var, 1);
		}
	}
	else{
		g_Draw.FillRGBA(x + 207, y + 2, 13, 13, 133, 175, 192, 255);
		//g_Draw.DrawStringA(g_Draw.Font_CHECK, false, x + 209, y , 255,255,255,255, "X");

		if (gGui.MouseLeftClic(x + 207, y, 15, 15))
		{
			g_Console.setValue(var, 0);
		}
	}

	g_Draw.DrawStringA(g_Draw.font_GUI, false, x - 60, y + 1, 255, 255, 255, 255, name);
	if (gGui.MouseOver(x - 60, y + 1, 120, 15))
		g_Draw.DrawStringA(g_Draw.font_GUI, false, gVars.misc_menu_x + 10, gVars.misc_menu_y + 6, 133, 175, 192, 255, desc);
}
	//==================================================================================

#pragma region FX Menu
	void cGui::Body2(int x, int y, int w, int h)
	{
		int iScreenWidth, iScreenHeight;
		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		gVars.misc_menu_x = iScreenWidth / 2 - 175;
		gVars.misc_menu_y = iScreenHeight / 2 - 140;
		/******************************************Tabs**********************************************/
		//Background
		g_Draw.FillRGBA( gVars.misc_menu_x + 20, gVars.misc_menu_y - 20, 330, 21, 60, 60, 60, 255);
		for (int i = 0; i <= 20; i++)
		{
			g_Draw.LineRGBA(gVars.misc_menu_x + i, gVars.misc_menu_y, gVars.misc_menu_x + 20 + i, gVars.misc_menu_y - 20, 60, 60, 60, 255);
			g_Draw.LineRGBA(gVars.misc_menu_x + 370 - i, gVars.misc_menu_y - 20, gVars.misc_menu_x + 350 - i, gVars.misc_menu_y, 60, 60, 60, 255);
		}
		//Tab #1
		g_Draw.LineRGBA(gVars.misc_menu_x, gVars.misc_menu_y, gVars.misc_menu_x + 20, gVars.misc_menu_y - 20, 85, 85, 85, 255);//  Erste Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 20, gVars.misc_menu_y - 20, gVars.misc_menu_x + 100, gVars.misc_menu_y - 20, 85, 85, 85, 255);  //Gerade Linie von Oben
		g_Draw.LineRGBA(gVars.misc_menu_x + 80, gVars.misc_menu_y, gVars.misc_menu_x + 100, gVars.misc_menu_y - 20, 85, 85, 85, 255);//  Zweite Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x, gVars.misc_menu_y, gVars.misc_menu_x + 80, gVars.misc_menu_y, 85, 85, 85, 255); // Gerade Linie von Unten
		g_Draw.DrawStringA(g_Draw.font_Tab, false, gVars.misc_menu_x + 40, gVars.misc_menu_y - 20, 133, 175, 192, 255, "Aim");
		//Tab #2
		g_Draw.LineRGBA(gVars.misc_menu_x + 90, gVars.misc_menu_y, gVars.misc_menu_x + 110, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Erste Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 110, gVars.misc_menu_y - 20, gVars.misc_menu_x + 190, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Gerade Linie von Oben
		g_Draw.LineRGBA(gVars.misc_menu_x + 170, gVars.misc_menu_y, gVars.misc_menu_x + 190, gVars.misc_menu_y - 20, 85, 85, 85, 255);  //Zweite Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 90, gVars.misc_menu_y, gVars.misc_menu_x + 170, gVars.misc_menu_y, 85, 85, 85, 255); // Gerade Linie von Unten
		g_Draw.DrawStringA(g_Draw.font_Tab, false, gVars.misc_menu_x + 130, gVars.misc_menu_y - 20, 133, 175, 192, 255, "ESP");
		//Tab #3
		g_Draw.LineRGBA(gVars.misc_menu_x + 180, gVars.misc_menu_y, gVars.misc_menu_x + 200, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Erste Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 200, gVars.misc_menu_y - 20, gVars.misc_menu_x + 280, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Gerade Linie von Oben
		g_Draw.LineRGBA(gVars.misc_menu_x + 260, gVars.misc_menu_y, gVars.misc_menu_x + 280, gVars.misc_menu_y - 20, 85, 85, 85, 255);  //Zweite Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 180, gVars.misc_menu_y, gVars.misc_menu_x + 260, gVars.misc_menu_y, 85, 85, 85, 255);  //Gerade Linie von Unten
		g_Draw.DrawStringA(g_Draw.font_Tab, false, gVars.misc_menu_x + 210, gVars.misc_menu_y - 20, 133, 175, 192, 255, "VISUALS");
		//Tab #4
		g_Draw.LineRGBA(gVars.misc_menu_x + 270, gVars.misc_menu_y, gVars.misc_menu_x + 290, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Erste Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 290, gVars.misc_menu_y - 20, gVars.misc_menu_x + 370, gVars.misc_menu_y - 20, 85, 85, 85, 255); // Gerade Linie von Oben
		g_Draw.LineRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y, gVars.misc_menu_x + 370, gVars.misc_menu_y - 20, 85, 85, 85, 255);  //Zweite Schräge nach Rechts
		g_Draw.LineRGBA(gVars.misc_menu_x + 270, gVars.misc_menu_y, gVars.misc_menu_x + 350, gVars.misc_menu_y, 85, 85, 85, 255);  //Gerade Linie von Unten
		g_Draw.DrawStringA(g_Draw.font_Tab, false, gVars.misc_menu_x + 310, gVars.misc_menu_y - 20, 133, 175, 192,255,  "MISC");
		/*********************************************************************************************/

		/******************************************Menu**********************************************/
		blackBorder(gVars.misc_menu_x + 1, gVars.misc_menu_y + 2, 348, 279); // Black outlining of Dark Gray background
		g_Draw.FillRGBA(gVars.misc_menu_x + 1, gVars.misc_menu_y + 2, 348, 278, 35, 35, 35, 255); // Dark Gray background
		blackBorder(gVars.misc_menu_x + 7, gVars.misc_menu_y + 8, 336, 267); // Black outlining of Dark Gray background
		g_Draw.FillRGBA(gVars.misc_menu_x + 7, gVars.misc_menu_y + 8, 336, 266, 50, 50, 50, 255);  //Gray Background
		g_Draw.FillRGBA(gVars.misc_menu_x + 5, gVars.misc_menu_y + 6, 340, 17, 0, 0, 0, 255);
		g_Draw.FillRGBA(gVars.misc_menu_x + 5, gVars.misc_menu_y + 7, 340, 15, 50, 50, 50, 255);

		#define BUILDSTAMP ( __DATE__ " @ " __TIME__ )
		//g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 70, gVars.misc_menu_y + 7, 133, 175, 192,255, "BINARY BUILD DATE: %s", BUILDSTAMP);

		// Save/Load Zeug
		/*Legit*/
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 7, 133, 175, 192,255, "L");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 17, 133, 175, 192,255, "E");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 27, 133, 175, 192,255, "G");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 37, 133, 175, 192, 255,"I");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 47, 133, 175, 192, 255,"T");
		g_Draw.FillRGBA(gVars.misc_menu_x - 15, gVars.misc_menu_y + 87, 15, 55, 0, 0, 0, 255);
		if (MouseLeftClic(gVars.misc_menu_x - 14, gVars.misc_menu_y + 88, 14, 53))
		{
			g_Console.readConfig("legit");
			g_Draw.FillRGBA(gVars.misc_menu_x - 15, gVars.misc_menu_y + 87, 15, 55, 133, 175, 192, 255);
		}
		g_Draw.FillRGBA(gVars.misc_menu_x - 14, gVars.misc_menu_y + 88, 14, 53, 100, 100, 100, 255);
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 88, 133, 175, 192, 255, "L");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 100, 133, 175, 192,255, "O");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 112, 133, 175, 192, 255 ,"A");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 124, 133, 175, 192, 255, "D");
		g_Draw.FillRGBA(gVars.misc_menu_x - 15, gVars.misc_menu_y + 193, 15, 55, 0, 0, 0, 255);
		if (MouseLeftClic(gVars.misc_menu_x - 14, gVars.misc_menu_y + 194, 14, 53))
		{
			g_Console.saveConfig("legit");
			g_Draw.FillRGBA(gVars.misc_menu_x - 15, gVars.misc_menu_y + 193, 15, 55, 133, 175, 192, 255);
		}
		g_Draw.FillRGBA(gVars.misc_menu_x - 14, gVars.misc_menu_y + 194, 14, 53, 100, 100, 100, 255);
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 194, 133, 175, 192, 255, "S");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 206, 133, 175, 192, 255, "A");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 218, 133, 175, 192, 255, "V");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x - 10, gVars.misc_menu_y + 230, 133, 175, 192, 255, "E");
		/*Rage*/
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 7, 133, 175, 192, 255, "R");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 17, 133, 175, 192, 255, "A");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 27, 133, 175, 192, 255, "G");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 37, 133, 175, 192, 255, "E");
		g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 87, 16, 50, 0, 0, 0, 255);
		if (MouseLeftClic(gVars.misc_menu_x + 350, gVars.misc_menu_y + 88, 15, 48))
		{
			g_Console.readConfig("rage");
			g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 87, 16, 50, 133, 175, 192, 255);
		}
		g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 88, 15, 48, 100, 100, 100, 255);
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 88, 133, 175, 192, 255, "L");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 98, 133, 175, 192, 255, "O");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 108, 133, 175, 192, 255, "A");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 118, 133, 175, 192, 255, "D");
		g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 193, 15, 55, 0, 0, 0, 255);
		if (MouseLeftClic(gVars.misc_menu_x + 355, gVars.misc_menu_y + 194, 14, 53))
		{
			g_Console.saveConfig("rage");
			g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 193, 15, 55, 133, 175, 192, 255);
		}
		g_Draw.FillRGBA(gVars.misc_menu_x + 350, gVars.misc_menu_y + 194, 14, 53, 100, 100, 100, 255);
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 194, 133, 175, 192, 255, "S");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 206, 133, 175, 192, 255, "A");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 218, 133, 175, 192, 255, "V");
		g_Draw.DrawStringA(g_Draw.font_build, false, gVars.misc_menu_x + 355, gVars.misc_menu_y + 230, 133, 175, 192, 255, "E");

		///*********************************************************************************************/
	}
	void cGui::PlusMinus2(char *var, int x, int y, char *name, char *desc)
	{
		//blackBorder(x+45.70,y+4,10,10);
		//g_Draw.FillRGBA(x+45.70,y+4,10,10,0,153,255,155);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+47.65f,y+4.10f,255,255,255,false,"+");
		//if(gMouse.Leftclick(x+46,y+1,10,15))
		//	*var = 1;
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+18,y+4.70f,255,255,255,false,"%0.1f",*var);
		//blackBorder(x+4.80,y+4.80,10,10); //blackBorder(x+5,y+1,10,10);
		//g_Draw.FillRGBA(x+4.80,y+4.80,10,10,0,153,255,155);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+8.5f,y+4.70f,255,255,255,false,"-");
		//if(gMouse.Leftclick(x+5,y+1,10,15))
		//	*var = 0;
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x-60,y+1,255,255,255,false,name);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+60,y+1,255,255,255,false,desc);


		blackBorder(x + 225.70, y + 4, 12, 11);
		if (MouseOver(x + 93, y + 1, 11, 15)){
			g_Draw.FillRGBA(x + 225.70, y + 4, 12, 10, 79, 79, 79, 255);
		}
		else{
			g_Draw.FillRGBA(x + 225.70, y + 4, 12, 10, 79, 79, 79, 255);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 227.65f, y + 2.70f, 133, 175, 192, 255, ">");
		if (MouseLeftClic(x + 223, y + 1, 11, 15))
		{
			g_Console.setValue(var, 1);
		}
		if (MouseLeftClic(x + 223, y + 1, 11, 15))
		{
			yellowBorder2(x + 225.70, y + 4, 12, 11);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 210, y + 4.40f, 255, 255, 255, 255, "%i", g_Console.getValue(var));

		blackBorder(x + 190.80, y + 4.80, 12, 11); //blackBorder(x+5,y+1,10,10);
		if (MouseOver(x + 190, y + 1, 15, 15)){
			g_Draw.FillRGBA(x + 190.80, y + 4.80, 12, 10, 79, 79, 79, 255);
		}
		else{

			g_Draw.FillRGBA(x + 190.80, y + 4.80, 12, 10, 79, 79, 79, 255);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 192.5f, y + 2.70f, 133, 175, 192, 255, "<");
		if (MouseLeftClic(x + 192, y + 1, 15, 15))
		{
			g_Console.setValue(var, 0);
		}
		if (MouseLeftClic(x + 192, y + 1, 15, 15))
		{
			yellowBorder2(x + 190.80, y + 4.80, 12, 11);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI, false, x - 60, y + 1, 255, 255, 255, 255, name);

		if (gGui.MouseOver(x-60, y + 1, 120, 15))
			g_Draw.DrawStringA(g_Draw.font_GUI, false, gVars.misc_menu_x + 10, gVars.misc_menu_y + 6, 133, 175, 192, 255, desc);
	}
	void cGui::LessMore2(char *var, int x, int y, char *name, char *desc, int limit)
	{
		//blackBorder(x+45.70,y+1,10,10);
		//g_Draw.FillRGBA(x+45.70,y+1,10,10,0,0,0,155);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+47.65f,y+1.10f,255,255,255,false,"+");
		//if(gMouse.OneLeftclick(x+46,y+1,10,10))
		//	*var += 1;
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+18,y,255,255,255,false,"%i",*var);
		//blackBorder(x+4.80,y+1.80,10,10); //blackBorder(x+5,y+1,10,10);
		//g_Draw.FillRGBA(x+4.80,y+1.80,10,10,0,0,0,155);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+8.5f,y+1.70f,255,255,255,false,"-");
		//if(gMouse.OneLeftclick(x+5,y+1,10,10))
		//	*var -= 1;
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x-60,y,255,255,255,false,name);
		//g_Draw.DrawStringA(g_Draw.font_GUI,false,x+60,y,255,255,255,false,desc);

		if (g_Console.getValue(var) < 10 && g_Console.getValue(var) >= 0)
		{
			g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 210, y + 4.70f, 255, 255, 255, 255, "%i", g_Console.getValue(var));
		}
		if (g_Console.getValue(var) >= 10 && g_Console.getValue(var) < 100)
		{
			g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 208, y + 4.70f, 255, 255, 255, 255, "%i", g_Console.getValue(var));
		}
		if (g_Console.getValue(var) >= 100)
		{
			g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 207, y + 4.70f, 255, 255, 255, 255, "%i", g_Console.getValue(var));
		}
		/*g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 210, y + 4.70f, 255, 255, 255, "%i", *var);*/
		blackBorder(x + 225.70, y + 4, 12, 11);
		if (MouseOver(x + 223, y + 1, 11, 15)){
			g_Draw.FillRGBA(x + 225.70, y + 4, 12, 10, 79, 79, 79, 255);
		}
		else{
			g_Draw.FillRGBA(x + 225.70, y + 4, 12, 10, 79, 79, 79, 255);
		}
		if (g_Console.getValue(var) > limit)
		{
			g_Console.setValue(var,0);
		}
		if (g_Console.getValue(var) < 0)
		{
			g_Console.setValue(var, limit);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 227.65f, y + 2.70f, 133, 175, 192, 255, ">");
		if (MouseLeftClic(x + 223, y + 1, 11, 15))
		{
			int value = g_Console.getValue(var);
			g_Console.setValue(var, value += 1);
		}
		if (MouseLeftClic(x + 223, y + 1, 11, 15))
		{
			yellowBorder2(x + 225.70, y + 4, 12, 11);
		}

		blackBorder(x + 190.80, y + 4.80, 12, 11); //blackBorder(x+5,y+1,10,10);
		if (MouseOver(x + 192, y + 1, 15, 15)){
			g_Draw.FillRGBA(x + 190.80, y + 4.80, 12, 10, 79, 79, 79, 255);
		}
		else{
			g_Draw.FillRGBA(x + 190.80, y + 4.80, 12, 10, 79, 79, 79, 255);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI2, false, x + 192.5f, y + 2.70f, 133, 175, 192, 255, "<");
		if (MouseLeftClic(x + 190, y + 1, 15, 15))
		{
			int value = g_Console.getValue(var);
			g_Console.setValue(var, value -= 1);
		}
		if (MouseLeftClic(x + 190, y + 1, 15, 15))
		{
			yellowBorder2(x + 190.80, y + 4.80, 12, 11);
		}
		g_Draw.DrawStringA(g_Draw.font_GUI, false, x - 60, y + 1, 255, 255, 255, 255, name);
		if (gGui.MouseOver(x - 60, y + 1, 120, 15))
			g_Draw.DrawStringA(g_Draw.font_GUI, false, gVars.misc_menu_x + 10, gVars.misc_menu_y + 6, 133, 175, 192, 255, desc);

	}
	void cGui::MenuPages2()
	{
		int iScreenWidth, iScreenHeight;
		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		if (gVars.menu_page2 == 1)// Aimbot
		{
			g_Draw.LineRGBA(iScreenWidth / 2 - 175, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 20, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Erste Schräge nach Rechts
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 20, iScreenHeight / 2 - 140 - 20, iScreenWidth / 2 - 175 + 100, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Gerade Linie von Oben
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 100, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Zweite Schräge nach Rechts
			Checkbox2("aim_active", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 35, "aim_active", "Enable Aimbot");
			Checkbox2("aim_key", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 55, "aim_legit_key", "This makes the Aimbot shoot when pressing MOUSE1");
			this->LessMore2("aim_targetby", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 75, "aim_targetby","This specifies the targetselection", 2);
			this->LessMore2("aim_smooth_y", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 95, "aim_legit_smooth_x", "Smoothes the Aim-Movement", 100);
			this->LessMore2("aim_smooth_x", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 115, "aim_legit_smooth_y", "Smoothes the Aim-Movement", 100);
			this->LessMore2("aim_fov", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 135, "aim_legit_fov", "Specifies the radius where the Aimbot is working", 360);
			this->LessMore2("aim_hitbox", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 155, "aim_legit_hitbox", "This is the desired Hitbox the Aimbot shoots", 11);
			Checkbox2("esp_hitbox", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 175, "aim_legit_hitbox_draw", "This draws the chosen hitbox");
			Checkbox2("aim_silent", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 195, "aim_rage_silent", "This makes the Aimbot invisible for you");
			Checkbox2("aim_psilent", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 215, "aim_rage_psilent", "This makes the Aimbot invisible for everyone");

		}
		if (gVars.menu_page2 == 2)
		{
			int maxvalue = 4;
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 90, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 110, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Erste Schräge nach Rechts
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 110, iScreenHeight / 2 - 140 - 20, iScreenWidth / 2 - 175 + 190, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Gerade Linie von Oben
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 170, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 190, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Zweite Schräge nach Rechts
			Checkbox2("esp_enemyonly", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 35, "esp_enemyonly", "This makes the ESP only draw at enemies");
			Checkbox2("esp_visibleonly", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 55, "esp_visibleonly", "This makes the ESP only draw when player is visible");
			Checkbox2("esp_box", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 75, "esp_box", "This draws a box around players");
			Checkbox2("esp_name", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 95, "esp_name", "This shows the players name");
			Checkbox2("esp_health", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 115, "esp_health", "This shows the players health in a healthbar");
			Checkbox2("esp_weapon", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 135, "esp_weapon", "This shows the players weapon");
			Checkbox2("esp_bone", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 155, "esp_bone", "Draws the player models bones");
			Checkbox2("esp_helmet", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 175, "esp_helmet", "This shows if the player has a helmet");
			Checkbox2("esp_flashed", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 195, "esp_flashed", "This shows if the player is flashed");
			Checkbox2("esp_entity", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 215, "esp_entity", "This shows entities on the map");
		}
		if (gVars.menu_page2 == 3)
		{
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 180, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 200, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Erste Schräge nach Rechts
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 200, iScreenHeight / 2 - 140 - 20, iScreenWidth / 2 - 175 + 280, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Gerade Linie von Oben
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 260, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 280, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Zweite Schräge nach Rechts
			Checkbox2("vis_radar", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 35, "vis_radar", "This enables a radar showing enemies");
			this->LessMore2("vis_chams", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 55, "vis_chams", "This makes players model colored", 2);
			Checkbox2("esp_glow", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 75, "vis_glow", "This outlines the players model");
			Checkbox2("misc_noflash", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 95, "vis_noflash", "This removes the flash effect");
			this->LessMore2("misc_crosshair", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 115, "vis_crosshair", "This shows a crosshair", 6);

		}
		if (gVars.menu_page2 == 4)
		{
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 270, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 290, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Erste Schräge nach Rechts
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 290, iScreenHeight / 2 - 140 - 20, iScreenWidth / 2 - 175 + 370, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255); // Gerade Linie von Oben
			g_Draw.LineRGBA(iScreenWidth / 2 - 175 + 350, iScreenHeight / 2 - 140, iScreenWidth / 2 - 175 + 370, iScreenHeight / 2 - 140 - 20, 133, 175, 192, 255);  //Zweite Schräge nach Rechts
			Checkbox2("misc_norecoil", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 35, "misc_norecoil", "This removes the weapons recoil");
			Checkbox2("misc_rcs", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 55, "misc_recoilcontrol", "This compensates the weapons recoil");
			Checkbox2("misc_scs", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 75, "misc_spreadcontrol", "This compensates the weapons spread");
			Checkbox2("misc_nospread", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 95, "misc_nospread", "This removes the weapons spread");
			Checkbox2("misc_bhop", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 115, "misc_bunnyhop", "This automatically bunnyhops for you");
			Checkbox2("misc_strafe", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 135, "misc_autostrafe", "This automatically strafes for you");
			Checkbox2("misc_autopistol", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 155, "misc_autopistol", "This makes Pistols shoot automatically");
			//Checkbox2("misc_stats", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 175, "misc_stats", "This gives you some statistics");
			Checkbox2("trigger_active", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 175, "misc_trigger", "This enables the triggerbot");
			this->LessMore2("trigger_key", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 195, "misc_triggerkey", "This specifies a key to activate the Triggerbot", 255);
			Checkbox2("trigger_spot", iScreenWidth / 2 - 175 + 80, iScreenHeight / 2 - 140 + 215, "misc_triggerspot", "This limits the Triggerbot on a certain hitbox");
		}

	}
	void cGui::MenuButtons2()
	{
		int iScreenWidth, iScreenHeight;
		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		if (MouseLeftClic(iScreenWidth / 2 - 175, iScreenHeight / 2 - 140 - 20, 100, 20))
		{
			gVars.menu_page2 = 1;
		}
		if (MouseLeftClic(iScreenWidth / 2 - 175 + 90, iScreenHeight / 2 - 140 - 20, 100, 20))
		{
			gVars.menu_page2 = 2;
		}
		if (MouseLeftClic(iScreenWidth / 2 - 175 + 180, iScreenHeight / 2 - 140 - 20, 100, 20))
		{
			gVars.menu_page2 = 3;
		}
		if (MouseLeftClic(iScreenWidth / 2 - 175 + 270, iScreenHeight / 2 - 140 - 20, 100, 20))
		{
			gVars.menu_page2 = 4;
		}
	}
#pragma endregion

	//==================================================================================
	//===================================================================================
	void cGui::DrawMenu(int x, int y, int w, int h, char *title)
	{
		//enable mouse and gui by clicking on insert
		if (IsKeyPressed(VK_INSERT))
		{
			cursoractive = !cursoractive;
			//block the view ingame when menu is active
			if (g_pEngine->isinGame())
			{
				char buf[32];
				sprintf(buf, "cl_mouseenable %i", !cursoractive);	//when gui enabled => cl_mouseenable 0
				g_pEngine->ClientCmd(buf);
			}
		}

		if (cursoractive)
		{
			Title(x, y, w, h, "muma");
			//Menu - FX
			Body2(x, y, w, h);
			MenuButtons2();
			MenuPages2();
			gGui.GetMouseClic();
		}

		/* Blue Watch - Borders */
		/* End Of Code */
	}
	//===================================================================================
	void cGui::DrawConsole(int x, int y, int w, int h, char* title)
	{
		this->Title(x, y - 21, w, 20, title);
		g_Draw.FillRGBA(x, y, w, h, 153, 153, 153, 155);
		blackBorder(x, y, w, h);
		g_Draw.FillRGBA(x + 5, y + 9, w - 9, 1, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + 5, y + h - 27, w - 9, 1, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + 5, y + 9, 1, h - 35, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + w - 5, y + 9, 1, h - 35, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + 5, y + h - 19, w - 75, 1, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + 5, y + h - 5, w - 75, 1, 0, 0, 0, 255);
		g_Draw.FillRGBA(x + 5, y + h - 18, 1, 14, 0, 0, 0, 255);
	}
	//===================================================================================
	void cGui::LoadConfig1()
	{
		char* szIniFile = "C:\\SEARCHYWEBZ CSGO - Legit.ini";
		//Aim
		gVars.aim_active = GetPrivateProfileIntA("Aim", "Active", 0, szIniFile);
		gVars.aim_key = GetPrivateProfileInt("Aim", "Key", 0, szIniFile);
		gVars.aim_hitbox = GetPrivateProfileInt("Aim", "Hitbox", 0, szIniFile);
		gVars.esp_hitbox = GetPrivateProfileInt("Aim", "Draw Hitbox", 0, szIniFile);
		gVars.aim_fov = GetPrivateProfileInt("Aim", "FOV", 0, szIniFile);
		gVars.aim_silent = GetPrivateProfileInt("Aim", "Silent", 0, szIniFile);
		gVars.aim_psilent = GetPrivateProfileInt("Aim", "pSilent", 0, szIniFile);
		gVars.aim_smooth_x = GetPrivateProfileInt("Aim", "aim_smooth_x", 0, szIniFile);
		gVars.aim_smooth_y = GetPrivateProfileInt("Aim", "aim_smooth_y", 0, szIniFile);
		//Esp
		gVars.esp_enemyonly = GetPrivateProfileInt("ESP", "EnemyOnly", 0, szIniFile);
		gVars.esp_box = GetPrivateProfileInt("ESP", "Box", 0, szIniFile);
		gVars.esp_name = GetPrivateProfileInt("ESP", "Name", 0, szIniFile);
		gVars.esp_health = GetPrivateProfileInt("ESP", "Health", 0, szIniFile);
		gVars.esp_weapon = GetPrivateProfileInt("ESP", "Weapon ESP", 0, szIniFile);
		gVars.esp_helmet = GetPrivateProfileInt("ESP", "Helmet", 0, szIniFile);
		gVars.esp_flashed = GetPrivateProfileInt("ESP", "Flashed", 0, szIniFile);
		gVars.esp_entity = GetPrivateProfileInt("ESP", "Entity", 0, szIniFile);
		//Misc 
		gVars.misc_norecoil = GetPrivateProfileInt("Misc", "NoRecoil", 0, szIniFile);
		gVars.misc_recoilreduce = GetPrivateProfileInt("Misc", "Recoil Reducer", 0, szIniFile);
		gVars.aim_recoil_x = GetPrivateProfileInt("Misc", "Recoil Reducer X", 0, szIniFile);
		gVars.aim_recoil_y = GetPrivateProfileInt("Misc", "Recoil Reducer Y", 0, szIniFile);
		gVars.misc_rcs = GetPrivateProfileInt("Misc", "RecoilControl", 0, szIniFile);
		gVars.misc_nospread = GetPrivateProfileInt("Misc", "NoSpread", 0, szIniFile);
		gVars.misc_bhop = GetPrivateProfileInt("Misc", "Bunnyhop", 0, szIniFile);
		gVars.vis_chams = GetPrivateProfileInt("Misc", "Chams", 0, szIniFile);
		gVars.esp_glow = GetPrivateProfileInt("Misc", "Glow", 0, szIniFile);
		gVars.misc_crosshair = GetPrivateProfileInt("Misc", "Crosshair", 0, szIniFile);
		gVars.misc_stats = GetPrivateProfileInt("Misc", "Status", 0, szIniFile);
		gVars.misc_info = GetPrivateProfileInt("Misc", "Info", 0, szIniFile);
		gVars.misc_apistol = GetPrivateProfileInt("Misc", "Autopistol", 0, szIniFile);
		gVars.vis_radar = GetPrivateProfileInt("Misc", "Radar", 0, szIniFile);
		//Color
		gVars.ct_r = GetPrivateProfileInt("Colors", "CT Red", 30, szIniFile);
		gVars.ct_g = GetPrivateProfileInt("Colors", "CT Green", 144, szIniFile);
		gVars.ct_b = GetPrivateProfileInt("Colors", "CT Red", 255, szIniFile);
		gVars.ct_a = GetPrivateProfileInt("Colors", "CT Alpha", 255, szIniFile);
		gVars.vis_ct_r = GetPrivateProfileInt("Colors", "Visible CT Red", 0, szIniFile);
		gVars.vis_ct_g = GetPrivateProfileInt("Colors", "Visible CT Green", 255, szIniFile);
		gVars.vis_ct_b = GetPrivateProfileInt("Colors", "Visible CT Red", 0, szIniFile);
		gVars.vis_ct_a = GetPrivateProfileInt("Colors", "Visible CT Alpha", 255, szIniFile);
		gVars.t_r = GetPrivateProfileInt("Colors", "T Red", 255, szIniFile);
		gVars.t_g = GetPrivateProfileInt("Colors", "T Green", 0, szIniFile);
		gVars.t_b = GetPrivateProfileInt("Colors", "T Red", 40, szIniFile);
		gVars.t_a = GetPrivateProfileInt("Colors", "T Alpha", 255, szIniFile);
		gVars.vis_t_r = GetPrivateProfileInt("Colors", "Visible T Red", 255, szIniFile);
		gVars.vis_t_g = GetPrivateProfileInt("Colors", "Visible T Green", 255, szIniFile);
		gVars.vis_t_b = GetPrivateProfileInt("Colors", "Visible T Red", 0, szIniFile);
		gVars.vis_t_a = GetPrivateProfileInt("Colors", "Visible T Alpha", 255, szIniFile);
		gVars.esp_alpha = GetPrivateProfileInt("Colors", "ESP Alpha", 255, szIniFile);
		/* Counter-Terrorists */
		gVars.chams_ct_r = GetPrivateProfileInt("Colors", "Chams CT Red", 30, szIniFile);
		gVars.chams_ct_g = GetPrivateProfileInt("Colors", "Chams CT Green", 144, szIniFile);
		gVars.chams_ct_b = GetPrivateProfileInt("Colors", "Chams CT Blue", 255, szIniFile);
		gVars.chams_vis_ct_r = GetPrivateProfileInt("Colors", "Visible Chams CT Red", 255, szIniFile);
		gVars.chams_vis_ct_g = GetPrivateProfileInt("Colors", "Visible Chams CT Green", 255, szIniFile);
		gVars.chams_vis_ct_b = GetPrivateProfileInt("Colors", "Visible Chams CT Blue", 0, szIniFile);
		/* Terrorists */
		gVars.chams_t_r = GetPrivateProfileInt("Colors", "Chams T Red", 255, szIniFile);
		gVars.chams_t_g = GetPrivateProfileInt("Colors", "Chams T Green", 0, szIniFile);
		gVars.chams_t_b = GetPrivateProfileInt("Colors", "Chams CT Red", 40, szIniFile);
		gVars.chams_vis_t_r = GetPrivateProfileInt("Colors", "Visible Chams CT Red", 255, szIniFile);
		gVars.chams_vis_t_g = GetPrivateProfileInt("Colors", "Visible Chams CT Green", 255, szIniFile);
		gVars.chams_vis_t_b = GetPrivateProfileInt("Colors", "Visible Chams CT Blue", 0, szIniFile);
		gVars.chams_alpha = GetPrivateProfileInt("Colors", "Chams Alpha", 255, szIniFile);
		// Glow colors
		/* Counter-Terrorists */
		gVars.glow_ct_r = GetPrivateProfileInt("Colors", "Glow CT Red", 30, szIniFile);
		gVars.glow_ct_g = GetPrivateProfileInt("Colors", "Glow CT Green", 144, szIniFile);
		gVars.glow_ct_b = GetPrivateProfileInt("Colors", "Glow CT Blue", 255, szIniFile);
		gVars.glow_vis_ct_r = GetPrivateProfileInt("Colors", "Visible Glow CT Red", 0, szIniFile);
		gVars.glow_vis_ct_g = GetPrivateProfileInt("Colors", "Visible Glow CT Green", 255, szIniFile);
		gVars.glow_vis_ct_b = GetPrivateProfileInt("Colors", "Visible Glow CT Blue", 0, szIniFile);
		/* Terrorists */
		gVars.glow_t_r = GetPrivateProfileInt("Colors", "Glow T Red", 255, szIniFile);
		gVars.glow_t_g = GetPrivateProfileInt("Colors", "Glow T Green", 0, szIniFile);;
		gVars.glow_t_b = GetPrivateProfileInt("Colors", "Glow T Blue", 40, szIniFile);
		gVars.glow_vis_t_r = GetPrivateProfileInt("Colors", "Visible Glow T Red", 255, szIniFile);
		gVars.glow_vis_t_g = GetPrivateProfileInt("Colors", "Visible Glow T Green", 255, szIniFile);
		gVars.glow_vis_t_b = GetPrivateProfileInt("Colors", "Visible Glow T Blue", 0, szIniFile);
		/*Alpha*/
		gVars.glow_alpha = GetPrivateProfileInt("Colors", "Glow Alpha", 255, szIniFile);

		gVars.crosshair_r = GetPrivateProfileInt("Colors", "Crosshair Red", 255, szIniFile);
		gVars.crosshair_g = GetPrivateProfileInt("Colors", "Crosshair Green", 0, szIniFile);
		gVars.crosshair_b = GetPrivateProfileInt("Colors", "Crosshair Blue", 0, szIniFile);

		gVars.ent_r = GetPrivateProfileInt("Colors", "Entity Red", 255, szIniFile);
		gVars.ent_g = GetPrivateProfileInt("Colors", "Entity Green", 255, szIniFile);
		gVars.ent_b = GetPrivateProfileInt("Colors", "Entity Blue", 0, szIniFile);

		gVars.bomb_r = GetPrivateProfileInt("Colors", "Bomb Red", 255, szIniFile);
		gVars.bomb_g = GetPrivateProfileInt("Colors", "Bomb Green", 0, szIniFile);
		gVars.bomb_b = GetPrivateProfileInt("Colors", "Bomb Blue", 0, szIniFile);

		GetPrivateProfileString("Watermark", "Name", "SEARCHYWEBZ", gVars.watermarkname, 255, szIniFile);
	}
	void cGui::SaveConfig1()
	{
		char* szIniFile = "C:\\SEARCHYWEBZ CSGO - Legit.ini";
		//Aim
		WritePrivateProfileInt("Aim", "Active", gVars.aim_active, szIniFile);
		WritePrivateProfileInt("Aim", "Key", gVars.aim_key, szIniFile);
		WritePrivateProfileInt("Aim", "Hitbox", gVars.aim_hitbox, szIniFile);
		WritePrivateProfileInt("Aim", "Draw Hitbox", gVars.esp_hitbox, szIniFile);
		WritePrivateProfileInt("Aim", "FOV", gVars.aim_fov, szIniFile);
		WritePrivateProfileInt("Aim", "Silent", gVars.aim_silent, szIniFile);
		WritePrivateProfileInt("Aim", "pSilent", gVars.aim_psilent, szIniFile);
		WritePrivateProfileInt("Aim", "aim_smooth_x", gVars.aim_smooth_x, szIniFile);
		WritePrivateProfileInt("Aim", "aim_smooth_y", gVars.aim_smooth_y, szIniFile);
		//Esp
		WritePrivateProfileInt("Esp", "EnemyOnly", gVars.esp_enemyonly, szIniFile);
		WritePrivateProfileInt("ESP", "Box", gVars.esp_box, szIniFile);
		WritePrivateProfileInt("Esp", "Name", gVars.esp_name, szIniFile);
		WritePrivateProfileInt("Esp", "Health", gVars.esp_health, szIniFile);
		WritePrivateProfileInt("Esp", "Weapon ESP", gVars.esp_weapon, szIniFile);
		WritePrivateProfileInt("Esp", "Helmet", gVars.esp_helmet, szIniFile);
		WritePrivateProfileInt("Esp", "Flashed", gVars.esp_flashed, szIniFile);
		//Misc
		WritePrivateProfileInt("Misc", "NoRecoil", gVars.misc_norecoil, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer", gVars.misc_recoilreduce, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer X", gVars.aim_recoil_x, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer Y", gVars.aim_recoil_y, szIniFile);
		WritePrivateProfileInt("Misc", "RecoilControl", gVars.misc_rcs, szIniFile);
		WritePrivateProfileInt("Misc", "NoSpread", gVars.misc_nospread, szIniFile);
		WritePrivateProfileInt("Misc", "Glow", gVars.esp_glow, szIniFile);
		WritePrivateProfileInt("Misc", "Bunnyhop", gVars.misc_bhop, szIniFile);
		WritePrivateProfileInt("Misc", "Chams", gVars.vis_chams, szIniFile);
		WritePrivateProfileInt("Misc", "Radar", gVars.vis_radar, szIniFile);
		WritePrivateProfileInt("Misc", "Crosshair", gVars.misc_crosshair, szIniFile);
		WritePrivateProfileInt("Misc", "Status", gVars.misc_stats, szIniFile);
		WritePrivateProfileInt("Misc", "Info", gVars.misc_info, szIniFile);
		WritePrivateProfileInt("Misc", "Autopistol", gVars.misc_apistol, szIniFile);
		//Colors
		// ESP colors	
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "CT Red", gVars.ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "CT Blue", gVars.ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "CT Green", gVars.ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "CT Alpha", gVars.ct_a, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Red", gVars.vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Green", gVars.vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Blue", gVars.vis_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Alpha", gVars.vis_ct_a, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "T Red", gVars.t_r, szIniFile);
		WritePrivateProfileInt("Colors", "T Blue", gVars.t_b, szIniFile);
		WritePrivateProfileInt("Colors", "T Green", gVars.t_g, szIniFile);
		WritePrivateProfileInt("Colors", "T Alpha", gVars.t_a, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Red", gVars.vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Green", gVars.vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Blue", gVars.vis_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Alpha", gVars.vis_t_a, szIniFile);
		/* Independent */
		WritePrivateProfileInt("Colors", "ESP Alpha", gVars.esp_alpha, szIniFile);
		// Chams colors
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "Chams CT Red", gVars.chams_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Chams CT Green", gVars.chams_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Chams CT Blue", gVars.chams_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Red", gVars.chams_vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Green", gVars.chams_vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Blue", gVars.chams_vis_ct_b, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "Chams T Red", gVars.chams_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Chams T Green", gVars.chams_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Chams T Blue", gVars.chams_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Red", gVars.chams_vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Green", gVars.chams_vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Blue", gVars.chams_vis_t_b, szIniFile);
		/*Alpha*/
		WritePrivateProfileInt("Colors", "Chams Alpha", gVars.chams_alpha, szIniFile);


		// Glow colors
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "Glow CT Red", gVars.glow_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Glow CT Green", gVars.glow_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Glow CT Blue", gVars.glow_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Red", gVars.glow_vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Green", gVars.glow_vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Blue", gVars.glow_vis_ct_b, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "Glow T Red", gVars.glow_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Glow T Green", gVars.glow_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Glow T Blue", gVars.glow_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Red", gVars.glow_vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Green", gVars.glow_vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Blue", gVars.glow_vis_t_b, szIniFile);
		/*Alpha*/
		WritePrivateProfileInt("Colors", "Glow Alpha", gVars.glow_alpha, szIniFile);

		// Crosshair colors
		WritePrivateProfileInt("Colors", "Crosshair Red", gVars.crosshair_r, szIniFile);
		WritePrivateProfileInt("Colors", "Crosshair Green", gVars.crosshair_g, szIniFile);
		WritePrivateProfileInt("Colors", "Crosshair Blue", gVars.crosshair_b, szIniFile);

		// Entity colors
		WritePrivateProfileInt("Colors", "Entity Red", gVars.ent_r, szIniFile);
		WritePrivateProfileInt("Colors", "Entity Green", gVars.ent_g, szIniFile);
		WritePrivateProfileInt("Colors", "Entity Blue", gVars.ent_b, szIniFile);

		// Bomb color
		WritePrivateProfileInt("Colors", "Bomb Red", gVars.bomb_r, szIniFile);
		WritePrivateProfileInt("Colors", "Bomb Green", gVars.bomb_g, szIniFile);
		WritePrivateProfileInt("Colors", "Bomb Blue", gVars.bomb_b, szIniFile);

		WritePrivateProfileString("Watermark", "Name", gVars.watermarkname, szIniFile);
	}
	void cGui::LoadConfig2()
	{
		
		char* szIniFile = "C:\\SEARCHYWEBZ CSGO - Rage.ini";
		//Aim
		gVars.aim_active = GetPrivateProfileIntA("Aim", "Active", 0, szIniFile);
		gVars.aim_key = GetPrivateProfileInt("Aim", "Key", 0, szIniFile);
		gVars.aim_hitbox = GetPrivateProfileInt("Aim", "Hitbox", 0, szIniFile);
		gVars.esp_hitbox = GetPrivateProfileInt("Aim", "Draw Hitbox", 0, szIniFile);
		gVars.aim_fov = GetPrivateProfileInt("Aim", "FOV", 0, szIniFile);
		gVars.aim_silent = GetPrivateProfileInt("Aim", "Silent", 0, szIniFile);
		gVars.aim_psilent = GetPrivateProfileInt("Aim", "pSilent", 0, szIniFile);
		gVars.aim_smooth_x = GetPrivateProfileInt("Aim", "aim_smooth_x", 0, szIniFile);
		gVars.aim_smooth_y = GetPrivateProfileInt("Aim", "aim_smooth_y", 0, szIniFile);
		//Esp
		gVars.esp_enemyonly = GetPrivateProfileInt("ESP", "EnemyOnly", 0, szIniFile);
		gVars.esp_box = GetPrivateProfileInt("ESP", "Box", 0, szIniFile);
		gVars.esp_name = GetPrivateProfileInt("ESP", "Name", 0, szIniFile);
		gVars.esp_health = GetPrivateProfileInt("ESP", "Health", 0, szIniFile);
		gVars.esp_weapon = GetPrivateProfileInt("ESP", "Weapon ESP", 0, szIniFile);
		gVars.esp_helmet = GetPrivateProfileInt("ESP", "Helmet", 0, szIniFile);
		gVars.esp_flashed = GetPrivateProfileInt("ESP", "Flashed", 0, szIniFile);
		gVars.esp_entity = GetPrivateProfileInt("ESP", "Entity", 0, szIniFile);
		//Misc 
		gVars.misc_norecoil = GetPrivateProfileInt("Misc", "NoRecoil", 0, szIniFile);
		gVars.misc_recoilreduce = GetPrivateProfileInt("Misc", "Recoil Reducer", 0, szIniFile);
		gVars.aim_recoil_x = GetPrivateProfileInt("Misc", "Recoil Reducer X", 0, szIniFile);
		gVars.aim_recoil_y = GetPrivateProfileInt("Misc", "Recoil Reducer Y", 0, szIniFile);
		gVars.misc_rcs = GetPrivateProfileInt("Misc", "RecoilControl", 0, szIniFile);
		gVars.misc_nospread = GetPrivateProfileInt("Misc", "NoSpread", 0, szIniFile);
		gVars.misc_bhop = GetPrivateProfileInt("Misc", "Bunnyhop", 0, szIniFile);
		gVars.vis_chams = GetPrivateProfileInt("Misc", "Chams", 0, szIniFile);
		gVars.esp_glow = GetPrivateProfileInt("Misc", "Glow", 0, szIniFile);
		gVars.misc_crosshair = GetPrivateProfileInt("Misc", "Crosshair", 0, szIniFile);
		gVars.misc_stats = GetPrivateProfileInt("Misc", "Status", 0, szIniFile);
		gVars.misc_info = GetPrivateProfileInt("Misc", "Info", 0, szIniFile);
		gVars.misc_apistol = GetPrivateProfileInt("Misc", "Autopistol", 0, szIniFile);
		gVars.vis_radar = GetPrivateProfileInt("Misc", "Radar", 0, szIniFile);
		//Color
		gVars.ct_r = GetPrivateProfileInt("Colors", "CT Red", 30, szIniFile);
		gVars.ct_g = GetPrivateProfileInt("Colors", "CT Green", 144, szIniFile);
		gVars.ct_b = GetPrivateProfileInt("Colors", "CT Red", 255, szIniFile);
		gVars.ct_a = GetPrivateProfileInt("Colors", "CT Alpha", 255, szIniFile);
		gVars.vis_ct_r = GetPrivateProfileInt("Colors", "Visible CT Red", 0, szIniFile);
		gVars.vis_ct_g = GetPrivateProfileInt("Colors", "Visible CT Green", 255, szIniFile);
		gVars.vis_ct_b = GetPrivateProfileInt("Colors", "Visible CT Red", 0, szIniFile);
		gVars.vis_ct_a = GetPrivateProfileInt("Colors", "Visible CT Alpha", 255, szIniFile);
		gVars.t_r = GetPrivateProfileInt("Colors", "T Red", 255, szIniFile);
		gVars.t_g = GetPrivateProfileInt("Colors", "T Green", 0, szIniFile);
		gVars.t_b = GetPrivateProfileInt("Colors", "T Red", 40, szIniFile);
		gVars.t_a = GetPrivateProfileInt("Colors", "T Alpha", 255, szIniFile);
		gVars.vis_t_r = GetPrivateProfileInt("Colors", "Visible T Red", 255, szIniFile);
		gVars.vis_t_g = GetPrivateProfileInt("Colors", "Visible T Green", 255, szIniFile);
		gVars.vis_t_b = GetPrivateProfileInt("Colors", "Visible T Red", 0, szIniFile);
		gVars.vis_t_a = GetPrivateProfileInt("Colors", "Visible T Alpha", 255, szIniFile);
		gVars.esp_alpha = GetPrivateProfileInt("Colors", "ESP Alpha", 255, szIniFile);
		/* Counter-Terrorists */
		gVars.chams_ct_r = GetPrivateProfileInt("Colors", "Chams CT Red", 30,szIniFile);
		gVars.chams_ct_g = GetPrivateProfileInt("Colors", "Chams CT Green", 144, szIniFile);
		gVars.chams_ct_b = GetPrivateProfileInt("Colors", "Chams CT Blue", 255, szIniFile);
		gVars.chams_vis_ct_r = GetPrivateProfileInt("Colors", "Visible Chams CT Red", 255, szIniFile);
		gVars.chams_vis_ct_g = GetPrivateProfileInt("Colors", "Visible Chams CT Green", 255, szIniFile);
		gVars.chams_vis_ct_b = GetPrivateProfileInt("Colors", "Visible Chams CT Blue", 0,szIniFile);
		/* Terrorists */
		gVars.chams_t_r = GetPrivateProfileInt("Colors", "Chams T Red", 255,szIniFile);
		gVars.chams_t_g = GetPrivateProfileInt("Colors", "Chams T Green", 0, szIniFile);
		gVars.chams_t_b = GetPrivateProfileInt("Colors", "Chams CT Red", 40, szIniFile);
		gVars.chams_vis_t_r = GetPrivateProfileInt("Colors", "Visible Chams CT Red", 255, szIniFile);
		gVars.chams_vis_t_g = GetPrivateProfileInt("Colors", "Visible Chams CT Green", 255, szIniFile);
		gVars.chams_vis_t_b = GetPrivateProfileInt("Colors", "Visible Chams CT Blue", 0, szIniFile);
		gVars.chams_alpha = GetPrivateProfileInt("Colors", "Chams Alpha", 255, szIniFile);
		// Glow colors
		/* Counter-Terrorists */
		gVars.glow_ct_r = GetPrivateProfileInt("Colors", "Glow CT Red", 30, szIniFile);
		gVars.glow_ct_g = GetPrivateProfileInt("Colors", "Glow CT Green", 144, szIniFile);
		gVars.glow_ct_b = GetPrivateProfileInt("Colors", "Glow CT Blue", 255, szIniFile);
		gVars.glow_vis_ct_r = GetPrivateProfileInt("Colors", "Visible Glow CT Red", 0, szIniFile);
		gVars.glow_vis_ct_g = GetPrivateProfileInt("Colors", "Visible Glow CT Green", 255, szIniFile);
		gVars.glow_vis_ct_b = GetPrivateProfileInt("Colors", "Visible Glow CT Blue", 0, szIniFile);
		/* Terrorists */
		gVars.glow_t_r = GetPrivateProfileInt("Colors", "Glow T Red", 255, szIniFile);
		gVars.glow_t_g = GetPrivateProfileInt("Colors", "Glow T Green", 0, szIniFile);;
		gVars.glow_t_b = GetPrivateProfileInt("Colors", "Glow T Blue", 40, szIniFile);
		gVars.glow_vis_t_r = GetPrivateProfileInt("Colors", "Visible Glow T Red", 255, szIniFile); 
		gVars.glow_vis_t_g = GetPrivateProfileInt("Colors", "Visible Glow T Green", 255, szIniFile);
		gVars.glow_vis_t_b = GetPrivateProfileInt("Colors", "Visible Glow T Blue", 0, szIniFile);
		/*Alpha*/
		gVars.glow_alpha = GetPrivateProfileInt("Colors", "Glow Alpha", 255, szIniFile);

		gVars.crosshair_r = GetPrivateProfileInt("Colors", "Crosshair Red", 255, szIniFile);
		gVars.crosshair_g = GetPrivateProfileInt("Colors", "Crosshair Green", 0, szIniFile);
		gVars.crosshair_b = GetPrivateProfileInt("Colors", "Crosshair Blue", 0, szIniFile);
	
		gVars.ent_r = GetPrivateProfileInt("Colors", "Entity Red", 255, szIniFile);
		gVars.ent_g = GetPrivateProfileInt("Colors", "Entity Green", 255, szIniFile);
		gVars.ent_b = GetPrivateProfileInt("Colors", "Entity Blue", 0, szIniFile);

		gVars.bomb_r = GetPrivateProfileInt("Colors", "Bomb Red", 255, szIniFile);
		gVars.bomb_g = GetPrivateProfileInt("Colors", "Bomb Green", 0, szIniFile);
		gVars.bomb_b = GetPrivateProfileInt("Colors", "Bomb Blue", 0, szIniFile);

		 GetPrivateProfileString("Watermark", "Name", "SEARCHYWEBZ",gVars.watermarkname, 255, szIniFile);
	}
	void cGui::SaveConfig2()
	{
		
		char* szIniFile = "SEARCHYWEBZ CSGO - Rage.ini";
		//Aim
		WritePrivateProfileInt("Aim", "Active", gVars.aim_active, "C:\SEARCHYWEBZ CSGO - Rage.ini");
		WritePrivateProfileInt("Aim", "Key", gVars.aim_key, szIniFile);
		WritePrivateProfileInt("Aim", "Hitbox", gVars.aim_hitbox, szIniFile);
		WritePrivateProfileInt("Aim", "Draw Hitbox", gVars.esp_hitbox, szIniFile);
		WritePrivateProfileInt("Aim", "FOV", gVars.aim_fov, szIniFile);
		WritePrivateProfileInt("Aim", "Silent", gVars.aim_silent, szIniFile);
		WritePrivateProfileInt("Aim", "pSilent", gVars.aim_psilent, szIniFile);
		WritePrivateProfileInt("Aim", "aim_smooth_x", gVars.aim_smooth_x, szIniFile);
		WritePrivateProfileInt("Aim", "aim_smooth_y", gVars.aim_smooth_y, szIniFile);
		//Esp
		WritePrivateProfileInt("Esp", "EnemyOnly", gVars.esp_enemyonly, szIniFile);
		WritePrivateProfileInt("ESP", "Box", gVars.esp_box, szIniFile);
		WritePrivateProfileInt("Esp", "Name", gVars.esp_name, szIniFile);
		WritePrivateProfileInt("Esp", "Health", gVars.esp_health, szIniFile);
		WritePrivateProfileInt("Esp", "Weapon ESP", gVars.esp_weapon, szIniFile);
		WritePrivateProfileInt("Esp", "Helmet", gVars.esp_helmet, szIniFile);
		WritePrivateProfileInt("Esp", "Flashed", gVars.esp_flashed, szIniFile);
		//Misc
		WritePrivateProfileInt("Misc", "NoRecoil", gVars.misc_norecoil, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer", gVars.misc_recoilreduce, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer X", gVars.aim_recoil_x, szIniFile);
		WritePrivateProfileInt("Misc", "Recoil Reducer Y", gVars.aim_recoil_y, szIniFile);
		WritePrivateProfileInt("Misc", "RecoilControl", gVars.misc_rcs, szIniFile);
		WritePrivateProfileInt("Misc", "NoSpread", gVars.misc_nospread, szIniFile);
		WritePrivateProfileInt("Misc", "Glow", gVars.esp_glow, szIniFile);
		WritePrivateProfileInt("Misc", "Bunnyhop", gVars.misc_bhop, szIniFile);
		WritePrivateProfileInt("Misc", "Chams", gVars.vis_chams, szIniFile);
		WritePrivateProfileInt("Misc", "Radar", gVars.vis_radar, szIniFile);
		WritePrivateProfileInt("Misc", "Crosshair", gVars.misc_crosshair, szIniFile);
		WritePrivateProfileInt("Misc", "Status", gVars.misc_stats, szIniFile);
		WritePrivateProfileInt("Misc", "Info", gVars.misc_info, szIniFile);
		WritePrivateProfileInt("Misc", "Autopistol", gVars.misc_apistol, szIniFile);
		//Colors
		// ESP colors	
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "CT Red", gVars.ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "CT Blue", gVars.ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "CT Green", gVars.ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "CT Alpha", gVars.ct_a, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Red", gVars.vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Green", gVars.vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Blue", gVars.vis_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible CT Alpha", gVars.vis_ct_a, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "T Red", gVars.t_r, szIniFile);
		WritePrivateProfileInt("Colors", "T Blue", gVars.t_b, szIniFile);
		WritePrivateProfileInt("Colors", "T Green", gVars.t_g, szIniFile);
		WritePrivateProfileInt("Colors", "T Alpha", gVars.t_a, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Red", gVars.vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Green", gVars.vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Blue", gVars.vis_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible T Alpha", gVars.vis_t_a, szIniFile);
		/* Independent */
		WritePrivateProfileInt("Colors", "ESP Alpha", gVars.esp_alpha, szIniFile);
		// Chams colors
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "Chams CT Red", gVars.chams_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Chams CT Green", gVars.chams_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Chams CT Blue", gVars.chams_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Red", gVars.chams_vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Green", gVars.chams_vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams CT Blue", gVars.chams_vis_ct_b, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "Chams T Red", gVars.chams_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Chams T Green", gVars.chams_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Chams T Blue", gVars.chams_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Red", gVars.chams_vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Green", gVars.chams_vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Chams T Blue", gVars.chams_vis_t_b, szIniFile);
		/*Alpha*/
		WritePrivateProfileInt("Colors", "Chams Alpha", gVars.chams_alpha, szIniFile);


		// Glow colors
		/* Counter-Terrorists */
		WritePrivateProfileInt("Colors", "Glow CT Red", gVars.glow_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Glow CT Green", gVars.glow_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Glow CT Blue", gVars.glow_ct_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Red", gVars.glow_vis_ct_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Green", gVars.glow_vis_ct_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow CT Blue", gVars.glow_vis_ct_b, szIniFile);
		/* Terrorists */
		WritePrivateProfileInt("Colors", "Glow T Red", gVars.glow_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Glow T Green", gVars.glow_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Glow T Blue", gVars.glow_t_b, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Red", gVars.glow_vis_t_r, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Green", gVars.glow_vis_t_g, szIniFile);
		WritePrivateProfileInt("Colors", "Visible Glow T Blue", gVars.glow_vis_t_b, szIniFile);
		/*Alpha*/
		WritePrivateProfileInt("Colors", "Glow Alpha", gVars.glow_alpha, szIniFile);

		// Crosshair colors
		WritePrivateProfileInt("Colors", "Crosshair Red", gVars.crosshair_r, szIniFile);
		WritePrivateProfileInt("Colors", "Crosshair Green", gVars.crosshair_g, szIniFile);
		WritePrivateProfileInt("Colors", "Crosshair Blue", gVars.crosshair_b, szIniFile);

		// Entity colors
		WritePrivateProfileInt("Colors", "Entity Red", gVars.ent_r, szIniFile);
		WritePrivateProfileInt("Colors", "Entity Green", gVars.ent_g, szIniFile);
		WritePrivateProfileInt("Colors", "Entity Blue", gVars.ent_b, szIniFile);

		// Bomb color
		WritePrivateProfileInt("Colors", "Bomb Red", gVars.bomb_r, szIniFile);
		WritePrivateProfileInt("Colors", "Bomb Green", gVars.bomb_g, szIniFile);
		WritePrivateProfileInt("Colors", "Bomb Blue", gVars.bomb_b, szIniFile);
		WritePrivateProfileString("Watermark", "Name", gVars.watermarkname, szIniFile);
	}