#include "dll.h"

bool drawMenu = false;

void GUIManager::AddSection(char* name)
{
	++count;

	sections[count].name = name;
	sections[count].count = -1;
}

void GUIManager::AddItem(char* name, int max, int* val)
{
	int i = ++count;

	items[i].name = name;
	items[i].max = max;
	items[i].val = val;
}

void GUIManager::Init()
{
	select = 0;
	sub = -1;
	count = -1;

	AddSection("aim");

	AddItem("aim_fov",45,&cvars->aim_fov);
	AddItem("aim_silent",2,&cvars->aim_silent);
	AddItem("aim_autofire",1,&cvars->aim_autofire);
	AddItem("aim_body",1,&cvars->aim_body);
	AddItem("aim_team",1,&cvars->aim_team);

	AddSection("esp");

	AddItem("esp_enable",1,&cvars->esp_enable);
	AddItem("esp_name",1,&cvars->esp_name);
	AddItem("esp_weapon",1,&cvars->esp_weapon);
	AddItem("esp_health",1,&cvars->esp_health);
	AddItem("esp_box",1,&cvars->esp_box);
	AddItem("esp_team",1,&cvars->esp_team);
	AddItem("esp_outline",1,&cvars->esp_outline);

	AddSection("misc");

	AddItem("misc_nospread",1,&cvars->misc_nospread);
	AddItem("misc_norecoil",2,&cvars->misc_norecoil);
	AddItem("misc_noeffects",1,&cvars->misc_noeffects);
	AddItem("misc_autopistol",1,&cvars->misc_autopistol);
	AddItem("misc_autohop",2,&cvars->misc_autohop);
	AddItem("misc_antiaim",1,&cvars->misc_antiaim);
}

bool GUIManager::InputThink(int key)
{
	if (key == 60 || key == 72)
	{
		if (sub != -1)
		{
			select = sub;
			sub = -1;
		}
		else
			menu = !menu;

		for (int n=0; n <= count; ++n)
		{
		
		}

		return true;
	}

	if (drawn == -1)
		return false;

	if (key == 88 || key == 112)
	{
		--select;

		if (select < 0)
			select = drawn;

		return true;
	}
	else if (key == 90 || key == 113)
	{
		++select;

		if (select > drawn)
			select = 0;
	
		return true;
	}

	if (key == 89 || key == 107)
	{
		if (sub != -1)
		{
			MenuItem item = sections[sub].items[select];

			++item.val[0];

			if (item.val[0] > item.max)
				item.val[0] = 0;
		}
		else
		{
			sub = select;
			select = 0;
		}

		return true;
	}
	else if (key == 91 || key == 108)
	{
		if (sub != -1)
		{
			MenuItem item = sections[sub].items[select];

			--item.val[0];

			if (item.val[0] < 0)
				item.val[0] = item.max;
		}
		else
		{
			sub = select;
			select = 0;
		}

		return true;
	}

	return false;
}

void GUIManager::Think()
{
	if (!count)
		Init();

	drawn = -1;

	if (drawn != -1 && select > drawn)
		select = 0; // out of bounds by alternate menu

	if (menu)
		DrawMenu();
}

void GUIManager::DrawMenu()
{
	if (GetAsyncKeyState(0x2E) & 1)
		drawMenu = !drawMenu;

	if (!drawMenu)
		return;

	drawn = count;

	char str[128];

	sprintf_s(str, "%s", hackstr);

	vgui->SetTextColor(255, 255, 255);

	float x = game->scrw*0.005;
	float y = game->scrh*0.025;

	vgui->SetTextPos(x, y - 20);
	vgui->DrawText(str);

	for (int i = 0; i <= count; ++i)
	{
		if (i == select)
			vgui->SetDrawColor(205, 205, 205);
		else
			vgui->SetDrawColor(155, 155, 155);

		vgui->DrawRect(x, y, 140, 18);

		if (items[i].val[0])
			vgui->SetDrawColor(105, 205, 105);
		else
			vgui->SetDrawColor(205, 105, 105);

		vgui->DrawRect(x + 142, y, 36, 18);

		vgui->SetDrawColor(0, 0, 0);
		vgui->DrawOutlined(x, y, 140, 18);
		vgui->DrawOutlined(x + 142, y, 36, 18);

		vgui->SetTextColor(255, 255, 255);
		vgui->SetTextPos(x + 10, y);
		vgui->DrawText(items[i].name);

		if (items[i].val[0])
			vgui->SetTextColor(55, 255, 55);
		else
			vgui->SetTextColor(255, 55, 55);

		vgui->SetTextPos(x + 147, y);

		if (items[i].val[0])
		{
			char str[16];
			if (items[i].max > 1)
			{
				sprintf_s(str, "%i", items[i].val[0]);
				vgui->DrawText(str);
			}
			else
				vgui->DrawText("on");
		}
		else
			vgui->DrawText("off");

		y += 20;
	}
}