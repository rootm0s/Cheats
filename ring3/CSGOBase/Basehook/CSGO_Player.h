#ifndef CSGO_PLAYER__H__
#define CSGO_PLAYER__H__
class CSS_Player : public CBaseEntity
{
public:
	int _GetLifeState( void );
	int _GetTeamNumber( void );
	int _GetHealth( void );
	int _GetFlags( void );
	Vector _GetEyePosition( void );
	Vector _GetPunchAngle( void );
	int _GetTickBase ( void );
	int GetRenderColor(void);
	//EHANDLE _GetObserverTarget ( void );
	int _GetObserverMode ( void );
	ValveSDK::CBaseCombatWeapon* _GetBaseCombatActiveWeapon( void );
	//QAngle _GetEyeAngle ( void );
	//int _getWeaponID(CBaseEntity* pEntity );
	int _GetArmorValue();
	bool _bHasHelmet();
}; 
#endif