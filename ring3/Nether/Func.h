#ifndef FUNC_H
#define FUNC_H

#include "Includes.h"

struct Store{
	Store();
	~Store();
public:
	bool tOn, eOn, aOn;
	int i_team, e_team, iHealth,  i_Count, fFlags;
	float fPos[3], Angle[3];
};

class Engine: public Store{
public:

#pragma region FUNCTION PROTOTYPES
	//Misc & Reading Functions
	Engine();
	float Get3D(float X, float Y, float Z, float eX, float eY, float eZ);
	void Reading(bool on);
	~Engine();

	//Triggerbot
	void Trigger();

	//ESP Prototypes
	bool WorldToScreen(float * from, float * to);
	void DrawFilledRect(int x, int y, int w, int h);
	void DrawBorderBox(int x, int y, int w, int h, int thickness);
	void DrawString(int x, int y, COLORREF color, const char* text);
	void DrawESP(int x, int y, float distance, int Health, int fFlags);
	void ESP();

	//Aim-Bot Prototypes
	void CalcAngle( float *src, float *dst, float *angles, int fFlags);
	float CloseEnt();
	void Aimbot();
	
#pragma endregion
#pragma region Variables
	//Misc Variables
	DWORD dwClient, dwEngine, dwPBase, dwEntity, dwBase, dwAngPtr;
	int i_Enemies[32];
	
	//ESP Variables
	RECT m_Rect; 
	HDC HDC_Desktop;//Our desktop handle
	HBRUSH EnemyBrush;//Brush to paint ESP etc
	HFONT Font; //font we use to write text with
	HWND TargetWnd, Handle;
	COLORREF TextCOLOR;
	
	//World To Screen Info
	typedef struct{
		float flMatrix [4][4];
	}WorldToScreenMatrix_t;
	WorldToScreenMatrix_t W2S_M;
#pragma endregion
};
#endif