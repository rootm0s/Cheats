#include "DLLMain.h"

#ifndef CMENU_H
#define CMENU_H
/* CheckBox Count	*/   #define CheckBox_Count	15
struct CheckBox_t
{
	bool State, IsClicked;
};

class cGui
{
public:
	CheckBox_t CheckBox[CheckBox_Count];
	void AddCheck(float* value, int x, int y);
	void DrawMenu(int, int, int, int, char*);
	void DrawStats(int, int, int, int, char*);
	void DrawRadar(int, int, int, char*);
	void DrawConsole(int, int, int, int, char*);
	void Title(int, int, int, int, char*);
	void ShowFPS(void);
	void Body(int, int, int, int);
	void MenuEntry(int*, int, int, char*, char*);
	void PlusMinus(float*, int, int, char*, char*);
	void LessMore(int*, int, int, char*, char*, int limit);
	void Body2(int, int, int, int);
	void Body3(int, int, int, int);
	void MenuEntry3(int*, int, int, char*, char*);
	void MenuEntry2(int*, int, int, char*, char*);
	void PlusMinus2(char *var, int x, int y, char *name, char *desc);
	void PlusMinus3(float*, int, int, char*, char*);
	void LessMore2(char *var, int x, int y, char *name, char *desc, int limit);
	void LessMore3(int*, int, int, char*, char*, int limit);
	void blackBorder(int x, int y, int w, int h);
	void MenuButtons();
	void MenuButtons3();
	void MenuPages();
	void MenuPages3();
	void MenuButtons2();
	void MenuPages2();
	void SaveConfig1();
	void SaveConfig2();
	void LoadConfig1();
	void LoadConfig2();
	void GetIniFile(HMODULE hModule);
	int legitbuttons;
	int ragebuttons;
	int tabbackgroundX;
	int tabbackgroundY;
	void DrawMouse();
	void GetMouseClic();
	bool MouseOver(int x, int y, int w, int h);
	bool MouseLeftClic(int x, int y, int w, int h);
	bool cursoractive;
	int cursorpos[2];

	bool leftclic, rightclic, leftclicreleased, rightclicreleased;
};
extern cGui gGui;
#endif