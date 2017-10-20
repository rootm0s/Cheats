#include "DLLMain.h"

#ifndef _CRADAR_H_
#define _CRADAR_H_

//===================================================================================
class cRadar
{
public:
	/**********************************************************************************/
	void DrawRadarBack();
	void DrawRadarPoint(Vector vecOriginx, Vector vecOriginy, QAngle vAngle, int r, int g, int b, CBaseEntity* pLocal, CBaseEntity* pEntity);
};
extern cRadar gRadar;
//===================================================================================
//===================================================================================
#endif