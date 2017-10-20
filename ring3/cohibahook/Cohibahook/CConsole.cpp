#include "main.h"

CConsole* g_pConsole = new CConsole();

bool CConsole::IsActive()
{
	if (!bActive)
		return false;
	else
		return true;
}

char* stringToLower(char *string)
{
	int i;
	int len = strlen(string);
	for (i = 0; i<len; i++) {
		if (string[i] >= 'A' && string[i] <= 'Z') {
			string[i] += 32;
		}
	}
	return string;
}


bool bIsDigitInString(std::string pszString)
{
	for (int ax = 0; ax < 9; ax++)
	{
		char buf[MAX_PATH];

		_snprintf(buf, (size_t)255, "%d", ax);

		if (strstr(pszString.c_str(), buf))
		{
			return true;
		}
	}
	return false;
}


void CConsole::addCvar(char *szName, int iValue, bool bStyle)
{
	CRetVar buf = CRetVar(szName, iValue, bStyle);
	vars.push_back(buf);
}

int CConsole::getValue(const char* szName)
{
	for (int ax = 0; ax < vars.size(); ax++)
	{
		if (vars[ax].szName == szName)
			return vars[ax].iValue;
	}
	return 0;
}

void CConsole::HandleCommands(std::string szCommand)
{
	if (szCommand.empty())
		return;

	if (strcmp(szCommand.c_str(), "1aa") == 0)
	{
		selected = 0;
		bActive = !bActive;
		return;
	}

	int r = getValue("font_color_r");
	int g = getValue("font_color_g");
	int b = getValue("font_color_b");

	char szCommandBuffer[512] = "";
	strcpy(szCommandBuffer, szCommand.c_str());

	szCommand = stringToLower(szCommandBuffer);

	if (strcmp(szCommand.c_str(), "cvars") == 0)
	{
		for (int ax = 0; ax < vars.size(); ax++)
		{
			if (vars[ax].bStyle == true)
				continue;

			print(r, g, b, "%s %i", vars[ax].szName.c_str(), vars[ax].iValue);
		}
		return;
	}

	std::string szCmd, szValue;
	int iValue;
	size_t nPos;

	bool isDigitInString = bIsDigitInString(szCommand);
	bool isTextCmd = strstr(szCommand.c_str(), "echo") || strstr(szCommand.c_str(), "exec");

	if (isDigitInString || isTextCmd)
	{
		nPos = szCommand.find_first_of(" ");

		if (nPos != std::string::npos)
		{
			szCmd = szCommand.substr(0, nPos);

			szCommand.erase(0, nPos + 1);

			if (szCommand.length() > 0)
			{
				szValue = szCommand;
			}
		}

	}
	else
		print(r, g, b, "%s - %i", szCommand.c_str(), getValue(szCommand.c_str()));

	if (strcmp(szCmd.c_str(), "echo") == 0)
	{
		print(r, g, b, szValue.c_str());
		return;
	}

	if (strcmp(szCmd.c_str(), "exec") == 0)
	{
		readConfig(szValue.c_str());
	}


	iValue = atoi(szValue.c_str());

	for (int ax = 0; ax < vars.size(); ax++)
	{
		if (strcmp(vars[ax].szName.c_str(), szCmd.c_str()) == 0)
		{
			vars[ax].iValue = iValue;

			if (vars[ax].bStyle == true)
				continue;
			if (!bCfgload)
			{
				print(0, 255, 127, "%s -> %i", szCmd.c_str(), iValue);
			}
			if (bCfgload)
			{
				bCfgload = false;
			}
		}
	}
}

void CConsole::HandleKeys(int key)
{

	Key_t pKey = gKeyMap[key];

	switch (key)
	{
	case KEY_LSHIFT:
	case KEY_RSHIFT:
	case KEY_LCONTROL:
	case KEY_RCONTROL:
	case KEY_LEFT:
	case KEY_RIGHT:
	case KEY_CAPSLOCKTOGGLE:
		return;

	case KEY_UP:
		/*if(selected > 0)
		selected--;

		bufVar[selected+1].bSelected = false;

		bufVar[selected].bSelected = true;	*/

		return;

	case KEY_DOWN:
		/*bufVar[selected].bSelected = true;

		if(selected > 0)
		bufVar[selected-1].bSelected = false;

		if(selected < bufVar.size())
		selected++;*/

		return;

	case KEY_ENTER:
		if (bComplete == true)
		{
			command = vars[selected - 1].szName;
			command.append(" ");
			selected = 0;
			bComplete = false;
			return;
		}

		HandleCommands(command.c_str());
		command.clear();
		buf.clear();
		counter = 0;
		selected = 0;
		return;

	case KEY_SPACE:
		command.append(" ");
		return;

	case KEY_BACKSPACE:
		if (command.length() > 0)
		{
			command.erase(command.length() - 1);
			counter--;
		}
		return;
	}

	if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))
	{
		command.append(pKey.m_alt);
		counter++;
	}
	else
	{
		command.append(pKey.m_def);
		counter++;
	}
}

void CConsole::RenderConsole()
{
	if (!bActive)
		return;

	static bool bDidOnce = false;

	int r = getValue("font_color_r");
	int g = getValue("font_color_g");
	int b = getValue("font_color_b");

	if (bDidOnce == false)
	{
		//print(r, g, b, "Credits");
		//print(r, g, b, "Arctosa, GT3X, reactiioN");
		//print(r, g, b, "Greetings");
		//print(r, g, b, "Ally, Kim, Strafola");

		//readConfig("default.cfg");

		bDidOnce = true;
		iPrintCounter = 0;
	}

	int iWidth, iHeight;
	g_Valve.pEngine->GetScreenSize(iWidth, iHeight);

	g_Draw.FillRGBA(600, 100, 400, 500, getValue("menu_color_r"), getValue("menu_color_g"), getValue("menu_color_b"), 100); //MainFrame
	g_Draw.DrawBox(600, 100, 400, 500, 3, getValue("menu_color_r"), getValue("menu_color_g"), getValue("menu_color_b"), 255); //MainBox
	g_Draw.DrawBox(600, 570, 400, 30, 3, getValue("menu_color_r"), getValue("menu_color_g"), getValue("menu_color_b"), 255); //InputField
	g_Draw.FillRGBA(600, 100, 400, 20, getValue("menu_color_r"), getValue("menu_color_g"), getValue("menu_color_b"), 255); //HeaderFrame
	g_Draw.DrawBox(600, 100, 400, 20, 3, getValue("menu_color_r"), getValue("menu_color_g"), getValue("menu_color_b"), 255); //Header

	static bool bDidOnce1 = false;

	if (!bDidOnce1)
	{
		g_Draw.initFont();
		bDidOnce1 = true;
		Base::Debug::LOG("Console font initialized");
	}

	g_Draw.DrawStringA(g_Draw.m_DrawFont, false, 605, 98, getValue("font_color_r"), getValue("font_color_g"), getValue("font_color_b"), 255, "console 1.0");
	g_Draw.DrawStringA(g_Draw.m_DrawFont, false, 605, 575, getValue("font_color_r"), getValue("font_color_g"), getValue("font_color_b"), 255, command.c_str());

	//Drawconsole history
	for (int ax = 0; ax < output.size(); ax++)
	{
		int iIndex = (output.size() - ax) + 1;
		int iHeight = 580 - (20 * (iIndex));

		if (iHeight < 120)
			continue;

		g_Draw.DrawStringA(g_Draw.m_DrawFont, false, 605, iHeight, r, g, b, 255, (char*)output[ax].c_str());
	}

}

void CConsole::print(int r, int g, int b, const char* szInput, ...)
{
	va_list		va_alist;
	char		szBuf[2048];

	va_start(va_alist, szInput);
	_vsnprintf(szBuf, sizeof(szBuf), szInput, va_alist);
	va_end(va_alist);

	COLOR buf;

	buf.r = r;
	buf.g = g;
	buf.b = b;
	buf.a = 255;

	output.push_back(szBuf);
	outputColor.push_back(buf);
}

void CConsole::readConfig(const char* szName)
{
	std::string line;
	std::string crtlDir = "C:\\settings\\";
	std::string path = crtlDir + szName;

	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (strstr(line.c_str(), "//"))
				continue;

			bCfgload = true;
			HandleCommands(line);
		}
		myfile.close();
	}
	else
		print(255, 0, 0, "file not found");
}

void CConsole::initCommands()
{
	addCvar("aimbot_active", 0, false);
	addCvar("aimbot_psilent", 0, false);
	addCvar("aimbot_choke", 6, false);
	addCvar("aimbot_fov", 0, false);
	addCvar("aimbot_hitbox", 11, false);
	addCvar("aimbot_autowall", 0, false);
	addCvar("aimbot_hitscan", 0, false);
	addCvar("aimbot_norecoil", 0, false);
	addCvar("aimbot_nospread", 0, false);
	addCvar("aimbot_auto", 0, false);
	addCvar("aimbot_stop", 0, false);
	addCvar("aimbot_stopkey", 0, false);
	addCvar("aimbot_scope", 1, false);
	addCvar("aimbot_smart", 0, false);
	addCvar("antiaim_fake", 0, false);
	addCvar("antiaim_choke", 8, false);
	addCvar("norecoil", 0, false);
	addCvar("novisrecoil", 0, false);
	addCvar("autopistol", 0, false);
	addCvar("esp_box", 0, false);
	addCvar("esp_name", 0, false);
	addCvar("esp_health", 0, false);
	addCvar("esp_glow", 0, false);
	addCvar("esp_crosshair", 0, false);
	addCvar("esp_watermark", 0, false);
	addCvar("misc_fov", 90, false);
	addCvar("misc_knifeskinchanger", 0, false);
	addCvar("misc_knifemodel", 0, false);
	addCvar("misc_skinchanger", 0, false);
	addCvar("skin_deagle", 0, false);
	addCvar("skin_duals", 0, false);
	addCvar("skin_fiveseven", 0, false);
	addCvar("skin_glock", 0, false);
	addCvar("skin_usp", 0, false);
	addCvar("skin_cz", 0, false);
	addCvar("skin_tec9", 0, false);
	addCvar("skin_p2000", 0, false);
	addCvar("skin_p250", 0, false);
	addCvar("skin_galil", 0, false);
	addCvar("skin_famas", 0, false);
	addCvar("skin_ak47", 0, false);
	addCvar("skin_m4a1", 0, false);
	addCvar("skin_m4a1s", 0, false);
	addCvar("skin_aug", 0, false);
	addCvar("skin_krieg", 0, false);
	addCvar("skin_scout", 0, false);
	addCvar("skin_awp", 0, false);
	addCvar("skin_g3sg1", 0, false);
	addCvar("skin_scar20", 0, false);
	addCvar("skin_m249", 0, false);
	addCvar("skin_negev", 0, false);
	addCvar("skin_knife", 0, false);
	addCvar("menu_color_r", 255, false);
	addCvar("menu_color_g", 255, false);
	addCvar("menu_color_b", 255, false);
	addCvar("font_color_r", 255, false);
	addCvar("font_color_g", 0, false);
	addCvar("font_color_b", 0, false);
}
