#include "dll.h"

void GameUtil::Think()
{
	if (cvars->misc_antiaim &&
		!(cmd->buttons & 32) &&
		(weapon && weapon->GetPrimaryClip() != -1) &&
		cmd->ang == viewangles)
	{
		cmd->ang.x = 89;
		cmd->ang.y += 180;
	}

	if (cmd->ang != cmd->ang)
		cmd->ang = Vector(0,0,0);

	if (cmd->ang.x > 89)
		cmd->ang.x = 89;

	if (cmd->ang.x < -89)
		cmd->ang.x = -89;

	if (cmd->ang.y > 180)
		cmd->ang.y -= 360;

	if (cmd->ang.y < -180)
		cmd->ang.y += 360;

	if (cmd->ang.y > 179.99)
		cmd->ang.y = 179.99;

	if (cmd->ang.y < -179.99)
		cmd->ang.y = -179.99;

	//cmd->ang.z = 0;

	// fix movement

	float len = cmd->move.Length();

	if (cmd->ang != viewangles && len)
	{
		Vector dt = cmd->move.Angle();
		dt.y += cmd->ang.y - viewangles.y;

		cmd->move = dt.Forward() * len;
	}

	if (cmd->move != cmd->move)
		cmd->move = Vector(0,0,0);

	if (cmd->move.x > 10000)
		cmd->move.x = 10000;

	if (cmd->move.x < -10000)
		cmd->move.x = -10000;

	if (cmd->move.y > 10000)
		cmd->move.y = 10000;

	if (cmd->move.y < -10000)
		cmd->move.y = -10000;

	cmd->move.z = 0;

	static int last_buttons = 0;

	int copy = cmd->buttons;

	if (cmd->buttons == last_buttons)
		copy = last_buttons;

	last_buttons = cmd->buttons;

	if (!cvars->misc_nospread)
		cmd->buttons = copy;
}

void GameUtil::Speedhack()
{
	void* baseptr;
	_asm mov baseptr,ebp;

	int i = 4;

	while (i--)
		baseptr = *(void**)baseptr;

	if (game->speed)
	{
		*((char*)baseptr + 0x4) -= 0x5;
		--game->speed;
	}
	else
		game->speed = 10;
}

Vector GameUtil::ToScreen(Vector pos)
{
	matrix3x4& mtx = engine->GetScreenMatrix();
	float w = mtx[3][0] * pos.x + mtx[3][1] * pos.y + mtx[3][2] * pos.z + mtx[3][3];

	if (w <= 0.001)
		return Vector(0);

	Vector scr(0);

	scr.x = (scrw * 0.5);
	scr.x += (0.5 * ((mtx[0][0] * pos.x + mtx[0][1] * pos.y + mtx[0][2] * pos.z + mtx[0][3]) / w) * scrw + 0.5);

	scr.y = (scrh * 0.5);
	scr.y -= (0.5 * ((mtx[1][0] * pos.x + mtx[1][1] * pos.y + mtx[1][2] * pos.z + mtx[1][3]) / w) * scrh + 0.5);

	return scr;
}

void GameUtil::SimulatePrediction()
{
	curtime = local->GetTickBase() * globals->interval;
	
	float backup = globals->frametime;

	globals->frametime = globals->interval;

	weapon->UpdateAccuracyPenalty();

	client->SetupMove(local,cmd);

	client->ProcessMovement(local);

	client->FinishMove(local,cmd);

	globals->frametime = backup;

	vieworigin = local->GetOrigin();
	vieworigin += local->GetViewOffset();
}

void GameUtil::Autohop()
{
	if (!cvars->misc_autohop)
		return;

	if (cvars->misc_autohop == 2)
	{
		Vector origin = local->GetOrigin();
		origin += local->GetVelocity() * globals->interval;

		RayData ray(origin,origin-Vector(0,0,1));

		TraceFilter filter;
		TraceResult tr;

		filter.local = local;

		engine->TraceRay(tr,ray,filter);

		if (tr.fraction != 1)
			cmd->buttons &= ~2;
	}

	if (!(local->GetFlags() & 1))
		cmd->buttons &= ~2;
}

bool GameUtil::IsReadyToFireWeapon()
{
	if (!weapon)
		return false;

	if (weapon->GetPrimaryClip() <= 0)
		return false;

	if (local->GetNextAttack() > curtime)
		return false;

	return true;
}

Vector GameUtil::GetSpread(int seed)
{
	game->RandomSeed(seed);

	float a = game->RandomFloat(0,pi*2);
	float b = game->RandomFloat(0,weapon->GetInaccuracy());
	float c = game->RandomFloat(0,pi*2);
	float d = game->RandomFloat(0,weapon->GetSpread());
	
	float sa,sb,ca,cb;
	SinCos(a,sa,ca);
	SinCos(c,sb,cb);

	return Vector(ca*b + cb*d,sa*b + sb*d,0);
}

bool IsPistol(char* name)
{
	if (cmp(name,"CWeaponGlock"))
		return true;

	if (cmp(name,"CWeaponHKP2000"))
		return true;

	if (cmp(name,"CDEagle"))
		return true;

	if (cmp(name,"CWeaponTec9"))
		return true;

	if (cmp(name,"CWeaponP250"))
		return true;

	if (cmp(name,"CWeaponElite"))
		return true;

	if (cmp(name,"CWeaponFiveSeven"))
		return true;

	if (cmp(name,"CWeaponAWP"))
		return true;

	if (cmp(name,"CWeaponSSG08"))
		return true;

	return false;
}

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / 0.015625 ) )

float GetWavRoll(Vector dir)
{
	Vector forward(1,-dir.x,dir.y);

	Vector up(0,-dir.x,abs(dir.y+(1/dir.y)+(1/dir.y)));

	if (dir.x > 0 && dir.y < 0)
		up.y = abs(up.y);
	else if (dir.x < 0 && dir.y < 0)
		up.y = -abs(up.y);

	Vector left = up.Cross(forward);

	float roll = deg(atan2f(left.z,(left.y * forward.x) - (left.x * forward.y)));

	return roll;
}

void GameUtil::SimulateFireBullets()
{
	if (!weapon)
		return;

	Vector punch = local->GetAimPunch();

/*	if (cvars->misc_norecoil == 2)
		cmd->ang -= punch * 2;

	if (cvars->misc_norecoil == 1 && !IsPistol(weapon->GetClass()->name))
	{
		float len = punch.Length();

		static float pull = 0;

		float rnd = game->RandomFloat(5,7);

		bool firing = false;

		if (cmd->buttons & 1 && IsReadyToFireWeapon())
			firing = true;

		if (firing && len > 0.5)
			pull += rnd;
		else
			pull -= rnd;

		if (pull < 0)
			pull = 0;

		if (pull > 100)
			pull = 100;

		if (len > 0 && len > 0.5)
		{
			punch /= len;

			punch *= (len*0.01)*pull;

			cmd->ang -= punch*2;
		}
	}*/

	if (!(cmd->buttons & 1))
		return;

	if (!IsReadyToFireWeapon())
	{
		cmd->ang = viewangles;
		return;
	}

	if (cvars->misc_nospread)
	{
		int seed = (MD5_PseudoRandom(cmd->index) & 255) + 1;

		Vector spread = GetSpread(seed) * -1;

		if (!spread)
			return;

		Vector v = cmd->ang;

		Vector dir = v.Forward() + v.Right() * spread.x + v.Up() * spread.y;

		cmd->ang = dir.Angle(&v.Up());
		cmd->ang.z += GetWavRoll(spread);
	}

	if (!(curtime >= weapon->GetNextPrimaryAttack()))
	{
		if (cvars->aim_silent == 2)
			cmd->ang = viewangles;

		if (IsPistol(weapon->GetClass()->name))
			cmd->buttons &= ~1;
	}
}