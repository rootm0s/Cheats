#include "Func.h"
#include "ProcMem.h"

ProcMem mem;
Store EntList[32];

Store::Store(){
	//Constructor
};

Engine::Engine()
{
	//MISC
	//ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);

	//Initialize Variables
	tOn = false;
	eOn = false;
	aOn = false;

	//Initialize Memory
	mem.GetProcess("csgo.exe");
	dwClient = mem.Module("client.dll");
	dwPBase = mem.Read<DWORD>(dwClient + 0x99E324);	
	dwEngine = mem.Module("engine.dll");
	dwAngPtr = mem.Read<DWORD>(dwEngine + 0x52F424);

	//Initialize GDI Drawing
	TargetWnd = FindWindow(0, "Counter-Strike: Global Offensive");
	HDC_Desktop = GetDC(TargetWnd);
	GetWindowRect(FindWindow(NULL, "Counter-Strike: Global Offensive"), &m_Rect);
	EnemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	TextCOLOR = RGB(255, 0, 0);
}

void Engine::Reading(bool on)	
{	
	if(on)
	{
		if(eOn || aOn)
		{
			//Read Our Info			
			EntList[0].fPos[0] = mem.Read<float>(dwPBase + 0x134);					
			EntList[0].fPos[1] = mem.Read<float>(dwPBase + 0x138);				
			EntList[0].fPos[2] = mem.Read<float>(dwPBase + 0x13C);
			W2S_M = mem.Read<WorldToScreenMatrix_t>(dwClient + 0x9A2954);
		}

		//Read our TeamNum to filter out team mates
		i_team = mem.Read<int>(dwPBase + 0xF0);

		for(int i = 1; i < 64; i++)
		{
			//Loop From Base Entity Address by 0x10 On Each Iteration
			dwEntity = mem.Read<DWORD>((dwClient + 0x9B76B4) + (i * 0x10));
 
			//Prevent Crash From Reading Null Pointer - also stop counting when weve read the last entity   
			if(!dwEntity)     
				return;
 		
			//Read Current Entity's TeamNum For Compare
			e_team = mem.Read<int>(dwEntity + 0xF0);

			//If An Enemy Has Been Found, Store Their Entity Index ID Inside Array
			if(e_team != i_team && e_team > 1)
			{
				if(tOn)	
					i_Enemies[i_Count] = mem.Read<int>(dwEntity + 0x64);//Ent ID
		
				if(eOn || aOn)
				{	
					EntList[i_Count].fPos[0] = mem.Read<float>(dwEntity + 0x134);//X			
					EntList[i_Count].fPos[1] = mem.Read<float>(dwEntity + 0x138);//Y			
					EntList[i_Count].fPos[2] = mem.Read<float>(dwEntity + 0x13C);//Z 				
					EntList[i_Count].iHealth = mem.Read<int>(dwEntity + 0xFC);//HP	
					EntList[i_Count].fFlags = mem.Read<int>(dwEntity + 0x100);//HP	
				}		
				i_Count++;
			}	
		}
	}
}

float Engine::Get3D(float X, float Y, float Z, float eX, float eY, float eZ)	
{
	return(sqrtf( (eX - X) * (eX - X) + (eY - Y) * (eY - Y) + (eZ - Z) * (eZ - Z)));
}

float Engine::CloseEnt()
{
	//Variables
	float fLowest = 100000, TMP;
	int iIndex;

	for(int i = 1; i < i_Count; i++)
	{
		//Store Distances In Array
		TMP = Get3D(EntList[0].fPos[0], EntList[0].fPos[1], EntList[0].fPos[2], EntList[i].fPos[0], EntList[i].fPos[1], EntList[i].fPos[2]);

		//If Enemy Has Lower Distance The Player 1, Replace (var)Lowest With Current Enemy Distance
		if(TMP < fLowest && EntList[i].iHealth != 0)
		{
			fLowest = TMP;	
			iIndex = i;
		}
	}
	return iIndex;
}

void Engine::Trigger()
{
	if(tOn && mem.Locate(i_Enemies, i_Count, mem.Read<int>(dwPBase + 0x23B4)))   	
	{      	
		mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );       	
		Sleep(1); //Response Time     				
		mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
	}
    return;
}

//Thanks to Fleep for providing these functions in his tutorials, Ive updated as much as I can
bool Engine::WorldToScreen(float * from, float * to)
{
	to[0] = W2S_M.flMatrix[0][0] * from[0] + W2S_M.flMatrix[0][1] * from[1] + W2S_M.flMatrix[0][2] * from[2] + W2S_M.flMatrix[0][3];
	to[1] = W2S_M.flMatrix[1][0] * from[0] + W2S_M.flMatrix[1][1] * from[1] + W2S_M.flMatrix[1][2] * from[2] + W2S_M.flMatrix[1][3];

	float w = W2S_M.flMatrix[3][0] * from[0] + W2S_M.flMatrix[3][1] * from[1] + W2S_M.flMatrix[3][2] * from[2] + W2S_M.flMatrix[3][3];

	if(w < 0.01f)
		return false;

	float invw = 1.0f / w;
	to[0] *= invw;
	to[1] *= invw;

	int width = (int)(m_Rect.right - m_Rect.left);
	int height = (int)(m_Rect.bottom - m_Rect.top);

	float x = width/2;
	float y = height/2;

	x += 0.5 * to[0] * width + 0.5;
	y -= 0.5 * to[1] * height + 0.5;

	to[0] = x + m_Rect.left;
	to[1] = y + m_Rect.top;

	return true;
}

void Engine::DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h }; 
	FillRect(HDC_Desktop, &rect, EnemyBrush);
}

void Engine::DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	//Top horiz line
	DrawFilledRect(x, y, w, thickness);
	//Left vertical line
	DrawFilledRect( x, y, thickness, h);
	//right vertical line
	DrawFilledRect((x + w), y, thickness, h);
	//bottom horiz line
	DrawFilledRect(x, y + h, w+thickness, thickness);
}

void Engine::DrawString(int x, int y, COLORREF color, const char* text)
{	
	SetTextAlign(HDC_Desktop,TA_CENTER|TA_NOUPDATECP);

	SetBkColor(HDC_Desktop,RGB(0,0,0));
	SetBkMode(HDC_Desktop,TRANSPARENT);

	SetTextColor(HDC_Desktop,color);

	SelectObject(HDC_Desktop,Font);

	TextOutA(HDC_Desktop,x,y,text,strlen(text));

	DeleteObject(Font);
}

void Engine::DrawESP(int x, int y, float distance, int Health, int fFlags)
{
	//ESP RECTANGLE
	int width = 18100/distance;
	int height = 36000/distance;

	if(fFlags == 775)
	{
		width = 10950/distance;
	    height = 14000/distance;
	}

	DrawBorderBox(x-(width/2), y-(height+30), width, height, 2);

	std::stringstream ss, hp;
	ss << (int)distance/3;
	hp << Health;

	char * distanceInfo = new char[ss.str().size()+1];
	strcpy(distanceInfo, ss.str().c_str());

	char * HealthInfo = new char[hp.str().size()+1];
	strcpy(HealthInfo, hp.str().c_str());

	DrawString(x - 10, y-(height+50), TextCOLOR, "HP:");
	DrawString(x + 15, y-(height+50), TextCOLOR, HealthInfo);
	
	DrawString(x - 10, y-30, TextCOLOR, "Dist:");
	DrawString(x + 20, y-30, TextCOLOR, distanceInfo);

	delete [] HealthInfo, distanceInfo;

}

void Engine::ESP()
{
	if(eOn)
		for(int i = 1; i < i_Count; i ++)
		{
			float EnemyXY[3];
			
			if(EntList[i].iHealth != 0 && WorldToScreen(EntList[i].fPos, EnemyXY))
				DrawESP(EnemyXY[0] - m_Rect.left, EnemyXY[1] - m_Rect.top, Get3D(EntList[0].fPos[0], EntList[0].fPos[1], EntList[0].fPos[2], EntList[i].fPos[0], EntList[i].fPos[1], EntList[i].fPos[2]), EntList[i].iHealth, EntList[i].fFlags);	
		}
}

void Engine::CalcAngle( float *src, float *dst, float *angles , int fFlags)
{
	double delta[3] = { (src[0]-dst[0]), (src[1]-dst[1]), (src[2]-dst[2]) };
	double hyp = sqrt(delta[0]*delta[0] + delta[1]*delta[1]);
	angles[0] = (float) (asinf(delta[2]/hyp) * 57.295779513082f);
	angles[1] = (float) (atanf(delta[1]/delta[0]) * 57.295779513082f);
	angles[2] = 0.0f;

	if(delta[0] >= 0.0) 
	{ 
		angles[1] += 180.0f; 
	}

	if(fFlags == 775)
		angles[0] = angles[0] + 5; 
}

void Engine::Aimbot(){

	if(GetAsyncKeyState(0x02))
	{
		//Get Closest Entity Array Index Number
		int Index = CloseEnt();

		//Calculate Angle To Closest Entity
		CalcAngle(EntList[0].fPos, EntList[Index].fPos, EntList[Index].Angle, EntList[Index].fFlags);

		//Write To AngRotation The Current Angle Of The Entity
		mem.Write<float>(dwAngPtr + 0x4C88, EntList[Index].Angle[0]);
		mem.Write<float>(dwAngPtr + 0x4C8C, EntList[Index].Angle[1]);
		//I did call the games function SetAng Here but it has bugs, Will be fixed on next release
		//Ill also add Bone-Aimbot ( i just cba to add it all in with the bulk pointers :P
	}
	
}

//De-Constructors
Store::~Store(){
	//De-Constructor
};
Engine::~Engine()
{}
