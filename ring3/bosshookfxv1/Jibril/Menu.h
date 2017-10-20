#pragma once

#include "Includes.h"

#define CONSOLE_MAX		37

struct sMenuItem{
	std::string m_Name;
	
	union{
		float* m_Float;
		int* m_Int;
		bool* m_Bool;
	};

	float m_FloatStep;
	int m_IntStep;
	int m_Type;

	union{
		float m_FloatMin;
		int m_IntMin;
	};

	union{
		float m_FloatMax;
		int m_IntMax;
	};
};

class CMenu{
public:
	void InsertMenuItems();
	void DrawMenu();
};

enum eButtonCode
{
	KEY_NONE = 0,
	KEY_MOUSE_LEFT,
	KEY_MOUSE_RIGHT,
	KEY_INVALID,
	KEY_MOUSE_MIDDLE,
	KEY_MOUSE_4,
	KEY_MOUSE_5,
	KEY_BACKSPACE = 8,
	KEY_ENTER = 13,
	KEY_SHIFT = 16,
	KEY_CAPSTOGGLE = 20,
	KEY_ESCAPE = 27,
	KEY_SPACE = 32,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_END,
	KEY_POS1,
	KEY_ARROW_LEFT,
	KEY_ARROW_UP,
	KEY_ARROW_RIGHT,
	KEY_ARROW_DOWN,
	KEY_INSERT = 45,
	KEY_DELETE,
	KEY_0 = 48,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A = 65,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_RWIN,
	KEY_LWIN,
	KEY_SETTING,
	KEY_NUMPAD_0 = 96,
	KEY_NUMPAD_1,
	KEY_NUMPAD_2,
	KEY_NUMPAD_3,
	KEY_NUMPAD_4,
	KEY_NUMPAD_5,
	KEY_NUMPAD_6,
	KEY_NUMPAD_7,
	KEY_NUMPAD_8,
	KEY_NUMPAD_9,
	KEY_NUMPAD_MULTIPLICATE,
	KEY_NUMPAD_PLUS,
	KEY_NUMPAD_MINUS = 109,
	KEY_NUMPAD_DIVIDE = 111,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_NUMPAD_NUM = 144,
	KEY_LSHIFT = 160,
	KEY_RSHIFT,
	KEY_LSTRG,
	KEY_RSTRG,
	KEY_ALT,
	KEY_ALTGR,
	KEY_MUTE = 173,
	KEY_Ü = 186,
	KEY_PLUS,
	KEY_COMMA,
	KEY_MINUS,
	KEY_DOT,
	KEY_RUE,
	KEY_Ö,
	KEY_BACKSLAH = 219,
	KEY_ROOF,
	KEY_IRGENDWAS,
	KEY_Ä,
	KEY_MAX = 255,
};
typedef struct{
	char szDef[5];
	char szShiftDef[5];
	char szAltDef[5];
}KeyStroke_t;
extern KeyStroke_t m_KeyStroke[KEY_MAX];
struct CVar{
	CVar(std::string Name, float Value){
		szName = Name;
		flValue = Value;
	}

	float flValue;
	std::string szName;

	template <typename f>
	f GetValue(){
		return flValue;
	}
};

struct Console{
public:
	std::vector<CVar*> CVars;
	bool bActive;
	size_t nCurrentPos = 0;
	std::string szCurrentString;
	std::vector<std::string> szOutput;


	CVar* GetVar(std::string szVarName){
		int iAt = 0;
		if (szVarName.empty())
			iAt = -1;
		for (int i = 0; i < CVars.size(); i++){
			if (!CVars.at(i)->szName.compare(szVarName))
				iAt = i;
		}
		if (iAt == 0)
			iAt = -1;

		if (iAt == -1)
			return nullptr;


		return CVars.at(iAt);
	}
	
	void AddVar(std::string szVarName, float flValue){
		if (szVarName.empty())
			return;
		CVars.push_back(new CVar(szVarName, flValue));
	}
	std::vector<CVar*> autoCompleteCVars;
	std::vector<CVar*> doAutoComplete(){
		std::vector<CVar*> cVars;
		for (size_t i = 0; i < CVars.size(); ++i){
			if ((int)CVars.at(i)->szName.find(szCurrentString) == 0 && !szCurrentString.empty())
				cVars.push_back(CVars.at(i));
		}
		autoCompleteCVars = cVars;
		return cVars;
	}
	int iAutoCVar;

	void Echo(const char* pszString, ...){
		va_list args;
		va_start(args, pszString);
		char szBuffer[256];
		vsprintf_s(szBuffer, pszString, args);
		va_end(args);
		szOutput.push_back(szBuffer);
	}
	bool IsVar(std::string szVarName)
	{
		if (szVarName.empty())
			return false;

		return (GetConVarIndexByName(szVarName) != -1);
	}
	int GetConVarIndexByName(std::string szVarName)
	{
		if (szVarName.empty())
			return -1;

		for (int i = 0; i < CVars.size(); i++)
			if (!CVars.at(i)->szName.compare(szVarName))
				return i;

		return -1;
	}
	void HandleCommand(std::string szCommand){
		if (szCommand.empty())
			return;

		std::vector<std::string> szArgs;
		HandleInput(szCommand, szArgs);

		if(szArgs.size() < 1)
			return;
		if(!szArgs.at(0).compare("clear")){
			szOutput.clear();
			return;
		}
		Echo(szCommand.c_str());
		std::transform(szArgs.at(0).begin(), szArgs.at(0).end(), szArgs.at(0).begin(), ::tolower);
		if (!szArgs.at(0).compare("help"))
		{
			if (szArgs.size() == 1)
			{
				for (UINT i = 0; i < CVars.size(); i++)
				{
					CVar* pConVar = CVars.at(i);
					Echo("%s = %.2f", pConVar->szName.c_str(), pConVar->flValue);
				}
			}
			else
			{
				std::transform(szArgs.at(1).begin(), szArgs.at(1).end(), szArgs.at(1).begin(), ::tolower);
				if (IsVar(szArgs.at(1)))
				{
					CVar* pConVar = GetVar(szArgs.at(1));
					Echo("%s = %.2f", pConVar->szName.c_str(), pConVar->flValue);
				}
				else Echo("Unknown command: %s", szArgs.at(0).c_str());
			}
			return;
		}
		else if (IsVar(szArgs.at(0)))
		{
			CVar* pConVar = GetVar(szArgs.at(0));

			if (szArgs.size() == 1)
				Echo("%s = %.2f", pConVar->szName.c_str(), pConVar->flValue);
			else
			{
				if (isdigit(szArgs.at(1).c_str()[0]))
				{
					float flValue = 0.f;
					if (szArgs.at(1).find(".") != std::string::npos)
					{
						flValue = std::strtof(szArgs.at(1).c_str(), NULL);
						pConVar->flValue = flValue;

						Echo("Set %s to %.2f", pConVar->szName.c_str(), pConVar->flValue);
					}
					else
					{
						flValue = (float)std::atoi(szArgs.at(1).c_str());
						pConVar->flValue = flValue;

						Echo("Set %s to %d", pConVar->szName.c_str(), pConVar->flValue);
					}
					return;
				}
				else if (szArgs.size() > 1 && !szArgs.at(1).empty())
				{
					std::transform(szArgs.at(1).begin(), szArgs.at(1).end(), szArgs.at(1).begin(), ::tolower);
					bool bIsTrue = !szArgs.at(1).compare("true") ? true : false;
					bool bIsFalse = !szArgs.at(1).compare("false") ? true : false;

					if (bIsFalse || bIsTrue)
					{
						if (bIsFalse)
							pConVar->flValue = false;
						else pConVar->flValue = true;

						Echo("%s %s", bIsTrue ? "Enabled" : "Disabled", pConVar->szName.c_str());
					}
					else Echo("Unknown argument: %s", szArgs.at(1).c_str());
				}
			}
		}
		else Echo("Unknown command: %s", szArgs.at(0).c_str());
	}

	void HandleInput(std::string szInput, std::vector<std::string>& szBuffer){
		size_t nPos = 0;
		while (true){
			nPos = szInput.find_first_of(" \n\0");
			if (nPos != std::string::npos){
				szBuffer.push_back(szInput.substr(0, nPos));
				szInput.erase(0, nPos + 1);
			} 
			else{
				if (!szInput.empty())
					szBuffer.push_back(szInput);
				break;
			}
		}
	}
	void HandleInput(){
		if (GetForegroundWindow() == FindWindow(0, "Counter-Strike: Global Offensive")){
			for (int i = 0; i < KEY_MAX; i++){
				if (GetAsyncKeyState(KEY_BACKSPACE) & 1){
					size_t nLengh = szCurrentString.length();
					if (nLengh > 0){
						szCurrentString.erase(nLengh - 1);
						nCurrentPos--;
					}
				}
				if (GetAsyncKeyState(VK_DOWN))
					iAutoCVar++;
				if (GetAsyncKeyState(VK_UP))
					iAutoCVar--;
				if (GetAsyncKeyState(VK_TAB))
					if (!autoCompleteCVars.empty())
						szCurrentString = autoCompleteCVars.at(iAutoCVar)->szName + " ";
				if (GetAsyncKeyState(KEY_ENTER))
					if (!szCurrentString.empty()){
						HandleCommand(szCurrentString);
						nCurrentPos = 0;
						szCurrentString.clear();
						iAutoCVar = 0;
						break;
					}
				if (GetAsyncKeyState(i) & 1 && szCurrentString.length() <= 150){
					if (GetAsyncKeyState(KEY_SHIFT) ){
						szCurrentString.append(m_KeyStroke[i].szShiftDef);
						nCurrentPos++;
					}
					else if (GetAsyncKeyState(KEY_ALTGR) ){
						szCurrentString.append(m_KeyStroke[i].szAltDef);
						nCurrentPos++;
					}
					else{
						szCurrentString.append(m_KeyStroke[i].szDef);
						nCurrentPos++;
					}
				}
			}
		}
	}
	void Render(unsigned long font, int m_x, int m_y, int m_w, int m_h);
};


extern Console* g_pConsole;