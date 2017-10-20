#include "cRadar.h"
#include "DLLMain.h"
cRadar gRadar;
//void blackBorder(int x, int y, int w, int h)
//{
//	g_Draw.FillRGBA(x - 1, y - 1, w + 2, 1, 0, 0, 0, 254); //top
//	g_Draw.FillRGBA(x - 1, y, 1, h - 1, 0, 0, 0, 254);	//left
//	g_Draw.FillRGBA(x + w, y, 1, h - 1, 0, 0, 0, 254);	//right
//	g_Draw.FillRGBA(x - 1, y + h - 1, w + 2, 1, 0, 0, 0, 254); //bottom
//}
void cRadar::DrawRadarBack()
{
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
	int centerx = iScreenWidth / 2 - 20;
	int centery = iScreenHeight - 110;
	int size = 100;
	size -= 10;
	//if (g_Console.getValue("vis_radar_bg") == 1){
	//	blackBorder(centerx - size - 6, centery - size - 20, 2 * size + 12, 2 * size + 26);

	//	g_Draw.FillRGBA(centerx - size - 6, centery - size - 20, centerx + size / 2 - 3, 20, 35, 35, 35, 255);
	//	g_TableFont.DrawString(false, centerx - size, centery - size - 19, 255, 255, 0, "Radar");
	//	g_Draw.FillRGBA(centerx - size - 6, centery - size, 5, centery + 30, 35, 35, 35, 255);
	//	g_Draw.FillRGBA(centerx - size - 6, centery + size, centerx + size / 2 - 3, 5, 35, 35, 35, 255);
	//	g_Draw.FillRGBA(centerx + size + 1, centery - size, 5, centery + 30, 35, 35, 35, 255);

	//	g_Draw.FillRGBA(centerx - size, centery - size, 2 * size, 2 * size, 35, 35, 35, 240);
	//}
	//blackBorder(centerx - size, centery - size, 2 * size, 2 * size);
	g_Draw.FillRGBA(centerx, centery - size, 1, 2 * size, 0, 0, 0, 120);
	g_Draw.FillRGBA(centerx - size, centery, 2 * size, 1, 0, 0, 0, 120);

	//g_Draw.fillArea(centerx - 12.5, centery, 13.5, 2, 255, 255, 255, 120);
	//g_Draw.fillArea(centerx, centery, 13.5, 2, 255, 255, 255, 120);
}

void  cRadar::DrawRadarPoint(Vector vecOriginx, Vector vecOriginy, QAngle vAngle, int r, int g, int b, CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
	int centerx = iScreenWidth / 2 - 20;
	int centery = iScreenHeight - 110;
	int size = 100;
	float dx = vecOriginx.x - vecOriginy.x;
	float dy = vecOriginx.y - vecOriginy.y;
	float flAngle = vAngle.y;
	float yaw = (flAngle)* (M_PI / 180.0);
	float mainViewAngles_CosYaw = cos(yaw);
	float mainViewAngles_SinYaw = sin(yaw);
	float x = dy*(-mainViewAngles_CosYaw) + dx*mainViewAngles_SinYaw;
	float y = dx*(-mainViewAngles_CosYaw) - dy*mainViewAngles_SinYaw;
	float range = size * 30;
	if (fabs(x)>range || fabs(y)>range)
	{
		if (y>x)
		{
			if (y>-x) {
				x = range*x / y;
				y = range;
			}
			else  {
				y = -range*y / x;
				x = -range;
			}
		}
		else {
			if (y>-x) {
				y = range*y / x;
				x = range;
			}
			else  {
				x = -range*x / y;
				y = -range;
			}
		}
	}
	int ScreenX = centerx + int(x / range*float(size));
	int ScreenY = centery + int(y / range*float(size));
	size -= 10;
	int radar_menu_x = centerx;
	int radar_menu_y = centery;

	if (ScreenX < radar_menu_x - size)
		ScreenX = radar_menu_x - size + 1;
	else if (ScreenX > radar_menu_x + size)
		ScreenX = radar_menu_x + size - 3;

	if (ScreenY < radar_menu_y - size)
		ScreenY = radar_menu_y - size + 2;
	else if (ScreenY > radar_menu_y + size)
		ScreenY = radar_menu_y + size - 4;



	g_Draw.FillRGBA(ScreenX - 1, ScreenY - 1, 4, 4, r, g, b, 255);
	/*else if (pEntity->GetAbsOrigin().z < pLocal->GetAbsOrigin().z - 10)
	{
		g_Draw.DrawPolygon(false, ScreenX - 1, ScreenY - 1, 4, 4, r, g, b);
	}
	else if (pEntity->GetAbsOrigin().z > pLocal->GetAbsOrigin().z + 10)
	{
		g_Draw.DrawPolygon(true, ScreenX - 1, ScreenY - 1, 4, 4, r, g, b);
	}*/
		
}