#include "DllMain.h"


__forceinline GenDT_BasePlayer* _GetBasePlayer ( CBaseEntity* BaseEntity)
{
	if ( !BaseEntity )
		return NULL;

	GenDT_BasePlayer* pBasePlayer = ( GenDT_BasePlayer* )( BaseEntity );
	if ( !pBasePlayer )
		return NULL;

	return pBasePlayer;
}

__forceinline GenDT_CSPlayer* _GetCSPlayer ( CBaseEntity* BaseEntity )
{
	if ( !BaseEntity )
		return NULL;

	GenDT_CSPlayer* pBasePlayer = ( GenDT_CSPlayer* )( BaseEntity );
	if ( !pBasePlayer )
		return NULL;

	return pBasePlayer;
}

int CSS_Player::_GetTeamNumber()
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *pBasePlayer->iTeamNum();
	}

	return 0;
}


int CSS_Player::_GetArmorValue()
{
	GenDT_CSPlayer* pBasePlayer = _GetCSPlayer(this);

	if ( pBasePlayer )
	{
		return *pBasePlayer->ArmorValue();
	}

	return 0;
}

bool CSS_Player::_bHasHelmet()
{
	GenDT_CSPlayer* pBasePlayer = _GetCSPlayer(this);

	if ( pBasePlayer )
	{
		return 	pBasePlayer->bHasHelmet();
	}

	return false;
}
int CSS_Player::_GetLifeState ( void )
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *( char* )( pBasePlayer->lifeState() );
	}

	return LIFE_DEAD;
}

int CSS_Player::_GetHealth ( void )
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *pBasePlayer->iHealth();
	}

	return 0;
}

int CSS_Player::_GetFlags ( void )
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *pBasePlayer->fFlags();
	}

	return 0;
}


int CSS_Player::GetRenderColor()
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *pBasePlayer->clrRender();
	}

	return 0;
}

Vector CSS_Player::_GetPunchAngle ( void )
{
	Vector viewOffset( 0, 0, 0 );
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		viewOffset = *pBasePlayer->localdata()->Local()->aimPunchAngle();

	}

	return Vector( viewOffset.x, viewOffset.y, viewOffset.z );
}

Vector CSS_Player::_GetEyePosition()
{
	Vector viewOffset( 0, 0, 0 );
	Vector Origin(0,0,0);
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		viewOffset = *pBasePlayer->localdata()->vecViewOffset0();	
		Origin = *pBasePlayer->vecOrigin();
	}

	return viewOffset + Origin;

}

int CSS_Player::_GetTickBase()
{
	GenDT_BasePlayer* pBasePlayer = _GetBasePlayer( this );

	if ( pBasePlayer )
	{
		return *pBasePlayer->localdata()->nTickBase();
	}

	return 0;
}

