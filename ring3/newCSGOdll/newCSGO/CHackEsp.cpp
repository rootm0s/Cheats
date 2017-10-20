#include "stdafx.h"
CHackEsp cEsp;
Vector vHead, vEyePos;
bool bIsEnemy;

void CHackEsp::DrawPlayers(int i, IClientEntity* pLocal, IClientEntity* pEntity )
{
	player_info_t pInfo;

	if( !pEngine->GetPlayerInfo(i, &pInfo ))
		return;

	if( !cEng.GetBonePosition( pEntity, vHead, 10 ) )
		return;

	/* Alternate way get headposition 

	bool bDucking = pEntity->GetFlags() &(1<<1);

	if( bDucking )
		vHead = pEntity->GetAbsOrigin() + Vector( 0, 0, 52 );
	else
		vHead = pEntity->GetAbsOrigin() + Vector( 0, 0, 72 );

	*/
	//bool bIsVisible = cEng.IsVisible(pLocal, pEntity, cEng.GetEyePosition(pLocal), vHead);

	bIsEnemy = pEntity->GetTeamNum() != pLocal->GetTeamNum( );

	Vector vScreenHead, vScreenLeg;
	
	if( cMath.WorldToScreen( vHead, vScreenHead ) && cMath.WorldToScreen( pEntity->GetOrigin(), vScreenLeg ) ) 
	{
		float Height = abs( vScreenLeg.Y - vScreenHead.Y );
		float Width = Height / 2.0f;

		int  fHealth = cEng.GetHealth( pEntity );
		
		char playerName[32];
		sprintf( playerName, "%s", pInfo.name );

		if( 1)//bIsEnemy )
		{
			float IsV=IsVisible(pLocal->GetOrigin(),vHead);

			cDraw.DrawHealthBar( vScreenHead.X, vScreenLeg.Y + 5, fHealth, Width, 5, Color(0,255,0,255) );

			//sprintf( playerName, "%f", IsV );
			cDraw.DrawString( espfont, 1, vScreenHead.X, vScreenHead.Y - 5, /*bIsVisible ? Color(255,0,0,255):*/Color(255,200,0,255), playerName );

			if(IsV >=0.99f)
			{
				cDraw.OutlinedRect(vScreenHead.X - Width / 2, vScreenHead.Y, Width, Height, Color(255,0,0,255));
			}else{
				cDraw.OutlinedRect(vScreenHead.X - Width / 2, vScreenHead.Y, Width, Height, Color(0,255,0,255));
			}

		}
	}
}