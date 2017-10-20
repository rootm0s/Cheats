#include "dll.h"

inline bool AimbotManager::Insight(Entity* player,Vector pos)
{
	RayData ray(game->vieworigin,pos);

	TraceFilter filter;
	TraceResult tr;

	filter.local = local;
	filter.ignore = player;

	engine->TraceRay(tr,ray,filter);

	return tr.fraction == 1;
}

inline bool AimbotManager::GetPosition(Entity* player,Vector& pos)
{
	matrix3x4 matrix[128];

	player->SetupBones(matrix);

	void* model = player->GetModel();

	if (!model)
		return false;

	void* studio = engine->GetStudioModel(model);

	if (!studio)
		return false;

	pos = player->GetHitboxPosition(cvars->aim_body ? 0 : 11,studio,matrix);

	if (!pos)
		return false;

	if (!Insight(player,pos))
		return false;

	return true;
}

bool AimbotManager::ValidTarget(Entity* player)
{
	if (player == local)
		return false;

	if (player->IsDormant())
		return false;

	if (!player->IsAlive())
		return false;

	if (player->GetColor()[3] == -128)
		return false;

	if (!cvars->aim_team && player->GetTeam() == local->GetTeam())
		return false;

	return true;
}

void AimbotManager::GetTarget(Vector eyeang,Vector& eyepos)
{
	int i;
	int clients = globals->clients;
	Entity* player;
	Vector pos,ang;
	float len,dist = 8192;

	float fov = (float)(cvars->aim_fov+90);
	float radian_fov = sin(rad(fov));

	for (i=1; i <= clients; ++i)
	{
		if (!(player = client->GetEntity(i)))
			continue;

	//	if (prediction->list[i].health <= 0)
		//	continue;

		if (!ValidTarget(player))
			continue;

		if (!GetPosition(player,pos))
			continue;

		if (cvars->aim_fov)
		{
			Vector dir = (pos - game->vieworigin);
			len = dir.Length();

			if (len)
			{
				dir /= len;

				if (dir.Dot(eyeang.Forward()) < radian_fov)
					continue;
			}
		}

		len = (game->vieworigin - pos).Length();

		if (len > dist)
			continue;

		target = player;
		dist = len;
		eyepos = pos;
	}
}

void AimbotManager::Think(UserCmd* cmd)
{
	target = 0;

	if (!weapon)
		return;

	if (!(cmd->buttons & 16384)) // IN_ALT1
		return;

	Vector eyepos;

	GetTarget(cmd->ang,eyepos);

	if (!target)
		return;

	if (cvars->aim_autofire)
	{
		cmd->buttons |= 1;

		if (!cvars->misc_nospread)
		{
			float cone = weapon->GetSpread();

			float inacr = weapon->GetInaccuracy();

			Vector spread = Vector(1 * inacr + 1 * cone,-1 * inacr + -1 * cone,0);

			Vector aimdir = eyepos - game->vieworigin;

			float len = aimdir.Length();

			Vector spreadang = aimdir.Angle() + Vector(1,-spread.x,spread.y).Angle();

			RayData ray(game->vieworigin,game->vieworigin+(spreadang.Forward()*8192)); 

			TraceFilter filter;
			TraceResult tr;

			filter.local = local;

			engine->TraceRay(tr,ray,filter);

			Vector dir = (tr.hitpos - game->vieworigin);

			dir /= dir.Length();

			dir *= len;

			float dist = (eyepos - (game->vieworigin + dir)).Length();

			if (local->GetVelocity().Length() <= 70)
				cmd->move *= 0.15;
			else
			{
				cmd->move.x = -cmd->move.x;
				cmd->move.y = -cmd->move.y;
			}

			if (tr.ent != target && dist > 10)
				cmd->buttons &= ~1;
		}
	}

	cmd->ang = (eyepos - game->vieworigin).Angle();

	if (!cvars->aim_silent)
		engine->SetViewAngles(cmd->ang);
}