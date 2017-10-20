#include "DllMain.h"
#include "ModuleSecurity.h"
#include <time.h>
//##############################
//#crownt sucki sucki big diq. #
//#gt3x						   #
//##############################
//=================================================================================================
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
//=================================================================================================
ValveSDK::HLCLient* g_pClient;
ValveSDK::CEngineClient* g_pEngine;
ValveSDK::ISurface* g_pSurface;
ValveSDK::IPanel* g_pPanel;
ValveSDK::CEntityList* g_pEntList;
ValveSDK::CInput* g_pInput;
ValveSDK::CPrediction* g_pPred;
ValveSDK::CGameMovement* g_pGameMovement;
ValveSDK::IVRenderView* g_pRenderView = NULL;
ValveSDK::CGlobalVars* g_pGlobalVars;
ValveSDK::IVModelRender* g_pModelRender;
ValveSDK::CModelInfo* g_pModel;
ValveSDK::CDebugOverlay* g_pDebugOverlay;
ValveSDK::IGameEventManager* g_pGameEventManager;
ValveSDK::CTrace* g_pEngineTrace;
ValveSDK::CGlowObjectManager* g_pGlowObjectManager;
ValveSDK::ImoveHelper* g_pMoveHelper;
//=================================================================================================
Base::Utils::CVMTHookManager* g_pPaintTraverseVMT = NULL;
Base::Utils::CVMTHookManager* g_pCreateMoveVMT = NULL;
Base::Utils::CVMTHookManager* g_pRunCommandVMT = NULL;
Base::Utils::CVMTHookManager* g_pInKeyEventVMT = NULL;
Base::Utils::CVMTHookManager* g_pFireEventVMT = NULL;
Base::Utils::CVMTHookManager* g_pModelRenderVMT = NULL;
//=================================================================================================
CDraw g_Draw;
CNoSpread g_NoSpread;
cTableFont g_TableFont;
CAimbot g_Aimbot;
CConsole g_Console;
CSpread g_pSpread;
CTimer g_Timer;
int delay(long seconds)
{
	time_t t1;
	t1 = time(0) + seconds;
	while (time(0) < t1)
		;
}
float betaVectorNormalize(Vector& vec)
{
	float flLength = (float)sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));

	if (flLength)
	{
		float flTemp = 1.f / flLength;
		vec[0] *= flTemp;
		vec[1] *= flTemp;
		vec[2] *= flTemp;
	}

	return flLength;
}
//=================================================================================================
bool WorldToScreen ( const Vector &vOrigin, Vector &vScreen )
{
	return ( g_pDebugOverlay->ScreenPosition( vOrigin, vScreen ) != 1 );
}
//=================================================================================================
typedef void ( *TraceLineWithFilter )( const Vector&, const Vector&, unsigned int, CBaseEntity*, CBaseEntity* , int, ValveSDK::CTrace::trace_t* );
//void TraceIgnore2Ents( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,CBaseEntity*  pLocal, CBaseEntity* pEnemy, int unknown, ValveSDK::CTrace::trace_t *tr )
//{
//	static TraceLineWithFilter tracer = (TraceLineWithFilter)(Base::Utils::PatternSearch(("client.dll"),(PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x83\xEC\x7C\x56\x8B\x43\x08","xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",NULL,NULL));
//	tracer( vecAbsStart, vecAbsEnd, mask, pLocal, pEnemy, unknown, tr );
//}
//=================================================================================================
bool CAimbot::IsVisible(Vector& vecStart, Vector& vecEnd, CBaseEntity* pEntity)
{
	ValveSDK::CTrace::trace_t tr;
	ValveSDK::CTrace::Ray_t ray;
	ValveSDK::CTrace::CTraceFilter filter;
	filter.pSkip = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	ray.Init(vecStart, vecEnd);
	g_pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

	return (tr.m_pEnt == pEntity || tr.fraction > 0.99f);
}
typedef bool ( __thiscall *IsPlayer_t )( void *thisptr );
bool IsPlayer(CBaseEntity* pEnt)
{
	return ((IsPlayer_t)(*(PDWORD)(*(PDWORD)( pEnt ) + 0x260 )))(pEnt);
}
bool bTriggerKey;
//=================================================================================================
namespace Base
{
	namespace Triggerbot
	{
		Vector traceEndPos;
		bool isTriggering;

		void Do(ValveSDK::CInput::CUserCmd* pUserCmd, BOOL norecoil, BOOL nospread)
		{
			if (g_Console.getValue("trigger_active"))
			{
				static DWORD tickCount = 0;

				if (isTriggering) // rotes crosshair etwas laenger anzeigen hehe :)
				{
					if (tickCount + 200 <= (DWORD)GetTickCount())
					{
						isTriggering = false;
					}
				}

				CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());
				if (!pLocal)
					return;

				if (pLocal->GetLifeState() != LIFE_ALIVE)
					return;

				float notNeeded1[3], notNeeded2[3];
				float forwardTrace[3];
				float spreadFix[3];

				Vector vPunch = pLocal->vecGetPunchAngle();
				Vector vTraceForward = Vector(0, 0, 0);
				Vector vTraceAngle = pUserCmd->viewangles;
				float nospreadInAngle[3] = { pUserCmd->viewangles.x, pUserCmd->viewangles.y, pUserCmd->viewangles.z };

				g_NoSpread.applySpread(pUserCmd->random_seed, nospreadInAngle, spreadFix);

				if (!norecoil)
					vTraceAngle += vPunch + vPunch;

				if (!nospread)
					vTraceAngle -= Vector(spreadFix[0], spreadFix[1], spreadFix[2]);

				float flTraceAngle[3] = { vTraceAngle[0], vTraceAngle[1], vTraceAngle[2] };

				g_NoSpread.angleVectors(flTraceAngle, forwardTrace, notNeeded1, notNeeded2);
				vTraceForward.Init(forwardTrace[0], forwardTrace[1], forwardTrace[2]);

				Vector vTraceStart = pLocal->GetEyePosition();
				Vector vTraceEnd = vTraceStart + (vTraceForward * 8192.0f);

				ValveSDK::CTrace::Ray_t ray;
				ValveSDK::CTrace::trace_t tr;
				ValveSDK::CTrace::CTraceFilter filter;
				filter.pSkip = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

				ray.Init(vTraceStart, vTraceEnd);
				g_pEngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

				traceEndPos = tr.endpos;

				if (!tr.m_pEnt)
					return;

				CBaseEntity* pEntity = g_pEntList->GetClientEntity(tr.m_pEnt->GetIndex());


				if (tr.m_pEnt->GetIndex() > 0 && tr.m_pEnt->GetIndex() <= 66) {}
				else
					return;

				if (g_Console.getValue("trigger_spot") == 1)
				{
					if (tr.hitgroup != 1)
						return;
				}

				bool isInEnemyTeam = pLocal->GetTeamNumber() != pEntity->GetTeamNumber();

				int begTime = clock();

				if (isInEnemyTeam)
				{
					tickCount = (DWORD)GetTickCount();
					isTriggering = true;
					unsigned short HighOrderBit = 1 << 15; // 1 = KeyDown; 0 = KeyUp;
					BOOL bIsTriggerKeyPressed = FALSE;
					GetKeyState(g_Console.getValue("trigger_key")) & HighOrderBit ? bIsTriggerKeyPressed = TRUE : bIsTriggerKeyPressed = FALSE;
					if (g_Console.getValue("trigger_key") == 0 || bIsTriggerKeyPressed)
						pUserCmd->buttons |= IN_ATTACK;
				}
			}
		}
	}
}
//
VOID glow(CBaseEntity *pEnt, int r, int g, int b, int a)
{
	auto entity_glow_index = *(UINT32*)((uintptr_t)pEnt + NetVars::Player::m_flFlashDuration + 0x18);

	// get a pointer to the m_GlowObjectDefinitions array (seen in glow_outline_effect.h)
	auto glow_object_array = *(GlowObject_t**)g_pGlowObjectManager;

	// get a pointer to the glow object at your entity's glow index
	auto glow_object = &glow_object_array[entity_glow_index];
	
	float glowr = (1 / 255.0f)*r;
	float glowg = (1 / 255.0f)*g;
	float glowb = (1 / 255.0f)*b;
	float glowa = (1 / 255.0f)*a;
	glow_object->m_bShouldGlow = true;
	glow_object->m_bUnknown = false;
	glow_object->m_vGlowColor = Vector((1 / 255.0f)*r, (1 / 255.0f)*g, (1 / 255.0f)*b);
	glow_object->m_flGlowAlpha = (1 / 255.0f)*a;
}
//=================================================================================================
VOID DrawEntityESP ( VOID )
{
	Vector vScreen;

	if ( !g_pEntList )
		return;

	for ( size_t iIndex = 64; iIndex <= 2048; iIndex++ )
	{
		CBaseEntity* pEnt = g_pEntList->GetClientEntity ( iIndex );

		if ( !pEnt || !g_pModel )
			continue;

		if ( pEnt->GetAbsAngles ( ).x == 0.0f && pEnt->GetAbsAngles ( ).z == 0.0f )
			continue;
	
		std::string sCurWeapon = g_pModel->GetModelName ( pEnt->GetModel ( ) );

		if (sCurWeapon.find("models/weapons/w_", 0) != std::string::npos)
		{
			/*glow(pEnt,255,255,255);*/
			if (sCurWeapon.find("silencer", 0) != std::string::npos)
				sCurWeapon.erase(0, sCurWeapon.find_first_of("_", std::string("models/weapons/w_").length()) + 1);
			else
				sCurWeapon.erase(0, sCurWeapon.find_last_of("_") + 1);

			sCurWeapon.erase(sCurWeapon.find_last_of("."), 4);

			if (WorldToScreen(pEnt->GetAbsOrigin(), vScreen) && g_Console.getValue("esp_entity") == 1)
				g_TableFont.DrawString(TRUE, vScreen.x, vScreen.y, g_Console.getValue("ent_r"), g_Console.getValue("ent_g"), g_Console.getValue("ent_b"), g_Console.getValue("ent_a"), sCurWeapon.c_str());

		}
		
	}
}
//=================================================================================================
void initCommands()
{
	// AIM
	g_Console.addCvar("aim_active", 1, false);
	g_Console.addCvar("aim_key", 1, false);
	g_Console.addCvar("aim_fov", 2, false);
	g_Console.addCvar("aim_hitbox", 10, false);
	g_Console.addCvar("aim_smooth_x", 3, false);
	g_Console.addCvar("aim_smooth_y", 3, false);
	g_Console.addCvar("aim_targetby", 1, false);
	g_Console.addCvar("aim_silent", 0, false);
	g_Console.addCvar("aim_psilent", 0, false);

	// ESP
	g_Console.addCvar("esp_enemyonly", 1, false);
	g_Console.addCvar("esp_box", 0, false);
	g_Console.addCvar("esp_name", 0, false);
	g_Console.addCvar("esp_health", 0, false);
	g_Console.addCvar("esp_glow", 0, false);
	g_Console.addCvar("esp_bomb", 0, false);
	g_Console.addCvar("esp_bone", 0, false);
	g_Console.addCvar("esp_hitbox", 0, false);
	g_Console.addCvar("esp_weapon", 0, false);
	g_Console.addCvar("esp_entity", 0, false);
	g_Console.addCvar("esp_visibleonly", 0, false);
	g_Console.addCvar("esp_helmet", 0, false);
	g_Console.addCvar("esp_flashed", 0, false);

	g_Console.addCvar("trigger_key", 6, false);
	g_Console.addCvar("trigger_active",1,false);
	g_Console.addCvar("trigger_spot", 0, false);
	g_Console.addCvar("vis_radar", 0, false);

	g_Console.addCvar("misc_scs", 1, false);
	g_Console.addCvar("misc_rcs", 1, false);
	g_Console.addCvar("misc_nospread",0,false);
	g_Console.addCvar("misc_norecoil",0,false);
	g_Console.addCvar("misc_crosshair",0,false);
	g_Console.addCvar("misc_hitmarker",1,false);
	g_Console.addCvar("misc_noflash", 0, false);
	g_Console.addCvar("misc_autopistol", 0,false);
	g_Console.addCvar("misc_bhop",0,false);	
	g_Console.addCvar("misc_strafe", 0, false);
	g_Console.addCvar("vis_chams", 1, false);
	//Style cvars	
	g_Console.addCvar("ct_r",30,true);
	g_Console.addCvar("ct_g",144,true);
	g_Console.addCvar("ct_b",255,true);
		
	g_Console.addCvar("t_r",255,true);
	g_Console.addCvar("t_g",0,true);
	g_Console.addCvar("t_b",0,true);

	g_Console.addCvar("vis_t_r",255,true);
	g_Console.addCvar("vis_t_g",255,true);
	g_Console.addCvar("vis_t_b",0,true);

	g_Console.addCvar("vis_ct_r", 0, true);
	g_Console.addCvar("vis_ct_g", 255, true);
	g_Console.addCvar("vis_ct_b", 0, true);

	g_Console.addCvar("ent_r",255,true);
	g_Console.addCvar("ent_g",255,true);
	g_Console.addCvar("ent_b",0,true);
	g_Console.addCvar("ent_a", 255, true);

	g_Console.addCvar("bomb_r",255,true);
	g_Console.addCvar("bomb_g",255,true);
	g_Console.addCvar("bomb_b",255,true);
	g_Console.addCvar("bomb_a", 255, true);

	g_Console.addCvar("mainframe_r",30,true);
	g_Console.addCvar("mainframe_g",30,true);
	g_Console.addCvar("mainframe_b",30,true);
	g_Console.addCvar("mainframe_a",170,true);

	g_Console.addCvar("mainbox_r",0,true);
	g_Console.addCvar("mainbox_g",0,true);
	g_Console.addCvar("mainbox_b",0,true);	
	g_Console.addCvar("mainbox_a",255,true);

	g_Console.addCvar("input_r",0,true);
	g_Console.addCvar("input_g",0,true);
	g_Console.addCvar("input_b",0,true);
	g_Console.addCvar("input_a",255,true);

	g_Console.addCvar("header_r",0,true);
	g_Console.addCvar("header_g",0,true);
	g_Console.addCvar("header_b",0,true);
	g_Console.addCvar("header_a",255,true);

	g_Console.addCvar("headerframe_r",0,true);
	g_Console.addCvar("headerframe_g",0,true);
	g_Console.addCvar("headerframe_b",0,true);
	g_Console.addCvar("headerframe_a",255,true);
		
	g_Console.addCvar("stringcons_r",238,true);
	g_Console.addCvar("stringcons_g",0,true);
	g_Console.addCvar("stringcons_b",0,true);
	
	g_Console.addCvar("cmdcolor_r",255,true);
	g_Console.addCvar("cmdcolor_g",255,true);
	g_Console.addCvar("cmdcolor_b",255,true);

	g_Console.addCvar("printfont_r",255,true);
	g_Console.addCvar("printfont_g",255,true);
	g_Console.addCvar("printfont_b",255,true);

	g_Console.addCvar("chams_t_r", 255, true);
	g_Console.addCvar("chams_t_g", 0, true);
	g_Console.addCvar("chams_t_b", 0, true);
	g_Console.addCvar("chams_ct_r", 0, true);
	g_Console.addCvar("chams_ct_g", 160, true);
	g_Console.addCvar("chams_ct_b", 255, true);

	g_Console.addCvar("vis_chams_t_r", 255, true);
	g_Console.addCvar("vis_chams_t_g", 255, true);
	g_Console.addCvar("vis_chams_t_b", 0, true);
	g_Console.addCvar("vis_chams_ct_r", 0, true);
	g_Console.addCvar("vis_chams_ct_g", 255, true);
	g_Console.addCvar("vis_chams_ct_b", 0, true);

	g_Console.addCvar("glow_alpha", 170, true);

}
CBaseCombatWeapon* GetBaseCombatActiveWeapon(CBaseEntity* pEntity)
{
	EHANDLE hHandle = (EHANDLE)*(PDWORD)((DWORD)pEntity + NetVars::Player::m_hActiveWeapon);
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pEntList->GetClientEntityFromHandle(hHandle);
	return pWeapon;
}
void DrawHealthBox( int x, int y, int r,int g,int b, int CurHealth, int MaxHealth ){
	if( CurHealth == 0 || MaxHealth == 0 )
		return;
	float mx = MaxHealth / 4;
	float wx = CurHealth / 4;
	if( mx == 0 ) return;
	x -= ( mx / 2 );
	g_Draw.FillRGBA(x-1,y-1,mx + 2,4+2,r,g,b,255);
	g_Draw.FillRGBA( x, y, wx, 4,r,g,b,255 );
}//well make rs later
bool IsCurWepGrenade(CBaseEntity* pEntity){
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	if (m_pWeapon){
		if (strstr(g_pModel->GetModelName(m_pWeapon->GetModel()), "eq"))
			return true;
	}
	return false;
}
bool IsCurWepKnife(CBaseEntity* pEntity){
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	if (m_pWeapon){
		if (strstr(g_pModel->GetModelName(m_pWeapon->GetModel()), "knife"))
			return true;
	}
	return false;
}
bool IsPistol(CBaseEntity* pEntity){
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	int iWeaponID = g_NoSpread.GetWeaponId(m_pWeapon);

	switch (iWeaponID)
	{
	case WEAPON_NONE:{return false; break; }
	case WEAPON_KNIFE:{return false; break; }
	case WEAPON_DEAGLE:{return true; break; }
	case WEAPON_ELITE:{return true; break; }
	case WEAPON_FIVESEVEN:{return true; break; }
	case WEAPON_GLOCK:{return true; break; }
	case WEAPON_P228:{return true; break; }
	case WEAPON_USP:{return false; break; }
	case WEAPON_AK47:{return false; break; }
	case WEAPON_AUG:{return false; break; }
	case WEAPON_AWP:{return false; break; }
	case WEAPON_FAMAS:{return false; break; }
	case WEAPON_G3SG1:{return false; break; }
	case WEAPON_GALIL:{return false; break; }
	case WEAPON_GALILAR:{return false; break; }
	case WEAPON_M249:{return false; break; }
	case WEAPON_M3:{return false; break; }
	case WEAPON_M4A1:{return false; break; }
	case WEAPON_MAC10:{return false; break; }
	case WEAPON_MP5NAVY:{return false; break; }
	case WEAPON_P90:{return false; break; }
	case WEAPON_SCOUT:{return false; break; }
	case WEAPON_SG550:{return false; break; }
	case WEAPON_SG552:{return false; break; }
	case WEAPON_TMP:{return false; break; }
	case WEAPON_UMP45:{return false; break; }
	case WEAPON_XM1014:{return false; break; }
	case WEAPON_BIZON:{return false; break; }
	case WEAPON_MAG7:{return false; break; }
	case WEAPON_NEGEV:{return false; break; }
	case WEAPON_SAWEDOFF:{return false; break; }
	case WEAPON_TEC9:{return true; break; }
	case WEAPON_TASER:{return false; break; }
	case WEAPON_HKP2000:{return true; break; }
	case WEAPON_MP7:{return false; break; }
	case WEAPON_MP9:{return false; break; }
	case WEAPON_NOVA:{return false; break; }
	case WEAPON_P250:{return true; break; }
	case WEAPON_SCAR17:{return false; break; }
	case WEAPON_SCAR20:{return false; break; }
	case WEAPON_SG556:{return false; break; }
	case WEAPON_SSG08:{return false; break; }
	case WEAPON_KNIFEGG:{return false; break; }
	case WEAPON_FLASHBANG:{return false; break; }
	case WEAPON_HEGRENADE:{return false; break; }
	case WEAPON_SMOKEGRENADE:{return false; break; }
	case WEAPON_MOLOTOV:{return false; break; }
	case WEAPON_DECOY:{return false; break; }
	case WEAPON_INCGRENADE:{return false; break; }
	case WEAPON_C4:{return false; break; }
	}
}
bool IsCurWepBomb(CBaseEntity* pEntity){
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	if (m_pWeapon){ if (strstr(g_pModel->GetModelName(m_pWeapon->GetModel()), "c4")) return true; }
	return false;
}
char* GetWeaponName(CBaseEntity *pEntity)
{
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	char m_szPrintName[100];
	int iWeaponID = g_NoSpread.GetWeaponId(m_pWeapon);

	switch (iWeaponID)
	{
	case WEAPON_NONE:{strcpy(m_szPrintName, " "); break; }
	case WEAPON_KNIFE:{strcpy(m_szPrintName, "knife"); break; }
	case WEAPON_DEAGLE:{strcpy(m_szPrintName, "Deagle"); break; }
	case WEAPON_ELITE:{strcpy(m_szPrintName, "Dual Berettas"); break; }
	case WEAPON_FIVESEVEN:{strcpy(m_szPrintName, "Five-Seven"); break; }
	case WEAPON_GLOCK:{strcpy(m_szPrintName, "Glock"); break; }
	case WEAPON_P228:{strcpy(m_szPrintName, "P228"); break; }
	case WEAPON_USP:{strcpy(m_szPrintName, "USP"); break; }
	case WEAPON_AK47:{strcpy(m_szPrintName, "AK47"); break; }
	case WEAPON_AUG:{strcpy(m_szPrintName, "AUG"); break; }
	case WEAPON_AWP:{strcpy(m_szPrintName, "AWP"); break; }
	case WEAPON_FAMAS:{strcpy(m_szPrintName, "FAMAS"); break; }
	case WEAPON_G3SG1:{strcpy(m_szPrintName, "G3SG1"); break; }
	case WEAPON_GALIL:{strcpy(m_szPrintName, "Galil"); break; }
	case WEAPON_GALILAR:{strcpy(m_szPrintName, "Galil-AR"); break; }
	case WEAPON_M249:{strcpy(m_szPrintName, "M249"); break; }
	case WEAPON_M3:{strcpy(m_szPrintName, "M3"); break; }
	case WEAPON_M4A1:{strcpy(m_szPrintName, "M4A1"); break; }
	case WEAPON_MAC10:{strcpy(m_szPrintName, "MAC10"); break; }
	case WEAPON_MP5NAVY:{strcpy(m_szPrintName, "MP5-Navy"); break; }
	case WEAPON_P90:{strcpy(m_szPrintName, "P90"); break; }
	case WEAPON_SCOUT:{strcpy(m_szPrintName, "Scout"); break; }
	case WEAPON_SG550:{strcpy(m_szPrintName, "SG550"); break; }
	case WEAPON_SG552:{strcpy(m_szPrintName, "SG552"); break; }
	case WEAPON_TMP:{strcpy(m_szPrintName, "TMP"); break; }
	case WEAPON_UMP45:{strcpy(m_szPrintName, "UMP45"); break; }
	case WEAPON_XM1014:{strcpy(m_szPrintName, "XM1014"); break; }
	case WEAPON_BIZON:{strcpy(m_szPrintName, "PP-Bizon"); break; }
	case WEAPON_MAG7:{strcpy(m_szPrintName, "MAG-7"); break; }
	case WEAPON_NEGEV:{strcpy(m_szPrintName, "Negev"); break; }
	case WEAPON_SAWEDOFF:{strcpy(m_szPrintName, "Sawed-Off Shotgun"); break; }
	case WEAPON_TEC9:{strcpy(m_szPrintName, "Tec-9"); break; }
	case WEAPON_TASER:{strcpy(m_szPrintName, "Zeus"); break; }
	case WEAPON_HKP2000:{strcpy(m_szPrintName, "P2000"); break; }
	case WEAPON_MP7:{strcpy(m_szPrintName, "MP7"); break; }
	case WEAPON_MP9:{strcpy(m_szPrintName, "MP9"); break; }
	case WEAPON_NOVA:{strcpy(m_szPrintName, "Nova"); break; }
	case WEAPON_P250:{strcpy(m_szPrintName, "p250"); break; }
	case WEAPON_SCAR17:{strcpy(m_szPrintName, "SCAR17"); break; }
	case WEAPON_SCAR20:{strcpy(m_szPrintName, "SCAR20"); break; }
	case WEAPON_SG556:{strcpy(m_szPrintName, "SG556"); break; }
	case WEAPON_SSG08:{strcpy(m_szPrintName, "SSG08"); break; }
	case WEAPON_KNIFEGG:{strcpy(m_szPrintName, "Golden Knife"); break; }
	case WEAPON_FLASHBANG:{strcpy(m_szPrintName, "Flashbang"); break; }
	case WEAPON_HEGRENADE:{strcpy(m_szPrintName, "HE-Grenade"); break; }
	case WEAPON_SMOKEGRENADE:{strcpy(m_szPrintName, "Smoke"); break; }
	case WEAPON_MOLOTOV:{strcpy(m_szPrintName, "Molotov"); break; }
	case WEAPON_DECOY:{strcpy(m_szPrintName, "Decoy"); break; }
	case WEAPON_INCGRENADE:{strcpy(m_szPrintName, "Molotov"); break; }
	case WEAPON_C4:{strcpy(m_szPrintName, "Bomb"); break; }
	}
	return m_szPrintName;
}
void WeaponESP(int x, int y, int r, int g, int b, int a, CBaseEntity *pEntity)
{
	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pEntity);
	char m_szPrintName[100];
	int iWeaponID = g_NoSpread.GetWeaponId(m_pWeapon);

	switch (iWeaponID)
	{
	case WEAPON_NONE:{strcpy(m_szPrintName, " "); break; }
	case WEAPON_KNIFE:{strcpy(m_szPrintName, "knife"); break; }
	case WEAPON_DEAGLE:{strcpy(m_szPrintName, "Deagle"); break; }
	case WEAPON_ELITE:{strcpy(m_szPrintName, "Dual Berettas"); break; }
	case WEAPON_FIVESEVEN:{strcpy(m_szPrintName, "Five-Seven"); break; }
	case WEAPON_GLOCK:{strcpy(m_szPrintName, "Glock"); break; }
	case WEAPON_P228:{strcpy(m_szPrintName, "P228"); break; }
	case WEAPON_USP:{strcpy(m_szPrintName, "USP"); break; }
	case WEAPON_AK47:{strcpy(m_szPrintName, "AK47"); break; }
	case WEAPON_AUG:{strcpy(m_szPrintName, "AUG"); break; }
	case WEAPON_AWP:{strcpy(m_szPrintName, "AWP"); break; }
	case WEAPON_FAMAS:{strcpy(m_szPrintName, "FAMAS"); break; }
	case WEAPON_G3SG1:{strcpy(m_szPrintName, "G3SG1"); break; }
	case WEAPON_GALIL:{strcpy(m_szPrintName, "Galil"); break; }
	case WEAPON_GALILAR:{strcpy(m_szPrintName, "Galil-AR"); break; }
	case WEAPON_M249:{strcpy(m_szPrintName, "M249"); break; }
	case WEAPON_M3:{strcpy(m_szPrintName, "M3"); break; }
	case WEAPON_M4A1:{strcpy(m_szPrintName, "M4A1"); break; }
	case WEAPON_MAC10:{strcpy(m_szPrintName, "MAC10"); break; }
	case WEAPON_MP5NAVY:{strcpy(m_szPrintName, "MP5-Navy"); break; }
	case WEAPON_P90:{strcpy(m_szPrintName, "P90"); break; }
	case WEAPON_SCOUT:{strcpy(m_szPrintName, "Scout"); break; }
	case WEAPON_SG550:{strcpy(m_szPrintName, "SG550"); break; }
	case WEAPON_SG552:{strcpy(m_szPrintName, "SG552"); break; }
	case WEAPON_TMP:{strcpy(m_szPrintName, "TMP"); break; }
	case WEAPON_UMP45:{strcpy(m_szPrintName, "UMP45"); break; }
	case WEAPON_XM1014:{strcpy(m_szPrintName, "XM1014"); break; }
	case WEAPON_BIZON:{strcpy(m_szPrintName, "PP-Bizon"); break; }
	case WEAPON_MAG7:{strcpy(m_szPrintName, "MAG-7"); break; }
	case WEAPON_NEGEV:{strcpy(m_szPrintName, "Negev"); break; }
	case WEAPON_SAWEDOFF:{strcpy(m_szPrintName, "Sawed-Off Shotgun"); break; }
	case WEAPON_TEC9:{strcpy(m_szPrintName, "Tec-9"); break; }
	case WEAPON_TASER:{strcpy(m_szPrintName, "Zeus"); break; }
	case WEAPON_HKP2000:{strcpy(m_szPrintName, "P2000"); break; }
	case WEAPON_MP7:{strcpy(m_szPrintName, "MP7"); break; }
	case WEAPON_MP9:{strcpy(m_szPrintName, "MP9"); break; }
	case WEAPON_NOVA:{strcpy(m_szPrintName, "Nova"); break; }
	case WEAPON_P250:{strcpy(m_szPrintName, "p250"); break; }
	case WEAPON_SCAR17:{strcpy(m_szPrintName, "SCAR17"); break; }
	case WEAPON_SCAR20:{strcpy(m_szPrintName, "SCAR20"); break; }
	case WEAPON_SG556:{strcpy(m_szPrintName, "SG556"); break; }
	case WEAPON_SSG08:{strcpy(m_szPrintName, "SSG08"); break; }
	case WEAPON_KNIFEGG:{strcpy(m_szPrintName, "Golden Knife"); break; }
	case WEAPON_FLASHBANG:{strcpy(m_szPrintName, "Flashbang"); break; }
	case WEAPON_HEGRENADE:{strcpy(m_szPrintName, "HE-Grenade"); break; }
	case WEAPON_SMOKEGRENADE:{strcpy(m_szPrintName, "Smoke"); break; }
	case WEAPON_MOLOTOV:{strcpy(m_szPrintName, "Molotov"); break; }
	case WEAPON_DECOY:{strcpy(m_szPrintName, "Decoy"); break; }
	case WEAPON_INCGRENADE:{strcpy(m_szPrintName, "Molotov"); break; }
	case WEAPON_C4:{strcpy(m_szPrintName, "Bomb"); break; }
	}

	g_TableFont.DrawString(true, x, y, r, g, b, a, "%s", m_szPrintName);
}
//=================================================================================================
VOID WINAPI DrawModel_Hooked(void * context, void * state, const ModelRenderInfo_t & pInfo, matrix3x4_t * pCustomBoneToWorld)
{
	static DWORD dwOriginalDrawModelExecuteVMT = g_pModelRenderVMT->dwGetMethodAddress(21);

	bool active = false;
	float flColor[4];

	ValveSDK::IMaterial *materials[32];
	ValveSDK::CEngineClient::player_info_t pInfoo;

	studiohdr_t* hdr = g_pModel->GetStudiomodel(pInfo.pModel);
	std::string charModelName = g_pModel->GetModelName(pInfo.pModel);
	int var = g_Console.getValue("vis_chams");
	if (hdr && pInfo.pModel)
	{
		CBaseEntity* pEntity = g_pEntList->GetClientEntity(pInfo.entity_index);
		CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (pEntity)
		{
			if (g_pEngine->GetPlayerInfo(pInfo.entity_index, &pInfoo) && g_Console.getValue("vis_chams") > 0)
			{
				g_pModel->GetModelMaterials(pInfo.pModel, hdr->numtextures, materials);
				for (int i = 0; i < hdr->numtextures; i++)
				{
					ValveSDK::IMaterial* mats = materials[i];

					if (!mats)
						continue;

					if (g_Console.getValue("esp_enemyonly") && pEntity->GetTeamNumber() == pLocal->GetTeamNumber())
							continue;

					if (var > 1)
					{
						mats->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mats->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
						mats->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, false);
					}

					//non visible
					if (pEntity->GetTeamNumber() == 2) //T
					{
						flColor[0] = g_Console.getValue("chams_t_r");
						flColor[1] = g_Console.getValue("chams_t_g");
						flColor[2] = g_Console.getValue("chams_t_b");
						flColor[3] = 255;
					}
					else if (pEntity->GetTeamNumber() == 3) //CT
					{
						flColor[0] = g_Console.getValue("chams_ct_r");
						flColor[1] = g_Console.getValue("chams_ct_g");
						flColor[2] = g_Console.getValue("chams_ct_b");
						flColor[3] = 255;
					}
					g_pRenderView->SetBlend((255 / 255.0f));
					g_pRenderView->SetColorModulation(flColor);

					_asm
					{
						PUSH pCustomBoneToWorld
							PUSH pInfo
							PUSH state
							PUSH context
							MOV ECX, g_pModelRender
							CALL dwOriginalDrawModelExecuteVMT
					}
					if (var > 1)
					{
						mats->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mats->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
						mats->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, false);
					}
					//visible
					if (pEntity->GetTeamNumber() == 2) //T
					{
						flColor[0] = g_Console.getValue("vis_chams_t_r");
						flColor[1] = g_Console.getValue("vis_chams_t_g");
						flColor[2] = g_Console.getValue("vis_chams_t_b");
						flColor[3] = 255;
					}
					else if (pEntity->GetTeamNumber() == 3) //CT
					{
						flColor[0] = g_Console.getValue("vis_chams_ct_r");
						flColor[1] = g_Console.getValue("vis_chams_ct_g");
						flColor[2] = g_Console.getValue("vis_chams_ct_b");
						flColor[3] = 200;
					}
					g_pRenderView->SetBlend((255 / 255.0f));
					g_pRenderView->SetColorModulation(flColor);

					_asm
					{
						PUSH pCustomBoneToWorld
							PUSH pInfo
							PUSH state
							PUSH context
							MOV ECX, g_pModelRender
							CALL dwOriginalDrawModelExecuteVMT
					}


					if (!active)
						active = true;
				}
			}
		}
	}

	if (active)
		return;

	_asm
	{
		PUSH pCustomBoneToWorld
			PUSH pInfo
			PUSH state
			PUSH context
			MOV ECX, g_pModelRender
			CALL dwOriginalDrawModelExecuteVMT
	}
}
void Draw3DBox(Vector or, Vector min, Vector max, QAngle ang, int r, int g, int b)
{
	g_pSurface->DrawSetColor(r, g, b, 255);

	Vector p1, p2, p3, p4, p5, p6, p7, p8;
	Vector s1, s2, s3, s4, s5, s6, s7, s8;

	p1 = or + min;
	p2 = or + Vector(max.x, min.y, min.z);
	p3 = or + Vector(min.x, max.y, min.z);
	p4 = or + Vector(max.x, max.y, min.z);
	p5 = or + max;
	p6 = or + Vector(min.x, max.y, max.z);
	p7 = or + Vector(min.x, min.y, max.z);
	p8 = or + Vector(max.x, min.y, max.z);


	WorldToScreen(p1, s1);
	WorldToScreen(p2, s2);
	WorldToScreen(p3, s3);
	WorldToScreen(p4, s4);
	WorldToScreen(p5, s5);
	WorldToScreen(p6, s6);
	WorldToScreen(p7, s7);
	WorldToScreen(p8, s8);

	g_pSurface->DrawLine(s1.x, s1.y, s2.x, s2.y);
	g_pSurface->DrawLine(s1.x, s1.y, s3.x, s3.y);
	g_pSurface->DrawLine(s1.x, s1.y, s7.x, s7.y);
	g_pSurface->DrawLine(s2.x, s2.y, s8.x, s8.y);
	g_pSurface->DrawLine(s2.x, s2.y, s4.x, s4.y);
	g_pSurface->DrawLine(s3.x, s3.y, s4.x, s4.y);
	g_pSurface->DrawLine(s3.x, s3.y, s6.x, s6.y);
	g_pSurface->DrawLine(s4.x, s4.y, s5.x, s5.y);
	g_pSurface->DrawLine(s5.x, s5.y, s6.x, s6.y);
	g_pSurface->DrawLine(s5.x, s5.y, s8.x, s8.y);
	g_pSurface->DrawLine(s6.x, s6.y, s7.x, s7.y);
	g_pSurface->DrawLine(s7.x, s7.y, s8.x, s8.y);

}
void Draw3DBox(Vector* points, int r, int g, int b, int a)
{
	Vector startPos, endPos;

	for (int i = 0; i < 3; i++)
	{
		if (WorldToScreen(points[i], startPos))
			if (WorldToScreen(points[i + 1], endPos))
				g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);
	}
	if (WorldToScreen(points[0], startPos))
		if (WorldToScreen(points[3], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);

	for (int i = 4; i < 7; i++)
	{
		if (WorldToScreen(points[i], startPos))
			if (WorldToScreen(points[i + 1], endPos))
				g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);
	}
	if (WorldToScreen(points[4], startPos))
		if (WorldToScreen(points[7], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);

	if (WorldToScreen(points[0], startPos))
		if (WorldToScreen(points[6], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);

	if (WorldToScreen(points[1], startPos))
		if (WorldToScreen(points[5], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);

	if (WorldToScreen(points[2], startPos))
		if (WorldToScreen(points[4], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);

	if (WorldToScreen(points[3], startPos))
		if (WorldToScreen(points[7], endPos))
			g_Draw.LineRGBA(startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a);
}
void HitboxESP(CBaseEntity* targetPlayer, matrix3x4* matrix, int bone, Vector max, Vector min, int r, int g, int b, int a)
{
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };
	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++)
	{
		points[i].x *= 0.9;
		points[i].y *= 0.9;
		points[i].z *= 0.9;
		g_Aimbot.VectorTransform(points[i], matrix[bone], pointsTransformed[i]);
	}
	Draw3DBox(pointsTransformed, r, g, b, a);
}
void hitboxes(CBaseEntity* pPlayer, int r, int g, int b, int a)
{
	matrix3x4 matrix[128];
	if (!pPlayer->SetupBones(matrix, 128, 0x00000100, g_pEngine->GetLastTimeStamp()))
		return;

	studiohdr_t* hdr = g_pModel->GetStudiomodel(pPlayer->GetModel());
	mstudiohitboxset_t* set = hdr->pHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++)
	{
		mstudiobbox_t* hitbox = set->pHitbox(g_Console.getValue("aim_hitbox"));
		if (!hitbox)
			continue;

		HitboxESP(pPlayer, matrix, hitbox->bone, hitbox->bbmax, hitbox->bbmin, r, g, b, a);

	}
}
void fullhitboxes(CBaseEntity* pPlayer, int r, int g, int b, int a)
{
	matrix3x4 matrix[128];
	if (!pPlayer->SetupBones(matrix, 128, 0x00000100, g_pEngine->GetLastTimeStamp()))
		return;

	studiohdr_t* hdr = g_pModel->GetStudiomodel(pPlayer->GetModel());
	mstudiohitboxset_t* set = hdr->pHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++)
	{
		mstudiobbox_t* hitbox = set->pHitbox(i);
		if (!hitbox)
			continue;
		int bone = hitbox->bone;
		Vector vMaxUntransformed = hitbox->bbmax;
		Vector vMinUntransformed = hitbox->bbmin;

		HitboxESP(pPlayer, matrix, bone, vMaxUntransformed, vMinUntransformed, r, g, b, a);
	}
}
void __stdcall hkdPaintTraverse( unsigned vguiPanel, bool forceRepaint, bool allowForce)
{
	static DWORD dwOrgPaintTraverse = g_pPaintTraverseVMT->dwGetMethodAddress ( 41 ); 	
	
	__asm 
	{  
		PUSH allowForce; 
		PUSH forceRepaint; 
		PUSH vguiPanel; 
		MOV ECX, g_pPanel; 
		CALL dwOrgPaintTraverse; 
	} 

	static bool bDidOnce = false;

	if(bDidOnce == false)
	{
		//Base::Debug::LOG("Font initalized");		
		initCommands();
		gVars.menu_page2 = 1;
		g_Draw.init(20);				
		bDidOnce = true;
	}
	
	const char* pszPanelName = g_pPanel->GetName( vguiPanel );
	bool isValidPanel = false;

	if ( pszPanelName && pszPanelName [ 0 ] == 'M' && pszPanelName [ 3 ] == 'S' &&
		pszPanelName [ 9 ] == 'T' && pszPanelName [ 12 ] == 'P' )
		isValidPanel = true;

	if(!isValidPanel)
		return;

	int x,y;
	g_pEngine->GetScreenSize(x,y);
	x *= 0.5;
	y *= 0.5; 

	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

	int gCenterX = (iScreenWidth / 2);
	int gCenterY = (iScreenHeight / 2);

	CBaseEntity* pMe = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	g_Draw.DrawStringA(g_Draw.m_Font, true, 250, 20, 255, 255, 255, 255, "SEARCHYWEBZ CSGO");
	
	//g_Timer.Start(3000);
	//g_Draw.DrawStringA(g_Draw.m_Font, true, 250, 50, 255, 255, 255, 255, "Timeleft: %d", g_Timer.TimeLeft());
	Vector vPlayer,vScreen;

	INT tSize = g_pEntList->GetHighestEntityIndex();
	INT r, g, b, a;

	bool doOnce;
	if (g_Console.getValue("vis_radar") == 1)
	{
		gRadar.DrawRadarBack();
		doOnce = false;
	}

	if(g_Console.getValue("esp_entity") == 1)
		DrawEntityESP();

	if (g_Console.getValue("misc_noflash") == 1)
	{
		if (g_pEngine->isinGame())
		{
			float* alphaflash = MakePtr(float*, pMe, NetVars::Player::m_flFlashMaxAlpha);
			*alphaflash = 0;
		}
	}
	for(INT ax = 0;ax < tSize;ax++)
	{
		CBaseEntity* pBaseEntity = g_pEntList->GetClientEntity(ax);
		
		if(pBaseEntity == NULL)
			continue;
		
		std::string bomb = (LPSTR)g_pModel->GetModelName(pBaseEntity->GetModel());		
	

		if(strstr(bomb.data(),"ied.mdl"))
		{
			ULONG hOwnerEntity = *(PULONG)((DWORD)pBaseEntity +  0x148);
			CBaseEntity* pOwnerEntity = (CBaseEntity*)g_pEntList->GetClientEntityFromHandle(hOwnerEntity);	
			
			if(pOwnerEntity == NULL)
				continue;

			vPlayer = pOwnerEntity->GetAbsOrigin();						
			if(WorldToScreen(vPlayer,vScreen))
			{				
				if(g_Console.getValue("esp_bomb") == 1)
					g_Draw.DrawStringA(g_Draw.m_Font, true, vScreen.x, vScreen.y, g_Console.getValue("bomb_r"), g_Console.getValue("bomb_g"), g_Console.getValue("bomb_b"), g_Console.getValue("bomb_a"), "bomb");
			}
		}

		if(g_Aimbot.isValidPlayer(pBaseEntity))
		{
			vPlayer = pBaseEntity->GetAbsOrigin();
			vPlayer.z += 32;	

			Vector vLocalEye,vRemoteEye;
			vLocalEye = pMe->GetEyePosition();
			vRemoteEye = pBaseEntity->GetEyePosition();	
			
			if(pBaseEntity == pMe)					
				continue;					

			bool isEnemyVis = g_Aimbot.IsVisible(pMe->GetEyePosition(), pBaseEntity->GetEyePosition(), pBaseEntity);

			if(pBaseEntity->GetTeamNumber() == 2 )
				isEnemyVis ? r = g_Console.getValue("vis_t_r") : r = g_Console.getValue("t_r"), isEnemyVis ? g = g_Console.getValue("vis_t_g") : g = g_Console.getValue("t_g"), isEnemyVis ? b = g_Console.getValue("vis_t_b") : b = g_Console.getValue("t_b"), isEnemyVis ? a = 255 : a = 255;
			else if (pBaseEntity->GetTeamNumber() == 3)
				isEnemyVis ? r = g_Console.getValue("vis_ct_r") : r = g_Console.getValue("ct_r"), isEnemyVis ? g = g_Console.getValue("vis_ct_g") : g = g_Console.getValue("ct_g"), isEnemyVis ? b = g_Console.getValue("vis_ct_b") : b = g_Console.getValue("ct_b"), isEnemyVis ? a = 255 : a = 255;

			int glowalpha = g_Console.getValue("glow_alpha");

			if (g_Console.getValue("esp_visibleonly"))
			{
				isEnemyVis ? a = 255 : a = 0;
			}

			

			ValveSDK::CEngineClient::player_info_t info;
			if(!g_pEngine->GetPlayerInfo(ax,&info))
				continue;				
			
			Vector vTop,vBot;
			vBot = pBaseEntity->GetAbsOrigin();

			int iFlags = pBaseEntity->GetFlags();
			int iHealth = pBaseEntity->GetHealth();

			if(iFlags & FL_DUCKING)
				vTop = vBot + Vector(0,0,50.f);
			else
				vTop = vBot + Vector(0,0,70.f);

			Vector vScreenBot,vScreenTop;

			Vector vHitbox, vHitboxScreen;

			CBaseCombatWeapon* hasweapon = GetBaseCombatActiveWeapon(pBaseEntity);
			g_Aimbot.GetHitbox ( vHitbox, 0, pBaseEntity->GetIndex ( ) );

			float radardistance = flGetDistance(pBaseEntity->GetvecOrigin(), pMe->GetvecOrigin());

			if (g_Console.getValue("vis_radar") == 1)
			{
				gRadar.DrawRadarPoint(pBaseEntity->GetAbsOrigin(), pMe->GetAbsOrigin(), pMe->GetAbsAngles(), r, g, b, pMe, pBaseEntity);
			}

			if (WorldToScreen(vBot, vScreenBot) && WorldToScreen(vTop, vScreenTop) &&
				WorldToScreen(vHitbox, vHitboxScreen))
			{
				float h = (vScreenBot.y - vScreenTop.y);
				float w = h / 5.f;
				float width2 = (vScreenBot.y - vScreenTop.y) / 4;
				float espY = vScreenBot.y + 5;
				float bsid = vScreenBot.x + width2 + 25;
				if (g_Console.getValue("esp_enemyonly") == 1)
				{
					if (pBaseEntity->GetTeamNumber() != pMe->GetTeamNumber())
					{
						if (g_Console.getValue("esp_glow") == 1)
							glow(pBaseEntity, r, g, b, glowalpha);
						if (g_Console.getValue("esp_bone") == 1)
						{
							Vector vBones[19];
							for (int iBone = 0; iBone < 19; iBone++)
							{
								Vector vPlayer;
								g_Aimbot.GetHitboxPosition(iBone, vPlayer, pMe->GetAbsAngles(), ax);
								WorldToScreen(vPlayer, vBones[iBone]);
							}
							//Legs
							g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[1].x, vBones[1].y, r,g,b, a);
							g_Draw.LineRGBA(vBones[1].x, vBones[1].y, vBones[2].x, vBones[2].y, r,g,b, a);
							g_Draw.LineRGBA(vBones[2].x, vBones[2].y, vBones[3].x, vBones[3].y, r, g, b, a);

							g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[4].x, vBones[4].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[4].x, vBones[4].y, vBones[5].x, vBones[5].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[5].x, vBones[5].y, vBones[6].x, vBones[6].y, r, g, b, a);
							
							//Body
							g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[9].x, vBones[9].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[9].x, vBones[9].y, vBones[10].x, vBones[10].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[11].x, vBones[11].y, r, g, b, a);
							
							//Arms
							g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[12].x, vBones[12].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[12].x, vBones[12].y, vBones[13].x, vBones[13].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[13].x, vBones[13].y, vBones[14].x, vBones[14].y, r, g, b, a);

							g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[15].x, vBones[15].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[15].x, vBones[15].y, vBones[16].x, vBones[16].y, r, g, b, a);
							g_Draw.LineRGBA(vBones[16].x, vBones[16].y, vBones[17].x, vBones[17].y, r, g, b, a);

						}
						if (g_Console.getValue("esp_box") == 1)
						{
							g_Draw.DrawOutlinedRect(vScreenTop.x - width2 - 1, vScreenTop.y - 1, vScreenBot.x + width2 + 1, vScreenBot.y + 1, 0, 0, 0, a);
							g_Draw.DrawOutlinedRect(vScreenTop.x - width2, vScreenTop.y, vScreenBot.x + width2, vScreenBot.y, r, g, b, a);
						}
						if (g_Console.getValue("esp_helmet") == 1)
						{
							if (pBaseEntity->HasHelmet())
							{
								g_TableFont.DrawString(true, vScreenBot.x, espY, r, g, b, a, "HELMET");
								espY += 10;
							}
						}
						if (g_Console.getValue("esp_box") == 2)
							g_Draw.DrawBox(vScreenTop.x - w, vScreenTop.y, w * 2, h, 2.5, r, g, b, 255);

						if (g_Console.getValue("esp_hitbox") == 1)
							hitboxes(pBaseEntity, r, g, b, a);

						if (g_Console.getValue("esp_hitbox") == 2)
							fullhitboxes(pBaseEntity, r, g, b, a);

						if (g_Console.getValue("esp_name") == 1)
						{
							if (g_Console.getValue("esp_health") == 1)
							{
								g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 25, r, g, b, a, "%s", info.name);
							}
							else{
								g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 15, r, g, b, a, info.name);
							}
						}

						if (g_Console.getValue("esp_health") == 1)
						{
							if (pBaseEntity->GetLifeState() == LIFE_ALIVE)
							{
								if (iHealth > 100)
								{
									iHealth = 100;
								}
								int backgrund;
								int healtha;

								if (g_Console.getValue("esp_visibleonly"))
									isEnemyVis ? healtha = 170 : healtha = 0;
								else
									healtha = 170;

								int height = width2 * 2;
								UINT hw = (UINT)(((height - 2) * iHealth) / 100); // Percentage I think
								g_Draw.FillRGBA(vScreenTop.x - width2, vScreenTop.y - 15, height, 10, 0, 0, 0, healtha);
								g_Draw.FillRGBA(vScreenTop.x - width2 + 1, vScreenTop.y - 14, hw, 8, r, g, b, a);

							}
						}

						if (g_Console.getValue("esp_weapon") == 1)
						{
							if (hasweapon)
							{
								WeaponESP(vScreenBot.x, espY, r, g, b, a, pBaseEntity);
								espY += 10;
							}
						}
						if (g_Console.getValue("esp_flashed") == 1)
						{
							if (pBaseEntity->GetFlashDuration() > 0)
							{
								g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 35, r, g, b, a, "Flashed");
							}
						}
					}
				}
				else{
					if (g_Console.getValue("esp_glow") == 1)
						glow(pBaseEntity, r, g, b, glowalpha);

					if (g_Console.getValue("esp_bone") == 1)
					{
						Vector vBones[19];
						for (int iBone = 0; iBone < 19; iBone++)
						{
							Vector vPlayer;
							g_Aimbot.GetHitboxPosition(iBone, vPlayer, pMe->GetAbsAngles(), ax);
							WorldToScreen(vPlayer, vBones[iBone]);
						}
						//Right Leg
						g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[1].x, vBones[1].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[1].x, vBones[1].y, vBones[2].x, vBones[2].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[2].x, vBones[2].y, vBones[3].x, vBones[3].y, r, g, b, a);
						//Left LEg
						g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[4].x, vBones[4].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[4].x, vBones[4].y, vBones[5].x, vBones[5].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[5].x, vBones[5].y, vBones[6].x, vBones[6].y, r, g, b, a);

						//Body
						g_Draw.LineRGBA(vBones[0].x, vBones[0].y, vBones[9].x, vBones[9].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[9].x, vBones[9].y, vBones[10].x, vBones[10].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[11].x, vBones[11].y, r, g, b, a);

						//Right Arm
						g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[12].x, vBones[12].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[12].x, vBones[12].y, vBones[13].x, vBones[13].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[13].x, vBones[13].y, vBones[14].x, vBones[14].y, r, g, b, a);
						//Left Arm
						g_Draw.LineRGBA(vBones[10].x, vBones[10].y, vBones[15].x, vBones[15].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[15].x, vBones[15].y, vBones[16].x, vBones[16].y, r, g, b, a);
						g_Draw.LineRGBA(vBones[16].x, vBones[16].y, vBones[17].x, vBones[17].y, r, g, b, a);

					}
					if (g_Console.getValue("esp_box") == 1)
					{
						g_Draw.DrawOutlinedRect(vScreenTop.x - width2 - 1, vScreenTop.y - 1, vScreenBot.x + width2 + 1, vScreenBot.y + 1, 0, 0, 0, a);
						g_Draw.DrawOutlinedRect(vScreenTop.x - width2, vScreenTop.y, vScreenBot.x + width2, vScreenBot.y, r, g, b, a);

					}
					if (g_Console.getValue("esp_helmet") == 1)
					{
						if (pBaseEntity->HasHelmet())
						{
							g_TableFont.DrawString(true, vScreenBot.x, espY, r, g, b, a, "HELMET");
							espY += 10;
						}
					}
					if (g_Console.getValue("esp_box") == 2)
						g_Draw.DrawBox(vScreenTop.x - w, vScreenTop.y, w * 2, h, 2.5, r, g, b, 255);

					if (g_Console.getValue("esp_name") == 1)
					{
						if (g_Console.getValue("esp_health") == 1)
						{
							g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 25, r, g, b, a, info.name);
						}
						else{
							g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 15, r, g, b, a, info.name);
						}
					}

					if (g_Console.getValue("esp_hitbox") == 1)
						hitboxes(pBaseEntity, r, g, b, a);

					if (g_Console.getValue("esp_hitbox") == 2)
						fullhitboxes(pBaseEntity, r, g, b, a);

					if (g_Console.getValue("esp_health") == 1)
					{
						if (pBaseEntity->GetLifeState() == LIFE_ALIVE)
						{
							if (iHealth > 100)
							{
								iHealth = 100;
							}
							int backgrund;
							int healtha;

							if (g_Console.getValue("esp_visibleonly"))
								isEnemyVis ? healtha = 170 : healtha = 0;
							else
								healtha = 170;

							int height = width2 * 2;
							UINT hw = (UINT)(((height - 2) * iHealth) / 100); // Percentage I think
							g_Draw.FillRGBA(vScreenTop.x - width2, vScreenTop.y - 15, height, 10, 0, 0, 0, healtha);
							g_Draw.FillRGBA(vScreenTop.x - width2 + 1, vScreenTop.y - 14, hw, 8, r, g, b, a);

						}
					}
					if (g_Console.getValue("esp_weapon") == 1)
					{
						if (hasweapon)
						{
							WeaponESP(vScreenBot.x, espY, r, g, b, a, pBaseEntity);
							espY += 10;

						}
					}
					if (g_Console.getValue("esp_flashed") == 1)
					{
						if (pBaseEntity->GetFlashDuration() > 0)
						{
							g_TableFont.DrawString(true, vScreenTop.x, vScreenTop.y - 35, r, g, b, a, "Flashed");
						}
					}
					
				}
			}
		}		
	}

	
	if (gGui.cursoractive)
	{
		g_pSurface->SurfaceGetCursorPos(gGui.cursorpos[0], gGui.cursorpos[1]);
		g_pSurface->SurfaceSetCursorPos(gGui.cursorpos[0], gGui.cursorpos[1]);
	}
	
	if (g_Console.getValue("misc_crosshair") == 1)
	{
		g_Draw.FillRGBA((iScreenWidth / 2) - 2, (iScreenHeight / 2) - 2, 5, 5,  255, 0, 0, 255);
	}
	if (g_Console.getValue("misc_crosshair") == 2)
	{
		int CenterX = (iScreenWidth / 2);
		int CenterY = (iScreenHeight / 2);

		g_Draw.FillRGBA(CenterX - 20, CenterY, 40, 2,  255, 0, 0, 255); // Cross X
		g_Draw.FillRGBA(CenterX, CenterY - 20, 2, 40,  255, 0, 0, 255); // Cross Y
		g_Draw.FillRGBA(CenterX, CenterY - 20, 20, 2,  255, 0, 0, 255); // Top
		g_Draw.FillRGBA(CenterX - 20, CenterY + 20, 21, 2,  255, 0, 0, 255); // Bottom
		g_Draw.FillRGBA(CenterX - 20, CenterY - 20, 2, 20,  255, 0, 0, 255); // Left
		g_Draw.FillRGBA(CenterX + 20, CenterY, 2, 20,  255, 0, 0, 255); // Right
	}
	if (g_Console.getValue("misc_crosshair") == 3)
	{
		g_Draw.FillRGBA(gCenterX - 20, gCenterY, 10, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 11, gCenterY, 10, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX, gCenterY - 20, 1, 10,  255, 0, 0, 255);
		g_Draw.FillRGBA((iScreenWidth / 2) - 1.5, (iScreenHeight / 2) - 2, 5, 5,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX, gCenterY + 11, 1, 10,  255, 0, 0, 255);

		g_Draw.FillRGBA(gCenterX - 11, gCenterY - 6, 1, 13,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 11, gCenterY - 6, 1, 13,  255, 0, 0, 255);

		g_Draw.FillRGBA(gCenterX - 10, gCenterY - 6, 4, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 7, gCenterY - 6, 4, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX - 10, gCenterY + 6, 4, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 7, gCenterY + 6, 4, 1,  255, 0, 0, 255);

		g_Draw.FillRGBA(gCenterX - 33, gCenterY, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX - 33, gCenterY - 2, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX - 33, gCenterY - 4, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX - 33, gCenterY + 2, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX - 33, gCenterY + 4, 8, 1,  255, 0, 0, 255);

		g_Draw.FillRGBA(gCenterX + 26, gCenterY, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 26, gCenterY - 2, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 26, gCenterY - 4, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 26, gCenterY + 2, 8, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA(gCenterX + 26, gCenterY + 4, 8, 1,  255, 0, 0, 255);
	}
	if (g_Console.getValue("misc_crosshair") == 4)
	{
		g_Draw.FillRGBA((iScreenWidth / 2) - 15, (iScreenHeight / 2), 30, 1,  255, 0, 0, 255);
		g_Draw.FillRGBA((iScreenWidth / 2), (iScreenHeight / 2) - 15, 1, 30,  255, 0, 0, 255);
	}
	if (g_Console.getValue("misc_crosshair") == 5)
	{
		int CenterX = (iScreenWidth / 2);
		int CenterY = (iScreenHeight / 2);

		g_Draw.LineRGBA(CenterX + 15, CenterY + 15, CenterX + 3, CenterY + 3, 255, 0, 0, 255);
		g_Draw.LineRGBA(CenterX - 15, CenterY + 15, CenterX - 3, CenterY + 3, 255, 0, 0, 255);
		g_Draw.LineRGBA(CenterX + 15, CenterY - 15, CenterX + 3, CenterY - 3, 255, 0, 0, 255);
		g_Draw.LineRGBA(CenterX - 15, CenterY - 15, CenterX - 3, CenterY - 3, 255, 0, 0, 255);
		g_Draw.FillRGBA(CenterX, CenterY, 1, 1, 255,  255, 0, 0);
	}

	/*int lol = 0;
	lol = g_Timer.Start(lol);
	g_Draw.DrawStringA(g_Draw.m_Font, true, 500, 500, 255, 0, 0, 255, "Timer: %i", lol);*/

	Vector vTrace2dPos;
	if (	WorldToScreen(Base::Triggerbot::traceEndPos, vTrace2dPos) && g_Console.getValue("misc_crosshair") == 6)
	{
		g_Draw.FillRGBA(vTrace2dPos.x, vTrace2dPos.y, 4, 4, 255, 50, 50, 255);
	}
	
	unsigned short HighOrderBit = 1 << 15; // 1 = KeyDown; 0 = KeyUp;
	BOOL bIsBackspaceKeyDown = FALSE;
	GetKeyState ( VK_BACK ) & HighOrderBit ? bIsBackspaceKeyDown = TRUE : bIsBackspaceKeyDown = FALSE;
	//g_Console.RenderConsole();
	gGui.DrawMenu(0, 0, 0, 0, "sick hake");
	gGui.DrawMouse();
	if ( g_Console.IsActive( ) )
	{
		static int iFadeColor = 0;
		iFadeColor += 2;
		if ( iFadeColor >= 220 )
			iFadeColor = 0;

		g_Draw.FillRGBA ( 606 + g_Draw.getWidht ( g_Console.command.c_str ( ) ), 575, 4, 20, 255, 255, 255, iFadeColor );

		static DWORD oldTick = GetTickCount ( );
		if ( ( ( DWORD ) oldTick + 100 ) <= GetTickCount() && bIsBackspaceKeyDown )
		{
			oldTick = (DWORD)GetTickCount ( );
			g_Console.HandleKeys ( 66 );
		}
	}
	
}
//=================================================================================================
void RunCommand(CBaseEntity *pEntity, ValveSDK::CInput::CUserCmd *pUserCmd)
{
	if (!g_pMoveHelper)
		return;

	float flOldCurtime = g_pGlobalVars->curtime;
	float flOldFrametime = g_pGlobalVars->frametime;

	g_pGlobalVars->curtime = pEntity->GetTickBase() * g_pGlobalVars->interval_per_tick;
	g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;

	DWORD dwOldUserCmd = *(DWORD *)((DWORD)pEntity + 0x16E8);
	*(DWORD *)((DWORD)pEntity + 0x16E8) = (DWORD)pUserCmd;

	BYTE bMoveData[0x200];
	g_pMoveHelper->SetHost(pEntity);
	g_pPred->SetupMove(pEntity, pUserCmd, NULL, &bMoveData);
	g_pGameMovement->ProcessMovement(pEntity, &bMoveData);
	g_pPred->FinishMove(pEntity, pUserCmd, &bMoveData);
	*(DWORD *)((DWORD)pEntity + 0x16E8) = dwOldUserCmd;

	g_pMoveHelper->SetHost(NULL);

	g_pGlobalVars->curtime = flOldCurtime;
	g_pGlobalVars->frametime = flOldFrametime;
}
bool bStuck;
//===========================================================================================
bool bSendThisPacket;
DWORD dwOriginCreateMove;

void CreateMoveFrame(int sequence_number, bool &bSendThisPacket)
{ 
	bSendThisPacket = true;

	CBaseEntity* pMe = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	ValveSDK::CInput::CUserCmd *pUserCmd = &(*(ValveSDK::CInput::CUserCmd**)((DWORD)g_pInput + USERCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
	if(!pUserCmd)
		return;
	ValveSDK::CInput::CVerifiedUserCmd *pVerifiedCmd = &(*(ValveSDK::CInput::CVerifiedUserCmd**)((DWORD)g_pInput + VERIFIEDCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
	if(!pVerifiedCmd)
		return;	
	
	Vector vOldView = pUserCmd->viewangles;
	float flOldSidemove = pUserCmd->sidemove;
	float flOldForwardmove = pUserCmd->forwardmove;


	//SMAC proof bhop by how2 
	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if(g_Console.getValue("misc_bhop") == 1)
	{
		if(!bLastJumped && bShouldFake) {
			bShouldFake = false;
			pUserCmd->buttons |= IN_JUMP;
		}
		else if(pUserCmd->buttons & IN_JUMP) {
			if(pMe->GetFlags() & FL_ONGROUND) {
				bLastJumped = true;
				bShouldFake = true;
			}
			else {
				pUserCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else {
			bLastJumped = false;
			bShouldFake = false;
		}  

	}

	CBaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pMe);
	///////////////////////////////////////////////////////////////////////
	if (m_pWeapon)
	{
		/////////////////////////////////////////////////////////////////////
		// fix annyoing processmovement soundbug 
		RunCommand(pMe, pUserCmd);
		/////////////////////////////////////////////////////////////////////

		if (g_Console.getValue("aim_active") && !IsCurWepGrenade(pMe) && !IsCurWepKnife(pMe) && !IsCurWepBomb(pMe))
		{
			unsigned short HighOrderBit = 1 << 15; // 1 = KeyDown; 0 = KeyUp;
			BOOL bIsAimPressed = FALSE;
			GetKeyState(g_Console.getValue("aim_key")) & HighOrderBit ? bIsAimPressed = TRUE : bIsAimPressed = FALSE;

			if (g_Console.getValue("aim_key") > 0)
			{
				if (bIsAimPressed&& !g_Console.IsActive())
					g_Aimbot.doAim(pUserCmd);
			}
			if (g_Console.getValue("aim_key") == 0 || bIsAimPressed)
				g_Aimbot.doAim(pUserCmd);
		}


		/////////////////////////////////////////////////////////////////////
		// PARAMETER für cvars norecoil, nospread adden und dann entsprechend
		// aus der traceangle removen =)

	
				Base::Triggerbot::Do(pUserCmd, g_Console.getValue("misc_norecoil"), g_Console.getValue("misc_nospread"));

		/////////////////////////////////////////////////////////////////////


		if (pUserCmd->buttons & IN_ATTACK)
		{
			Vector vPunchAngle = Vector(0, 0, 0);
			vPunchAngle = pMe->vecGetPunchAngle();
			vPunchAngle += vPunchAngle;

			Vector vRecoil = pMe->vecGetPunchAngle();
			vRecoil *= 1.98f;
			vRecoil = g_Aimbot.DecayPunchangle(vRecoil);

			float userViewAngle[3] = { pUserCmd->viewangles.x, pUserCmd->viewangles.y, pUserCmd->viewangles.z };
			float spreadFix[3] = { 0.0f, 0.0f, 0.0f };
			g_NoSpread.applySpread(pUserCmd->random_seed, userViewAngle, spreadFix);

			if (g_Console.getValue("misc_norecoil") == 1)
			{
				Vector vecPunchAngle = pMe->vecGetPunchAngle();
				pUserCmd->viewangles -= vecPunchAngle * 2.f;
				g_Aimbot.NormalizeVector(pUserCmd->viewangles);
			
			}


			if (g_Console.getValue("misc_nospread") == 1)
			{
				//Vector vFix = Vector(spreadFix[0], spreadFix[1], spreadFix[2]);

				//g_Aimbot.NormalizeVector(vFix);
				//g_Aimbot.ClampAngle(vFix);
				g_pSpread.GetSpreadFix(pMe, pUserCmd->random_seed, pUserCmd->viewangles);
				/*pUserCmd->viewangles += vFix;*/

			}

		}

		if (pUserCmd->buttons & IN_ATTACK && IsPistol(pMe) && !IsCurWepGrenade(pMe) && !IsCurWepKnife(pMe) && !IsCurWepBomb(pMe) && g_Console.getValue("misc_autopistol") == 1)
		{
			static bool bFlip = false;
			if (bFlip)
				pUserCmd->buttons &= ~IN_ATTACK;
			bFlip = !bFlip;
		}
	}

	if (pMe->GetLifeState() == LIFE_ALIVE && !IsCurWepGrenade(pMe) && !IsCurWepKnife(pMe) && !IsCurWepBomb(pMe) && g_Console.getValue("aim_psilent") == 1)
	{
		float flServerTime = (float)(*(int*)((DWORD)pMe + NetVars::Player::m_nTickBase) * g_pGlobalVars->interval_per_tick);
		float flNextAttack = *(float*)((DWORD)m_pWeapon + NetVars::Weapon::m_flNextPrimaryAttack);

		bool bBulletTime = true;
		if (flNextAttack > flServerTime)
			bBulletTime = false;

		if( pUserCmd->buttons & IN_ATTACK && bBulletTime ) {
			bSendThisPacket = false;
		} else {
			bSendThisPacket = true;
			pUserCmd->viewangles = vOldView;
			pUserCmd->sidemove = flOldSidemove;
			pUserCmd->forwardmove = flOldForwardmove;
		}
	}
	if (g_Console.getValue("misc_strafe") == 1)
	{
		if ((GetAsyncKeyState(VK_SPACE) && !(pMe->GetFlags() & FL_ONGROUND)))
		{
			if (pUserCmd->mousedx < 0)
			{
				pUserCmd->sidemove = -400.f;
			}

			if (pUserCmd->mousedx > 0)
			{
				pUserCmd->sidemove = 400.f;
			}

		}
	}

	pVerifiedCmd->m_cmd = *pUserCmd;
	pVerifiedCmd->m_crc = pUserCmd->GetChecksum();	
} 
void __declspec(naked) __stdcall nakedCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		MOV bSendThisPacket, BL
			PUSH EBP
			MOV EBP, ESP
			SUB ESP, 8
			PUSHAD
			PUSH active
			PUSH input_sample_frametime
			PUSH sequence_number
			CALL dwOriginCreateMove
	}

	CreateMoveFrame(sequence_number, bSendThisPacket);

	__asm
	{
		POPAD
			MOV BL, bSendThisPacket
			MOV ESP, EBP
			POP EBP
			RETN 0x0C
	}
}
void __stdcall hkdRunCommand(CBaseEntity* pEntity, ValveSDK::CInput::CUserCmd* pCmd, void* moveHelper)
{
	static DWORD dwOriginRunCommand = g_pRunCommandVMT->dwGetMethodAddress(19);
	__asm
	{
		PUSH moveHelper;
		PUSH pCmd;
		PUSH pEntity;
		call dwOriginRunCommand
	}

	g_pMoveHelper = (ValveSDK::ImoveHelper*)moveHelper;
}
//=================================================================================================
int __stdcall hkdInKeyEvent(int eventcode,ButtonCode_t keynum,const char *pszCurrentBinding)
{
	static DWORD dwOriginInKeyEvent = g_pInKeyEventVMT->dwGetMethodAddress(20);
	/*
	if(eventcode)
	{		
		if(keynum == KEY_INSERT || keynum == KEY_DELETE)
		{
			g_Console.HandleCommands("ActivateConsole");
			return 0;
		}

		if(g_Console.IsActive() == true)
		{
			if ( keynum != 66 )
				g_Console.HandleKeys(keynum);
			return 0;
		}
	}
*/
	__asm
	{
		push pszCurrentBinding;
		push keynum;
		push eventcode;
		call dwOriginInKeyEvent;
	}	
}
//=================================================================================================
bool __stdcall hkdFireEventClientSide(ValveSDK::IGameEvent *gameEvent )
{

	static DWORD dwOriginFireEventClientSide = g_pFireEventVMT->dwGetMethodAddress(8);	
	
	__asm
	{		
		push gameEvent;
		call dwOriginFireEventClientSide		
	}			
}
//=================================================================================================
void* GetInterfacePtr ( const char* interfaceName, const char* ptrName, CreateInterface_t pInterface )
{
	char szDebugString[1024];
	std::string sinterface = "";
	std::string interfaceVersion = "0";

	for (int i = 0; i <= 99; i++)
	{
		sinterface = interfaceName;
		sinterface += interfaceVersion;
		sinterface += std::to_string(i);

		void* funcPtr = pInterface(sinterface.c_str(), NULL);

		if ((DWORD)funcPtr != 0x0)
		{
			sprintf(szDebugString, "%s: 0x%x (%s%s%i)", ptrName, (DWORD)funcPtr, interfaceName, interfaceVersion.c_str(), i);

			Base::Debug::LOG(szDebugString);

			return funcPtr;
		}
		if (i >= 99 && interfaceVersion == "0")
		{
			interfaceVersion = "00";
			i = 0;
		}
		else if (i >= 99 && interfaceVersion == "00")
		{
			sprintf(szDebugString, "%s: 0x%x (error)", ptrName, (DWORD)funcPtr);
			Base::Debug::LOG(szDebugString);
		}
	}
	return 0;
}
//=================================================================================================
void InitDllThread ( void* ptr )
{
	Base::Debug::LOG ( "Init Thread" );
	char szDebugString[1024];
	char PanelReaction[1024];

	CreateInterface_t EngineFactory = NULL;
	CreateInterface_t ClientFactory = NULL;
	CreateInterface_t VGUIFactory = NULL;
	CreateInterface_t VGUI2Factory = NULL;

	ClientFactory = ( CreateInterfaceFn ) GetProcAddress( Base::Utils::GetModuleHandleSafe ( "client.dll" ), "CreateInterface" );
	EngineFactory = ( CreateInterfaceFn ) GetProcAddress( Base::Utils::GetModuleHandleSafe ( "engine.dll" ), "CreateInterface" );
	VGUIFactory = ( CreateInterfaceFn ) GetProcAddress( Base::Utils::GetModuleHandleSafe ( "vguimatsurface.dll" ), "CreateInterface" );
	VGUI2Factory = (CreateInterfaceFn) GetProcAddress( Base::Utils::GetModuleHandleSafe ("vgui2.dll"), "CreateInterface");

	g_pClient = ( ValveSDK::HLCLient* ) GetInterfacePtr( "VClient", "g_pClient", ClientFactory );
	g_pEntList = (ValveSDK::CEntityList*)GetInterfacePtr("VClientEntityList", "g_pEntList", ClientFactory );
	g_pPred = (ValveSDK::CPrediction*)GetInterfacePtr("VClientPrediction", "g_pPred", ClientFactory);
	g_pGameMovement = (ValveSDK::CGameMovement*)GetInterfacePtr("GameMovement", "g_pGameMovement", ClientFactory);

	
	typedef ValveSDK::CGlowObjectManager* (__thiscall *GetGlowObjectPtr_t)(void);
	static GetGlowObjectPtr_t GetGlowObjectPtr = (GetGlowObjectPtr_t)(Base::Utils::PatternSearch("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E", "x????xxxx", NULL, NULL));
	g_pGlowObjectManager = GetGlowObjectPtr();
	//g_pGlowObjectManager = *(ValveSDK::CGlowObjectManager**)(g_pClient + 0x2B31E0);
	sprintf(szDebugString, "g_pGlowObjectManager: 0x%x", (DWORD)g_pGlowObjectManager);
	Base::Debug::LOG(szDebugString);
	
	g_pEngine = (ValveSDK::CEngineClient*)GetInterfacePtr("VEngineClient", "g_pEngine", EngineFactory);
	//g_pModel = (ValveSDK::CModelInfo*)GetInterfacePtr("VEngineModel", "g_pModel", EngineFactory);
	g_pModel = (ValveSDK::CModelInfo*)GetInterfacePtr("VModelInfoClient", "g_pModel", EngineFactory);
	g_pDebugOverlay = (ValveSDK::CDebugOverlay*)GetInterfacePtr("VDebugOverlay", "g_pDebugOverlay", EngineFactory);	
	g_pEngineTrace = (ValveSDK::CTrace*)GetInterfacePtr("EngineTraceClient","g_pEngineTrace",EngineFactory);
	g_pGameEventManager =(ValveSDK::IGameEventManager*)EngineFactory("GAMEEVENTSMANAGER002",NULL);
	//g_pGameEventManager = (ValveSDK::IGameEventManager*)GetInterfacePtr("GAMEEVENTSMANAGER002", "g_pGameEventManager", EngineFactory);
	g_pModelRender = (ValveSDK::IVModelRender*)GetInterfacePtr("VEngineModel", "g_pModelRender", EngineFactory);
	g_pRenderView = (ValveSDK::IVRenderView*)GetInterfacePtr("VEngineRenderView", "g_pRenderView", EngineFactory);

	sprintf( szDebugString, "g_pGameEventManager: 0x%x",(DWORD)g_pGameEventManager );
	Base::Debug::LOG(szDebugString);

	g_pSurface = (ValveSDK::ISurface*)GetInterfacePtr("VGUI_Surface", "g_pSurface", VGUIFactory);
	g_pPanel = (ValveSDK::IPanel*)GetInterfacePtr("VGUI_Panel", "g_pPanel", VGUI2Factory);
	
	g_pGlobalVars = (ValveSDK::CGlobalVars*)*(PDWORD)*(PDWORD)(Base::Utils::PatternSearch("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04", "x????xxxxxx", NULL, NULL) + 0x1);
	sprintf(szDebugString, "g_pGlobalVars: 0x%x", (DWORD)g_pGlobalVars);
	Base::Debug::LOG(szDebugString);
	
	DWORD* pdwClientVMT2 = (DWORD*)*(DWORD*)g_pClient;
	g_pInput = *(ValveSDK::CInput**)(pdwClientVMT2[21] + 0x5F);
	sprintf(szDebugString, "g_pInput: 0x%08X ", (DWORD)g_pInput);
	Base::Debug::LOG(szDebugString);

	PDWORD* pdwPanelVMT = (PDWORD*)g_pPanel;
	g_pPaintTraverseVMT = new Base::Utils::CVMTHookManager(pdwPanelVMT);
	g_pPaintTraverseVMT->dwHookMethod((DWORD)hkdPaintTraverse, 41);
	Base::Debug::LOG("PaintTraverse: Hooked!");

	PDWORD* pdwClientVMT = (PDWORD*)g_pClient;
	PDWORD* pdwPredVMT = (PDWORD*)g_pPred;

	g_pInKeyEventVMT = new Base::Utils::CVMTHookManager(pdwClientVMT);
	g_pInKeyEventVMT->dwHookMethod((DWORD)hkdInKeyEvent, 20);
	Base::Debug::LOG("InKeyEvent: Hooked!");

	g_pCreateMoveVMT = new Base::Utils::CVMTHookManager(pdwClientVMT);
	dwOriginCreateMove = g_pCreateMoveVMT->dwGetMethodAddress(21);
	g_pCreateMoveVMT->dwHookMethod((DWORD)nakedCreateMove, 21);
	Base::Debug::LOG("CreateMove: Hooked!");

	g_pRunCommandVMT = new Base::Utils::CVMTHookManager(pdwPredVMT);
	g_pRunCommandVMT->dwHookMethod((DWORD)hkdRunCommand, 19);
	Base::Debug::LOG("RunCommand: Hooked!");

	PDWORD* pdwModelRenderVMT = (PDWORD*)g_pModelRender;
	g_pModelRenderVMT = new Base::Utils::CVMTHookManager(pdwModelRenderVMT);
	g_pModelRenderVMT->dwHookMethod((DWORD)DrawModel_Hooked, 21);
	Base::Debug::LOG("DrawModel: Hooked!");

	/*PDWORD* pdwGameEventVMT = (PDWORD*)g_pGameEventManager;

	g_pFireEventVMT = new Base::Utils::CVMTHookManager(pdwGameEventVMT);
	g_pFireEventVMT->dwHookMethod((DWORD)hkdFireEventClientSide,8);
	Base::Debug::LOG("hooked FireEventClientSide...");*/
	NetVars::Init();
	//gEvent.RegisterSelf();
	//gEvent.Register();
}
//=================================================================================================
void unhook(void)
{
	g_pPaintTraverseVMT->UnHook();
	g_pCreateMoveVMT->UnHook();
	g_pInKeyEventVMT->UnHook();
	g_pRunCommandVMT->UnHook();
	g_pModelRenderVMT->UnHook();
	//g_pFireEventVMT->UnHook();
}
//=================================================================================================
void unhookThread(LPARAM lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	while(true)
	{
		if( GetAsyncKeyState( VK_MULTIPLY ) & 1 && GetForegroundWindow() == FindWindow( NULL, "Counter-Strike: Global Offensive" ) ) 
		{ 

			Sleep( 1000 ); 
			unhook( ); 		
			Sleep( 2000 ); 
			MessageBeep( 0xFFFFFFFF ); 
			Base::Debug::LOG("unhooked");

			//fclose(g_pConStream);
			FreeConsole();
			SendMessage ( FindWindow(0,"DebugMessages"), WM_CLOSE, NULL, NULL );

			FreeLibraryAndExitThread( hModule, 0 );
			
		}

	}
}
IMAGE_DOS_HEADER __ImageBase;
//===============================================================================================
std::wstring folder;
BOOL WINAPI DllMain ( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved )
{
	BOOL bReturnValue = TRUE;
	DWORD ModuleBase = (DWORD)hinstDLL;
	switch ( dwReason )
	{	
	case DLL_PROCESS_ATTACH:

		HideModule(hinstDLL);
		HideModuleFromPEB(hinstDLL);
		HideModuleXta(hinstDLL);
		RemovePeHeader(ModuleBase);
		DestroyModuleHeader(hinstDLL);

		Base::Debug::AttachDebugConsole();
		_beginthread ( InitDllThread, 0, NULL );
		CreateThread ( NULL, NULL, ( LPTHREAD_START_ROUTINE ) unhookThread, hinstDLL, NULL, NULL );
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return bReturnValue;
}