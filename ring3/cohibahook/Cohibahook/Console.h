#pragma once

#define SetCvar( cvar, value ) int cvar_##cvar = value;



struct cvar_s
{
	int esp_box;
	int esp_name;
	int esp_health;
	int esp_glow;
	int esp_crosshair;
	int menu_color_r;
	int menu_color_g;
	int menu_color_b;
	int font_color_r;
	int font_color_g;
	int font_color_b;
};


typedef struct Key_s
{
	char m_def[16];
	char m_alt[16];
} Key_t;


static Key_t gKeyMap[MOUSE_COUNT + KEY_COUNT] = {
	{ "", "" },
	{ "0", ")" },			//KEY_0,
	{ "1", "!" },			//KEY_1,
	{ "2", "\"" },			//KEY_2,
	{ "3", "£" },			//KEY_3,
	{ "4", "$" },			//KEY_4,
	{ "5", "%" },			//KEY_5,
	{ "6", "^" },			//KEY_6,
	{ "7", "&" },			//KEY_7,
	{ "8", "*" },			//KEY_8,
	{ "9", "(" },			//KEY_9,
	{ "a", "A" },			//KEY_A,
	{ "b", "B" },			//KEY_B,
	{ "c", "C" },			//KEY_C,
	{ "d", "D" },			//KEY_D,
	{ "e", "E" },			//KEY_E,
	{ "f", "F" },			//KEY_F,
	{ "g", "G" },			//KEY_G,
	{ "h", "H" },			//KEY_H,
	{ "i", "I" },			//KEY_I,
	{ "j", "J" },			//KEY_J,
	{ "k", "K" },			//KEY_K,
	{ "l", "L" },			//KEY_L,
	{ "m", "M" },			//KEY_M,
	{ "n", "N" },			//KEY_N,
	{ "o", "O" },			//KEY_O,
	{ "p", "P" },			//KEY_P,
	{ "q", "Q" },			//KEY_Q,
	{ "r", "R" },			//KEY_R,
	{ "s", "S" },			//KEY_S,
	{ "t", "T" },			//KEY_T,
	{ "u", "U" },			//KEY_U,
	{ "v", "V" },			//KEY_V,
	{ "w", "W" },			//KEY_W,
	{ "x", "X" },			//KEY_X,
	{ "y", "Y" },			//KEY_Y,
	{ "z", "|" },			//KEY_Z,
	{ "0", "0" },			//KEY_PAD_0,
	{ "1", "1" },			//KEY_PAD_1,
	{ "2", "2" },			//KEY_PAD_2,
	{ "3", "3" },			//KEY_PAD_3,
	{ "4", "4" },			//KEY_PAD_4,
	{ "5", "5" },			//KEY_PAD_5,
	{ "6", "6" },			//KEY_PAD_6,
	{ "7", "7" },			//KEY_PAD_7,
	{ "8", "8" },			//KEY_PAD_8,
	{ "9", "9" },			//KEY_PAD_9,
	{ "/", "/" },			//KEY_PAD_DIVIDE,
	{ "*", "*" },			//KEY_PAD_MULTIPLY,
	{ "-", "-" },			//KEY_PAD_MINUS,
	{ "+", "+" },			//KEY_PAD_PLUS,
	{ "enter", "enter" },				//KEY_PAD_ENTER,
	{ ".", ">" },				//KEY_PAD_DECIMAL,
	{ "[", "{" },				//KEY_LBRACKET,
	{ "]", "}" },				//KEY_RBRACKET,
	{ ";", ":" },				//KEY_SEMICOLON,
	{ "'", "@" },				//KEY_APOSTROPHE,
	{ "`", "¬" },				//KEY_BACKQUOTE,
	{ ",", "<" },				//KEY_COMMA,
	{ ".", ">" },				//KEY_PERIOD,
	{ "/", "?" },				//KEY_SLASH,
	{ "\\", "|" },				//KEY_BACKSLASH,
	{ "-", "_" },				//KEY_MINUS,
	{ "=", "+" },				//KEY_EQUAL,
	{ "enter", "enter" },				//KEY_ENTER,
	{ "space", "space" },				//KEY_SPACE,
	{ "backspace", "backspace" },				//KEY_BACKSPACE,
	{ "tab", "tab" },				//KEY_TAB,
	{ "caps", "caps" },				//KEY_CAPSLOCK,
	{ "numlock", "numlock" },				//KEY_NUMLOCK,
	{ "escape", "escape" },				//KEY_ESCAPE,
	{ "scrlock", "scrlock" },				//KEY_SCROLLLOCK,
	{ "insert", "insert" },				//KEY_INSERT,
	{ "delete", "delete" },				//KEY_DELETE,
	{ "home", "home" },				//KEY_HOME,
	{ "end", "end" },				//KEY_END,
	{ "pageup", "pageup" },				//KEY_PAGEUP,
	{ "pagedown", "pagedown" },				//KEY_PAGEDOWN,
	{ "break", "break" },				//KEY_BREAK,
	{ "lshift", "lshift" },				//KEY_LSHIFT,
	{ "rshift", "rshift" },				//KEY_RSHIFT,
	{ "lalt", "lalt" },				//KEY_LALT,
	{ "ralt", "ralt" },				//KEY_RALT,
	{ "lctrl", "lctrl" },				//KEY_LCONTROL,
	{ "rctrl", "rctrl" },				//KEY_RCONTROL,
	{ "lwin", "lwin" },				//KEY_LWIN,
	{ "rwin", "rwin" },				//KEY_RWIN,
	{ "app", "app" },				//KEY_APP,
	{ "up", "up" },				//KEY_UP,
	{ "left", "left" },				//KEY_LEFT,
	{ "down", "down" },				//KEY_DOWN,
	{ "right", "right" },				//KEY_RIGHT,
	{ "f1", "f1" },				//KEY_F1,
	{ "f2", "f2" },				//KEY_F2,
	{ "f3", "f3" },				//KEY_F3,
	{ "f4", "f4" },				//KEY_F4,
	{ "f5", "f5" },				//KEY_F5,
	{ "f6", "f6" },				//KEY_F6,
	{ "f7", "f7" },				//KEY_F7,
	{ "f8", "f8" },				//KEY_F8,
	{ "f9", "f9" },				//KEY_F9,
	{ "f10", "f10" },			//KEY_F10,
	{ "f11", "f11" },			//KEY_F11,
	{ "f12", ")" },				//KEY_F12,
	{ "capstoggle", "capstoggle" },				//KEY_CAPSLOCKTOGGLE,
	{ "numlocktoggle", "numlocktoggle" },				//KEY_NUMLOCKTOGGLE,
	{ "scrlocktoggle", "scrlocktoggle" },				//KEY_SCROLLLOCKTOGGLE,

	//// Mouse
	{ "mouse1", "mouse1" },				//MOUSE_LEFT = MOUSE_FIRST,
	{ "mouse2", "mouse2" },				//MOUSE_RIGHT,
	{ "mouse3", "mouse3" },				//MOUSE_MIDDLE,
	{ "mouse4", "mouse4" },				//MOUSE_4,
	{ "mouse5", "mouse5" },				//MOUSE_5,
	{ "mwheelup", "mwheelup" },				//MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	{ "mwheeldown", "mwheeldown" },				//MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down
};


class CRetVar
{
public:
	CRetVar(std::string szName, int iValue, bool bStyle)
	{
		this->szName = szName;
		this->iValue = iValue;
		this->bStyle = bStyle;
	}

	std::string szName;
	int iValue;
	bool bPrinted;
	bool bSelected;
	bool bStyle;
};


struct COLOR
{
	int r;
	int g;
	int b;
	int a;
};

class CConsole
{
public:
	std::string command;

	void RenderConsole();
	void HandleKeys(int key);
	void HandleCommands(std::string szCommand);
	bool IsActive();
	void print(int r, int g, int b, const char* szInput, ...);
	void InitCommands();
	void addCvar(char* szName, int iValue, bool bStyle);
	int getValue(const char* szName);
	void readConfig(const char* szName);
	void initCommands();
	std::vector<CRetVar> vars;
	HINSTANCE hMod;
	int triggerKey;
private:
	std::vector<std::string> output, autoComplete;
	std::vector<COLOR> outputColor;
	int counter, selected, iPrintCounter;
	std::string buf;
	BOOL bActive, bComplete, bCfgload;
	int selbuf;
};

extern CConsole* g_pConsole;