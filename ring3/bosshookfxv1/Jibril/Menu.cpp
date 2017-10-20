#include "Menu.h"

KeyStroke_t m_KeyStroke[KEY_MAX] =
{
	{ "", "", "" }, // 0: KEY_NONE
	{ "", "", "" }, // 1: KEY_MOUSE_LEFT
	{ "", "", "" }, // 2: KEY_MOUSE_RIGHT
	{ "", "", "" }, // 3: KEY_MOUSE_INVALID
	{ "", "", "" }, // 4: KEY_MOUSE_MIDDLE
	{ "", "", "" }, // 5: KEY_MOUSE_4
	{ "", "", "" }, // 6: KEY_MOUSE_5
	{ "", "", "" }, // 7:
	{ "", "", "" }, // 8: KEY_BACKSPACE
	{ "", "", "" }, // 9:
	{ "", "", "" }, // 10:
	{ "", "", "" }, // 11:
	{ "", "", "" }, // 12:
	{ "", "", "" }, // 13: KEY_ENTER
	{ "", "", "" }, // 14:
	{ "", "", "" }, // 15:
	{ "", "", "" }, // 16: KEY_SHIFT
	{ "", "", "" }, // 17:
	{ "", "", "" }, // 18:
	{ "", "", "" }, // 19:
	{ "", "", "" }, // 20: KEY_CAPSTOGGLE
	{ "", "", "" }, // 21:
	{ "", "", "" }, // 22:
	{ "", "", "" }, // 23:
	{ "", "", "" }, // 24:
	{ "", "", "" }, // 25:
	{ "", "", "" }, // 26:
	{ "", "", "" }, // 27: KEY_ESCAPE
	{ "", "", "" }, // 28:
	{ "", "", "" }, // 29:
	{ "", "", "" }, // 30:
	{ "", "", "" }, // 31:
	{ " ", " ", " " }, // 32: KEY_SPACE
	{ "", "", "" }, // 33: KEY_PAGE_UP
	{ "", "", "" }, // 34: KEY_PAGE_DOWN
	{ "", "", "" }, // 35: KEY_END
	{ "", "", "" }, // 36: KEY_POS1
	{ "", "", "" }, // 37: KEY_ARROW_LEFT
	{ "", "", "" }, // 38: KEY_ARROW_UP
	{ "", "", "" }, // 39: KEY_ARROW_RIGHT
	{ "", "", "" }, // 40: KEY_ARROW_DOWN
	{ "", "", "" }, // 41:
	{ "", "", "" }, // 42:
	{ "", "", "" }, // 43:
	{ "", "", "" }, // 44:
	{ "", "", "" }, // 45: KEY_INSERT
	{ "", "", "" }, // 46: KEY_DELETE
	{ "", "", "" }, // 47:
	{ "0", "=", "}" }, // 48: KEY_0
	{ "1", "!", "" }, // 49: KEY_1
	{ "2", "'", "²" }, // 50: KEY_2
	{ "3", "§", "³" }, // 51: KEY_3
	{ "4", "$", "" }, // 52: KEY_4
	{ "5", "%", "" }, // 53: KEY_5
	{ "6", "&", "" }, // 54: KEY_6
	{ "7", "/", "{" }, // 55: KEY_7
	{ "8", "(", "[" }, // 56: KEY_8
	{ "9", ")", "]" }, // 57: KEY_9
	{ "", "", "" }, // 58:
	{ "", "", "" }, // 59:
	{ "", "", "" }, // 60:
	{ "", "", "" }, // 61:
	{ "", "", "" }, // 62:
	{ "", "", "" }, // 63:
	{ "", "", "" }, // 64:
	{ "a", "A", "" }, // 65: KEY_A
	{ "b", "B", "" }, // 66: KEY_B
	{ "c", "C", "" }, // 67: KEY_C
	{ "d", "D", "" }, // 68: KEY_D
	{ "e", "E", "€" }, // 69: KEY_E
	{ "f", "F", "" }, // 70: KEY_F
	{ "g", "G", "" }, // 71: KEY_G
	{ "h", "H", "" }, // 72: KEY_H
	{ "i", "I", "" }, // 73: KEY_I
	{ "j", "J", "" }, // 74: KEY_J
	{ "k", "K", "" }, // 75: KEY_K
	{ "l", "L", "" }, // 76: KEY_L
	{ "m", "M", "µ" }, // 77: KEY_M
	{ "n", "N", "" }, // 78: KEY_N
	{ "o", "O", "" }, // 79: KEY_O
	{ "p", "P", "" }, // 80: KEY_P
	{ "q", "Q", "@" }, // 81: KEY_Q
	{ "r", "R", "" }, // 82: KEY_R
	{ "s", "S", "" }, // 83: KEY_S
	{ "t", "T", "" }, // 84: KEY_T
	{ "u", "U", "" }, // 85: KEY_U
	{ "v", "V", "" }, // 86: KEY_V
	{ "w", "W", "" }, // 87: KEY_W
	{ "x", "X", "" }, // 88: KEY_X
	{ "y", "Y", "" }, // 89: KEY_Y
	{ "z", "Z", "" }, // 90: KEY_Z
	{ "", "", "" }, // 91: KEY_RWIN
	{ "", "", "" }, // 92: KEY_LWIN
	{ "", "", "" }, // 93: KEY_SETTINGS
	{ "", "", "" }, // 94:
	{ "", "", "" }, // 95:
	{ "0", "", "" }, // 96: KEY_NUMPAD_0
	{ "1", "", "" }, // 97: KEY_NUMPAD_1
	{ "2", "", "" }, // 98: KEY_NUMPAD_2
	{ "3", "", "" }, // 99: KEY_NUMPAD_3
	{ "4", "", "" }, // 100: KEY_NUMPAD_4
	{ "5", "", "" }, // 101: KEY_NUMPAD_5
	{ "6", "", "" }, // 102: KEY_NUMPAD_6
	{ "7", "", "" }, // 103: KEY_NUMPAD_7
	{ "8", "", "" }, // 104: KEY_NUMPAD_8
	{ "9", "", "" }, // 105: KEY_NUMPAD_9
	{ "*", "", "" }, // 106: KEY_NUMPAD_MULTIPLICATE
	{ "+", "", "" }, // 107: KEY_NUMPAD_PLUS
	{ "", "", "" }, // 108:
	{ "-", "", "" }, // 109: KEY_NUMPAD_MINUS
	{ "", "", "" }, // 110:
	{ "/", "", "" }, // 111: KEY_NUMPAD_DIVIDE
	{ "", "", "" }, // 112: KEY_F1
	{ "", "", "" }, // 113: KEY_F2
	{ "", "", "" }, // 114: KEY_F3
	{ "", "", "" }, // 115: KEY_F4
	{ "", "", "" }, // 116: KEY_F5
	{ "", "", "" }, // 117: KEY_F6
	{ "", "", "" }, // 118: KEY_F7
	{ "", "", "" }, // 119: KEY_F8
	{ "", "", "" }, // 120: KEY_F9
	{ "", "", "" }, // 121: KEY_F10
	{ "", "", "" }, // 122: KEY_F11
	{ "", "", "" }, // 123: KEY_F12
	{ "", "", "" }, // 124:
	{ "", "", "" }, // 125:
	{ "", "", "" }, // 126:
	{ "", "", "" }, // 127:
	{ "", "", "" }, // 128:
	{ "", "", "" }, // 129:
	{ "", "", "" }, // 130:
	{ "", "", "" }, // 131:
	{ "", "", "" }, // 132:
	{ "", "", "" }, // 133:
	{ "", "", "" }, // 134:
	{ "", "", "" }, // 135:
	{ "", "", "" }, // 136:
	{ "", "", "" }, // 137:
	{ "", "", "" }, // 138:
	{ "", "", "" }, // 139:
	{ "", "", "" }, // 140:
	{ "", "", "" }, // 141:
	{ "", "", "" }, // 142:
	{ "", "", "" }, // 143:
	{ "", "", "" }, // 144: KEY_NUMPAD_NUM
	{ "", "", "" }, // 145:
	{ "", "", "" }, // 146:
	{ "", "", "" }, // 147:
	{ "", "", "" }, // 148:
	{ "", "", "" }, // 149:
	{ "", "", "" }, // 150:
	{ "", "", "" }, // 151:
	{ "", "", "" }, // 152:
	{ "", "", "" }, // 153:
	{ "", "", "" }, // 154:
	{ "", "", "" }, // 155:
	{ "", "", "" }, // 156:
	{ "", "", "" }, // 157:
	{ "", "", "" }, // 158:
	{ "", "", "" }, // 159:
	{ "", "", "" }, // 160: KEY_LSHIFT
	{ "", "", "" }, // 161: KEY_RSHIFT
	{ "", "", "" }, // 162: KEY_LSTRG
	{ "", "", "" }, // 163: KEY_RSTRG
	{ "", "", "" }, // 164: KEY_ALT
	{ "", "", "" }, // 165: KEY_ALTGR
	{ "", "", "" }, // 166:
	{ "", "", "" }, // 167:
	{ "", "", "" }, // 168:
	{ "", "", "" }, // 169:
	{ "", "", "" }, // 170:
	{ "", "", "" }, // 171:
	{ "", "", "" }, // 172:
	{ "", "", "" }, // 173: KEY_MUTE
	{ "", "", "" }, // 174:
	{ "", "", "" }, // 175:
	{ "", "", "" }, // 176:
	{ "", "", "" }, // 177:
	{ "", "", "" }, // 178:
	{ "", "", "" }, // 179:
	{ "", "", "" }, // 180:
	{ "", "", "" }, // 181:
	{ "", "", "" }, // 182:
	{ "", "", "" }, // 183:
	{ "", "", "" }, // 184:
	{ "", "", "" }, // 185:
	{ "", "", "" }, // 186: KEY_Ü
	{ "+", "*", "~" }, // 187: KEY_PLUS
	{ ",", ";", "" }, // 188: KEY_COMMA
	{ "-", "_", "" }, // 189: KEY_MINUS
	{ ".", ":", "" }, // 190: KEY_DOT
	{ "#", "'", "" }, // 191: KEY_RUE
	{ "", "", "" }, // 192: KEY_Ö
	{ "", "", "" }, // 193:
	{ "", "", "" }, // 194:
	{ "", "", "" }, // 195:
	{ "", "", "" }, // 196:
	{ "", "", "" }, // 197:
	{ "", "", "" }, // 198:
	{ "", "", "" }, // 199:
	{ "", "", "" }, // 200:
	{ "", "", "" }, // 201:
	{ "", "", "" }, // 202:
	{ "", "", "" }, // 203:
	{ "", "", "" }, // 204:
	{ "", "", "" }, // 205:
	{ "", "", "" }, // 206:
	{ "", "", "" }, // 207:
	{ "", "", "" }, // 208:
	{ "", "", "" }, // 209:
	{ "", "", "" }, // 210:
	{ "", "", "" }, // 211:
	{ "", "", "" }, // 212:
	{ "", "", "" }, // 213:
	{ "", "", "" }, // 214:
	{ "", "", "" }, // 215:
	{ "", "", "" }, // 216:
	{ "", "", "" }, // 217:
	{ "", "", "" }, // 218:
	{ "sz", "?", "\\" }, // 219: KEY_BACKSLASH
	{ "^", "°", "" }, // 220: KEY_ROOF
	{ "´", "`", "" }, // 221: KEY_IRGENDWAS
	{ "", "", "" }, // 222: KEY_Ä
	{ "", "", "" }, // 223:
	{ "", "", "" }, // 224:
	{ "", "", "" }, // 225:
	{ "", "", "" }, // 226:
	{ "", "", "" }, // 227:
	{ "", "", "" }, // 228:
	{ "", "", "" }, // 229:
	{ "", "", "" }, // 230:
	{ "", "", "" }, // 231:
	{ "", "", "" }, // 232:
	{ "", "", "" }, // 233:
	{ "", "", "" }, // 234:
	{ "", "", "" }, // 235:
	{ "", "", "" }, // 236:
	{ "", "", "" }, // 237:
	{ "", "", "" }, // 238:
	{ "", "", "" }, // 239:
	{ "", "", "" }, // 240:
	{ "", "", "" }, // 241:
	{ "", "", "" }, // 242:
	{ "", "", "" }, // 243:
	{ "", "", "" }, // 244:
	{ "", "", "" }, // 245:
	{ "", "", "" }, // 246:
	{ "", "", "" }, // 247:
	{ "", "", "" }, // 248:
	{ "", "", "" }, // 249:
	{ "", "", "" }, // 250:
	{ "", "", "" }, // 251:
	{ "", "", "" }, // 252:
	{ "", "", "" }, // 253:
	{ "", "", "" }, // 254:
};



sMenuItem Items[120];
bool AimbotTab = false, TriggerbotTab = false, ESPTab = false, MiscTab = false, AntiAimTab = false;
int TotalItems = 0, CurrentPos = 0, MenuX = 250, MenuY = 250;

int AddMenuEntry(int n, std::string name, bool* value)
{
	Items[n].m_Name = name;
	Items[n].m_Bool = value;
	Items[n].m_Type = 0;

	return (n + 1);
}

int AddMenuEntry(int n, std::string name, int* value, int min, int max, int step)
{
	Items[n].m_Name = name;
	Items[n].m_Int = value;
	Items[n].m_IntMin = min;
	Items[n].m_IntMax = max;
	Items[n].m_IntStep = step;
	Items[n].m_Type = 1;

	return (n + 1);
}

int AddMenuEntry(int n, std::string name, float* value, float min, float max, float step)
{
	Items[n].m_Name = name;
	Items[n].m_Float = value;
	Items[n].m_FloatMin = min;
	Items[n].m_FloatMax = max;
	Items[n].m_FloatStep = step;
	Items[n].m_Type = 2;

	return (n + 1);
}

void CMenu::InsertMenuItems()
{
	int i = 0;

	if (!AimbotTab)
		i = AddMenuEntry(i, "[+] AIM", &AimbotTab);

	if (AimbotTab)
	{
		i = AddMenuEntry(i, "[-] AIM", &AimbotTab);

		i = AddMenuEntry(i, "Aim_Enable", &Interface.Settings->aim);
		//i = AddMenuEntry(i, "Aim_Key", &Interface.Settings->a_key, 1, 0, 166);
		i = AddMenuEntry(i, "Aim_FOV", &Interface.Settings->fov, 0.f, 180.f, .1f);
		i = AddMenuEntry(i, "Aim_Smooth", &Interface.Settings->smooth, 0.f, 100.f, .1f);
		i = AddMenuEntry(i, "Aim_Spot", &Interface.Settings->bone, 0, 19, 1);
		i = AddMenuEntry(i, "Aim_AutoShoot", &Interface.Settings->autoshoot);
		i = AddMenuEntry(i, "Aim_Silent", &Interface.Settings->silentaim);
		i = AddMenuEntry(i, "Aim_Auto_Pistol", &Interface.Settings->autopistol);
		i = AddMenuEntry(i, "Aim_RCS", &Interface.Settings->rcs);
		i = AddMenuEntry(i, "Aim_RCS_X", &Interface.Settings->intens_x, 0.f, 100.f, 1.f);
		i = AddMenuEntry(i, "Aim_RCS_Y", &Interface.Settings->intens_y, 0.f, 100.f, 1.f);
	}

	if (!TriggerbotTab)
		i = AddMenuEntry(i, "[+] TRIGGERBOT", &TriggerbotTab);

	if (TriggerbotTab)
	{
		i = AddMenuEntry(i, "[-] TRIGGERBOT", &TriggerbotTab);

		i = AddMenuEntry(i, "Active", &Interface.Settings->trigger);
		//i = AddMenuEntry(i, "Key", &Interface.Settings->t_key, 6, 0, 166);
		//i = AddMenuEntry(i, "Key", &Interface.Settings->t_key, 1, 1, 166); // default by n0xius
		i = AddMenuEntry(i, "Head", &Interface.Settings->head);
		i = AddMenuEntry(i, "Neck", &Interface.Settings->neck);
		i = AddMenuEntry(i, "Body", &Interface.Settings->body);
		i = AddMenuEntry(i, "Arms", &Interface.Settings->arms);
		i = AddMenuEntry(i, "Legs", &Interface.Settings->legs);
	}

	if (!ESPTab)
		i = AddMenuEntry(i, "[+] VISUALS", &ESPTab);

	if (ESPTab)
	{
		i = AddMenuEntry(i, "[-] VISUALS", &ESPTab);

		i = AddMenuEntry(i, "ESP_Enable", &Interface.Settings->esp);
		i = AddMenuEntry(i, "ESP_Legit", &Interface.Settings->legit);
		i = AddMenuEntry(i, "ESP_Box", &Interface.Settings->box);
		i = AddMenuEntry(i, "ESP_Glow", &Interface.Settings->glow);
		i = AddMenuEntry(i, "ESP_Defuse Glow", &Interface.Settings->defuseglow);
		i = AddMenuEntry(i, "ESP_Skeleton", &Interface.Settings->skeleton);
		i = AddMenuEntry(i, "ESP_Name", &Interface.Settings->name);
		i = AddMenuEntry(i, "ESP_Healthbar", &Interface.Settings->healthbar);
		i = AddMenuEntry(i, "ESP_Health text", &Interface.Settings->health);
		i = AddMenuEntry(i, "ESP_Weapon", &Interface.Settings->weapon);
		i = AddMenuEntry(i, "ESP_Rank", &Interface.Settings->rank);
		i = AddMenuEntry(i, "ESP_Hitbox Dot", &Interface.Settings->hitboxdot);
		i = AddMenuEntry(i, "ESP_Entity", &Interface.Settings->weapon_esp);
		//i = AddMenuEntry(i, "ESP_Chams", &Interface.Settings->chams);
	}

	if (!MiscTab)
		i = AddMenuEntry(i, "[+] MISC", &MiscTab);

	if (MiscTab)
	{
		i = AddMenuEntry(i, "[-] MISC", &MiscTab);

		i = AddMenuEntry(i, "MISC_NoRecoil", &Interface.Settings->norecoil);
		i = AddMenuEntry(i, "MISC_AutoDuck", &Interface.Settings->autoduck);
		i = AddMenuEntry(i, "MISC_Bunny", &Interface.Settings->bhop);
		i = AddMenuEntry(i, "MISC_RecoilDot", &Interface.Settings->rdot);
		i = AddMenuEntry(i, "MISC_Crosshair", &Interface.Settings->crosshair);
		i = AddMenuEntry(i, "MISC_Weapon Configs", &Interface.Settings->weaponcfg);
	}

	TotalItems = i;
}

void CMenu::DrawMenu()
{
	int FontSize = 13;

	//FillRGBA(MenuX - 20, MenuY + 1 + (FontSize * CurrentPos) + 4, 215, FontSize + 2, 255, 255, 255, 255);
	FillRGBA(MenuX - 20, MenuY + 2 + (FontSize * CurrentPos) + 3, 215, FontSize + 2, 255, 255, 255, 255);

	for (int i = 0; i < TotalItems; ++i)
	{
		if (!strncmp(Items[i].m_Name.c_str(), "[+]", 3) || !strncmp(Items[i].m_Name.c_str(), "[-]", 3)) // ghetto tab check
			Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
		else{
			switch (Items[i].m_Type)
			{
			case 0:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%s", (*Items[i].m_Bool) ? "true" : "false");
				break;
			case 1:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%i", *Items[i].m_Int);
				break;
			case 2:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%.2f", *Items[i].m_Float);
				break;
			default:
				break;
			}
		}
	}

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		if (CurrentPos > 0)
			CurrentPos--;
		else
			CurrentPos = TotalItems - 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		if (CurrentPos < TotalItems - 1)
			CurrentPos++;
		else
			CurrentPos = 0;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		switch (Items[CurrentPos].m_Type)
		{
		case 0:
			*Items[CurrentPos].m_Bool = !*Items[CurrentPos].m_Bool;
			break;
		case 1:
			*Items[CurrentPos].m_Int -= Items[CurrentPos].m_IntStep;
			if (*Items[CurrentPos].m_Int < Items[CurrentPos].m_IntMin)
				*Items[CurrentPos].m_Int = Items[CurrentPos].m_IntMax;
			break;
		case 2:
			*Items[CurrentPos].m_Float -= Items[CurrentPos].m_FloatStep;
			if (*Items[CurrentPos].m_Float < Items[CurrentPos].m_FloatMin)
				*Items[CurrentPos].m_Float = Items[CurrentPos].m_FloatMax;
			break;
		default:
			break;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		switch (Items[CurrentPos].m_Type)
		{
		case 0:
			*Items[CurrentPos].m_Bool = !*Items[CurrentPos].m_Bool;
			break;
		case 1:
			*Items[CurrentPos].m_Int += Items[CurrentPos].m_IntStep;
			if (*Items[CurrentPos].m_Int > Items[CurrentPos].m_IntMax)
				*Items[CurrentPos].m_Int = Items[CurrentPos].m_IntMin;
			break;
		case 2:
			*Items[CurrentPos].m_Float += Items[CurrentPos].m_FloatStep;
			if (*Items[CurrentPos].m_Float > Items[CurrentPos].m_FloatMax)
				*Items[CurrentPos].m_Float = Items[CurrentPos].m_FloatMin;
			break;
		default:
			break;
		}
	}
}



void Console::Render(unsigned long font, int m_x, int m_y, int m_w, int m_h){
	if (!bActive)
		return;
	int x = m_x;
	int y = m_y;
	int w = m_w;

	int h = m_h;


	FillRGBA(x - 2, y - 2, w + 4, h + 4, 48, 49, 50, 255);

	FillRGBA(x, y, w, h, 18, 19, 20, 230);

	y -= 29;
	x += 5;
	y += 35;
	w -= 10;
	h += 29;
	h -= 65;


	FillRGBA(x - 2, y - 2, w + 4, h + 4, 48, 49, 50, 255);

	FillRGBA(x, y, w, h, 34, 35, 36, 255);
	x += 5;
	y += 5;
	int iHistoryY = y;
	if (szOutput.size() < CONSOLE_MAX ? true : false)
	{
		for (UINT i = 0; i < szOutput.size(); i++)
		{
			iHistoryY += 2;
			Text(x, iHistoryY + (i * 12), 200, 200, 200, 255, false, font, szOutput.at(i).c_str());

		}
	}
	else
	{
		for (UINT i = 0; i < CONSOLE_MAX; i++)
		{
			iHistoryY += 2;
			Text(x, iHistoryY + (i * 12), 200, 200, 200, 255, false, font, szOutput.at(i + szOutput.size() - CONSOLE_MAX).c_str());
		}
	}
	x -= 5;
	y += h;
	h = 20;
	FillRGBA(x - 2, y - 2, w + 4, h + 4, 48, 49, 50, 255);
	FillRGBA(x, y, w, h, 34, 35, 36, 255);
	x += 5;
	y += 3;
	Text(x, y, 200, 200, 200, 255, false, font, szCurrentString.c_str());

	autoCompleteCVars = doAutoComplete();
	for (int i = 0; i < autoCompleteCVars.size(); ++i){
		if (iAutoCVar > autoCompleteCVars.size() - 1)
			iAutoCVar = 0;
		if (iAutoCVar < 0)
			iAutoCVar = autoCompleteCVars.size() - 1;
		w = 200;
		h = 12;
		FillRGBA(x - 1, y + 14 + (i * 14) - 1, w + 2, h + 2, 0, 0, 0, 230);

		if (iAutoCVar == i)
			FillRGBA(x, y + 14 + (i * 14), w, h, 150, 150, 150, 230);
		else
			FillRGBA(x, y + 14 + (i * 14), w, h, 40, 40, 40, 230);
		Text(x, y + 12 + (i * 14), 255, 255, 255, 255, false, font, autoCompleteCVars.at(i)->szName.c_str());
	}
}


Console* g_pConsole = new Console;