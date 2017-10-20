#include "main.h"

cGameEventManager gameEventManager;

cGameEventManager::cGameEventManager()
{
	this->m_nDebugID = 42;
}

int getZahlAnStelle(const int zahl, const int stelle)
{
	int retValue = int(zahl / pow(10.0, int(log10(double(zahl)) - (stelle - 1)))) % 10;
	if (retValue < 0)
		retValue = 0;
	else if (zahl < 10)
		retValue = 1;
	return retValue;
}

void cGameEventManager::FireGameEvent(ValveSDK::IGameEvent *event)
{
	const char* szEventName = event->GetName();

	/*
	short   userid		user ID who was hurt
	short	attacker	user ID who attacked
	byte	health		remaining health points
	byte	armor		remaining armor points
	string	weapon		weapon name attacker used, if not the world
	short	dmg_health	damage done to health
	byte	dmg_armor	damage done to armor
	byte	hitgroup	hitgroup that was damaged
	*/

	if (strcmp(szEventName, "player_hurt") == 0) {
		if (event->GetInt("attacker",0) - 1 == g_Valve.pEngine->GetLocalPlayer())
		{
			ValveSDK::CEngineClient::player_info_t info;
			if (!g_Valve.pEngine->GetPlayerInfo(event->GetInt("userid", 0) - 1, &info))
				return;


		}
	}
}

void cGameEventManager::RegisterSelf()
{
	g_Valve.pGameEventManager->AddListener(this, "player_hurrt", false);
}