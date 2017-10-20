#include "cGameEvent.h"
#include <time.h>
#define EVENT_HOOK( x )

cGameEvent gEvent;

void cGameEvent::FireGameEvent( ValveSDK::IGameEvent *event )
{
	const char* szEventName = event->GetName( );
	if( !szEventName )	return;

	if( strcmp( szEventName, "player_connect" ) == 0 )
	{
		/*string one = "Player ";
		one += event->GetString( "name" );
		one += " connected, IP : ";
		one += event->GetString( "address" );
		one += ", SteamID : ";
		one += event->GetString( "networkid" );
		g_pConsole->echo( one.c_str( ) );
		g_pConsole->scrollCount = 0;
		g_pConsole->lineIndex = g_pConsole->prevLines.size( ) + 1;*/
	}	

	/*if( strcmp( szEventName, "round_start" ) == 0 && g_bAutoWeapon )
	{
		g_pEngine->ClientCmd( czWeapon );
	}*/
	if (strcmp(szEventName, "round_start") == 0)
	{
		g_pEngine->ExecuteClientCmd("say lol");
	}
	if( strcmp( szEventName, "player_hurt" ) == 0 )
	{
		ValveSDK::CEngineClient::player_info_t pInfo;
		g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &pInfo);
		if( pInfo.userID == event->GetInt( "attacker", 0 ) )
		{
			DamageText[ m_iDamageText ] = DamageText_t( event->GetInt( "dmg_health", 0), ( char* )event->GetString( "weapon", "" ), clock( ) + 3000.f );
			m_iDamageText++;
			if( m_iDamageText == 15 ) m_iDamageText = 0;
		}
	}
	if (strcmp(szEventName, "player_death") == 0)
	{
		ValveSDK::CEngineClient::player_info_t pInfo;
		g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &pInfo);
		if (pInfo.userID == event->GetInt("attacker", 0) && pInfo.userID != event->GetInt("userid", 0))
		{
			gVars.kills++;
		}
		if (pInfo.userID == event->GetInt("attacker", 0) && pInfo.userID != event->GetInt("userid", 0) && event->GetBool("headshot", false))
		{
			gVars.headshots++;
		}
		if (pInfo.userID == event->GetInt("userid", 0))
		{
			gVars.deaths++;
		}
	}
}

void cGameEvent::RegisterSelf( )
{
	sprintf( czWeapon, "say !m249 !helmet" );

	m_iDamageText = 0;

	//g_EventManager2->AddListener( this, "round_start", false );
	g_pGameEventManager->AddListener( this, "player_connect", false );
	g_pGameEventManager->AddListener(this, "player_hurt", false);
	g_pGameEventManager->AddListener(this, "round_start", false);
	g_pGameEventManager->AddListener(this, "player_death", false);
}

void cGameEvent::Register( )
{
	EVENT_HOOK( FireEvent );
}