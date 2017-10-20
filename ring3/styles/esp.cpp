#include "dll.h"

void ESPManager::EntThink()
{
	index = -1;

	if (!local)
		return;

	int i;
	Entity* entity;

	for (i=1; i <= globals->clients; ++i)
	{
		if (!(entity = client->GetEntity(i)))
			continue;

		if (entity->IsDormant())
			continue;

		if (!cvars->esp_team && entity->GetTeam() == local->GetTeam())
			continue;

		if (entity == local)
			continue;

		if (!entity->IsAlive())
			continue;

		todraw[++index] = i;

		if (index == 255)
			break;
	}
}

void ESPManager::GetHealthColor(Entity* entity,float* hp)
{
	float health = entity->GetHealth();

	if (health >= 75)
		hp[0] = 0,hp[1] = 1;
	else if (health >= 25)
		hp[0] = 1,hp[1] = 1;
	else
		hp[0] = 1,hp[1] = 0;
}

void ESPManager::DrawName(Entity* entity)
{
	if (entity->GetTeam() == 2)
		vgui->SetTextColor(255,0,0);
	else
		vgui->SetTextColor(0,155,255);

	char str[64];

	PlayerInfo info;
	engine->GetPlayerInfo(entity->GetIndex(),info);

	sprintf_s(str,"%s",info.name);

	vgui->DrawText(str);
}

void ESPManager::DrawWeapon(Entity* entity)
{
	char str[64];

	Entity* gun = client->GetEntity(entity->GetActiveWeapon(),true);

	char* name = engine->GetModelName(gun->GetModel());

	int len = strlen(name);

	int i = len-4;

	while (true)
	{
		if (name[i-1] == '_')
			break;

		--i;
	}

	int n = 0;

	while (i+n != len-4)
	{
		str[n] = name[i+n];

		++n;
	}

	str[n] = '\0';

	if (cmp(str,"t") || cmp(str,"ct"))
		strcpy_s(str,"knife");

	if (cmp(str,"223"))
		strcpy_s(str,"usp");

	if (cmp(str,"s"))
	{
		if (cmp(gun->GetClass()->name,"CWeaponM4A1"))
			strcpy_s(str,"m4a1-s");
	}

	vgui->DrawText(str);
}

void ESPManager::DrawHealth(Entity* entity)
{
	float hp[3];

	GetHealthColor(entity,hp);

	hp[0] *= 255;
	hp[1] *= 255;
	hp[2] *= 255;

	vgui->SetTextColor((int)hp[0],(int)hp[1],(int)hp[2]);

	char str[64];

	sprintf_s(str,"%i HP",entity->GetHealth());
	 
	vgui->DrawText(str);
}

void ESPManager::DrawBox(Vector min,Vector max)
{
	Vector vecs[24] = { // there must be a better solution to this...
		// start and bottom
		min,
		Vector(max.x,min.y,min.z),
		min,
		Vector(min.x,max.y,min.z),
		max,
		Vector(max.x,min.y,max.z),
		max,
		Vector(min.x,max.y,max.z),
		// top opposite
		Vector(max.x,min.y,max.z),
		Vector(min.x,min.y,max.z),
		Vector(min.x,max.y,max.z),
		Vector(min.x,min.y,max.z),
		// bottom opposite
		Vector(min.x,max.y,min.z),
		Vector(max.x,max.y,min.z),
		Vector(max.x,min.y,min.z),
		Vector(max.x,max.y,min.z),
		// sides
		min,
		Vector(min.x,min.y,max.z),
		max,
		Vector(max.x,max.y,min.z),
		// opposite
		Vector(min.x,max.y,min.z),
		Vector(min.x,max.y,max.z),
		Vector(max.x,min.y,min.z),
		Vector(max.x,min.y,max.z),
	};

	int x = 0;

	while (x <= 23)
	{
		Vector start = game->ToScreen(vecs[x]);
		Vector end = game->ToScreen(vecs[x+1]);

		if (start.Length() && end.Length())
			vgui->DrawLine(start.x,start.y,end.x,end.y);

		x += 2;
	}
}

void ESPManager::Think()
{
	if (!cvars->esp_enable)
		return;

	if (!local || index == -1)
		return;

	Entity* entity;
	int i;
	Vector pos;

	for (i=0; i <= index; ++i)
	{
		if (!(entity = client->GetEntity(todraw[i])))
			continue;

		if (cvars->esp_box)
		{
			pos = entity->GetRenderOrigin();

			if (entity->GetTeam() == 2)
				vgui->SetDrawColor(255,0,0);
			else
				vgui->SetDrawColor(0,0,255);

			DrawBox(pos+entity->GetMins(),pos+entity->GetMaxs());
		}

		pos = game->ToScreen(entity->GetCenter());

		if (!pos)
			continue;

		int y = 0;

		if (cvars->esp_name)
		{
			vgui->SetTextPos(pos.x,pos.y);

			DrawName(entity);

			y += 10;
		}

		if (cvars->esp_weapon)
		{
			vgui->SetTextPos(pos.x,pos.y+y);

			DrawWeapon(entity);

			y += 10;
		}

		if (cvars->esp_health)
		{
			//vgui->SetTextPos(pos.x,pos.y+y);
			//DrawHealth(entity);

			y += 3;

			float hp = (float)entity->GetHealth();
			hp *= 0.5;

			float maxhp = 100;
			maxhp *= 0.5;

			int h = 6;

			vgui->SetDrawColor(0,0,0);
			vgui->DrawRect(pos.x,pos.y+y,maxhp,h);

			if (entity->GetTeam() == 2)
				vgui->SetDrawColor(255,0,0);
			else
				vgui->SetDrawColor(0,255,255);

			vgui->DrawRect(pos.x,pos.y+y,hp,h);

			if (entity->GetTeam() == 2)
				vgui->SetDrawColor(192,0,0);
			else
				vgui->SetDrawColor(0,192,255);

			vgui->DrawRect(pos.x,pos.y+y,min(hp,maxhp*0.6),h);

			if (entity->GetTeam() == 2)
				vgui->SetDrawColor(128,0,0);
			else
				vgui->SetDrawColor(0,128,255);

			vgui->DrawRect(pos.x,pos.y+y,min(hp,maxhp*0.2),h);

			vgui->SetDrawColor(0,0,0);
			vgui->DrawOutlined(pos.x,pos.y+y,maxhp,h);
		}
	}
}