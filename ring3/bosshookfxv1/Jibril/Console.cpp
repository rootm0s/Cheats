#include "Console.h"

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



struct CVar{
	float flValue;
	std::string szName;

	template <typename f>
	f GetValue(){
		return flValue;
	}
};

struct Console{
public:
	std::vector<CVar> CVars;

private:

};


Console* g_pConsole = new Console;