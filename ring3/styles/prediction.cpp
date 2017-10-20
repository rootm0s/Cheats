#include "dll.h"

void PredictionManager::Think(UserCmd* cmd)
{
	PredEnt null;
	null.health = 100;
	null.time = 0;

	for (int i=1; i <= globals->clients; ++i)
	{
		if (!list[i].base)
		{
			list[i] = null;
			list[i].base = 1;
		}
	}

	if (aimbot->target && game->IsReadyToFireWeapon())
	{
		float curtime = local->GetTickBase() * globals->interval;

		if (cmd->buttons & 1 && curtime >= weapon->GetNextPrimaryAttack())
		{
			float dmg = 132;//92.4;

			if (*(bool*)((long)aimbot->target + 0x23A8))
				dmg = 92.4;

			list[aimbot->target->GetIndex()].health -= (int)dmg;
			list[aimbot->target->GetIndex()].time = 3;
		}
	}
	
	for (int i=1; i <= globals->clients; ++i)
	{
		if (list[i].time <= 0)
			list[i].health = 100;

		if (list[i].time > 0)
			list[i].time -= 1;
	}
}