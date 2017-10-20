#define CLIENT_DLL
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include <iostream>
#include <sstream>
#include <Psapi.h>
#include <shlobj.h>

#include "offsets.h"

// Public
#include "public\\tier0\\wchartypes.h"
#include "public\\cdll_int.h"
#include "public\\iprediction.h"
#include "public\\bone_setup.h"
#include "public\\icliententitylist.h"
#include "public\\ienginevgui.h"
#include "public\\IGameUIFuncs.h"
#include "public\\igameevents.h"
#include "public\\view_shared.h"
#include "public\\inetchannelinfo.h"
#include "public\\iachievementmgr.h"
#include "public\\steam\\steam_api.h"
#include "public\\steam\\isteamuserstats.h"
#include "public\\model_types.h"
#include "public\\inputsystem\\iinputstacksystem.h"
#include "public/dt_recv.h"
// Client

#include "game\\client\\cbase.h"
#include "game\\client\\imessagechars.h"
#include "game\\client\\iclientmode.h"
#include "game\\client\\cliententitylist.h"
#include "game\\client\\cdll_client_int.h"
#include "game\\client\\c_baseanimating.h"
#include "game\\client\\c_basecombatweapon.h"
#include "game\\client\\c_baseplayer.h"
#include "game\\client\\enginesprite.h"
#include "game\\client\\input.h"
#include "game\\client\\c_playerresource.h"
#include "game\\client\\iviewrender.h"
#include "game\\client\\viewrender.h"
#include "game\\client\\game_controls\\commandmenu.h"
#include "game\\client\\hudelement.h"
#include "game\\shared\\gamemovement.h"
#include "game\\client\\clientmode_shared.h"
#include "game\\client\\glow_outline_effect.h"

// Engine
#include "engine\\client.h"
#include "public\\engine\\ivmodelrender.h"
#include "public\\engine\\ivdebugoverlay.h"
#include "public\\engine\\ivmodelinfo.h"
#include "public\\engine\\IEngineTrace.h"
#include "public\\engine\\IEngineSound.h"

// Material System
#include "public\\materialsystem\\imaterialsystemstub.h"
#include "public\\materialsystem\\itexture.h"
#include "public\\materialsystem\\imaterialvar.h"
#include "public\\tier1\\checksum_md5.h"

// VGUI
#include "public\\matsys_controls\\matsyscontrols.h"
#include "public\\vgui\\IClientPanel.h"
#include "public\\vgui\\IPanel.h"
#include "public\\vgui\\ISurface.h"
#include "public\\vgui\\ILocalize.h"
#include "public\\collisionutils.h"
#include "public\\vstdlib\\random.h"

// Server
#include "game\\server\\playerinfomanager.h"

// VGUI Controls
#include "public\\vgui_controls\\Panel.h"

// VGUI Material Surface
#include "public\\VGuiMatSurface\\IMatSystemSurface.h"
#include "public\\mathlib\\vector.h"

// Shared
#include "game\\shared\\usermessages.h"
#include "game\\shared\\basecombatweapon_shared.h"
#include "game\\shared\\takedamageinfo.h"
#include "game\\shared\\igamemovement.h"
#include "game\\shared\\in_buttons.h"

// Utils
#include "public\\tier1\\utlvector.h"

#include "public\\bspfile.h"
#include "public\\gamebspfile.h"

#pragma comment(lib, "tier1.lib")
#pragma comment(lib, "tier2.lib")
#pragma comment(lib, "tier3.lib")
#pragma comment(lib, "tier0.lib")
#pragma comment(lib, "mathlib.lib")
#pragma comment(lib, "vstdlib.lib")

enum eWeaponType
{
	WEAPON_DEAGLE = 1,
	WEAPON_DUALBERETTA,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_ZEUSX27,
	WEAPON_P2000,
	//WEAPON_USP,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INC,
	WEAPON_C4,
	WEAPON_TKNIFE = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75 = 63,
	WEAPON_BAYONET = 500,
	WEAPON_FLIP = 505,
	WEAPON_GUT,
	WEAPON_KARAMBIT,
	WEAPON_M9BAYONET,
	WEAPON_HUNTSMAN,
	WEAPON_FALCHION = 512,
	WEAPON_BUTTERFLY = 515
};

/* Client Class IDs */
enum eClientClassID
{
	CAK47 = 1,
	CBASECSGRENADEPROJECTILE = 9,
	CBASEENTITY = 11,
	CBASETRIGGER = 20,
	CCASCADELIGHT = 29,
	CCOLORCORRECTION = 31,
	CCSGAMERULESPROXY = 33,
	CCSPLAYER = 34,
	CCSPLAYERRESOURCE = 35,
	CCSRAGDOLL = 36,
	CCSTEAM = 37,
	CDEAGLE = 38,
	CDECOYGRENADE = 39,
	CDYNAMICPROP = 42,
	CENVTONEMAPCONTROLLER = 57,
	CFOGCONTROLLER = 64,
	CFUNC_DUST = 66,
	CFUNCBRUSH = 69,
	CFUNCOCCLUDER = 74,
	CFUNCROTATING = 76,
	CHEGRENADE = 81,
	CHOSTAGE = 82,
	CHOSTAGECARRIABLEPROP = 83,
	CKNIFE = 88,
	CLIGHTGLOW = 90,
	CPHYSICSPROP = 100,
	CPHYSICSPROPMULTIPLAYER = 101,
	CPOSTPROCESSCONTROLLER = 109,
	CPREDICTEDVIEWMODEL = 112,
	CPROPDOORROTATING = 114,
	CROPEKEYFRAME = 120,
	CSHADOWCONTROL = 123,
	CSMOKESTACK = 127,
	CSPRITE = 130,
	CSUN = 135,
	CVOTECONTROLLER = 192,
	CWEAPONAWP = 196,
	CWEAPONFAMAS = 202,
	CWEAPONGALILAR = 206,
	CWEAPONGLOCK = 207,
	CWEAPONHKP2000 = 208,
	CWEAPONM4A1 = 211,
	CWEAPONMAC10 = 212,
	CWEAPONMP7 = 215,
	CWEAPONMP9 = 216,
	CWEAPONNOVA = 218,
	CWEAPONP250 = 220,
	CWEAPONP90 = 221,
	CWEAPONSG556 = 227,
	CWEAPONSSG08 = 228,
	CWEAPONUMP45 = 232,
	CWEAPONXM1014 = 234,
	CWORLD = 235,
};

CBaseEntity* pLocalPlayer;
char* bSendPacket;
HWND hwCSGO;
int iScreenWidth, iScreenHeight = 0;
bool bMenuOpen = false;
DWORD dwPunch;
IMaterial* matCham;

#include "vars.h"
#include "config.h"
#include "vmt.h"
#include "interfaces.h"
#include "netvars.h"
#include "draw.h"
#include "aim.h"
#include "menu.h"
#include "hvh.h"
#include "textureholder.h"
#include "hooks.h"
#include "antileak.h"