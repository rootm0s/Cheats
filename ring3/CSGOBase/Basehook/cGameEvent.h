#include "DLLMain.h"

#ifndef I_GAMEEVENT
#define I_GAMEEVENT

struct DamageText_t
{
	int iDamage;
	char szWeapon[32];
	float flEnd;
	DamageText_t(){}
	DamageText_t(int iDamage, char *szWeapon, float flEnd)
	{
		strcpy(this->szWeapon, szWeapon);
		this->iDamage = iDamage;
		this->flEnd = flEnd;
	}
};

class cGameEvent : public ValveSDK::IGameEventListener2
{

private:

	int m_iDamageText;
	DamageText_t DamageText[ 15 ];

public:

	void FireGameEvent( ValveSDK::IGameEvent *event );
	void RegisterSelf( );
	void Register( );
	
	DamageText_t GetDamageText( int nId )
	{
		return DamageText[ nId ];
	}

	char czWeapon[ 30 ];

};

extern cGameEvent gEvent;

#endif