#pragma once

class cVars
{
public:
	// Aim
	int aim_active;
	int aim_key;
	int aim_fov;
	int aim_targetby;

	int aim_hitbox;
	int aim_smooth_x;
	int aim_smooth_y;
	int menu_page2;
	int aim_silent;
	int aim_psilent;
	// ESP
	int esp_enemyonly;
	int esp_box;
	int esp_name;

	int esp_health;
	int esp_glow;
	int esp_bomb;
	int esp_bone;
	int esp_hitbox;
	int esp_weapon;
	int esp_entity;
	int esp_helmet;
	int esp_flashed;
	// Visual
	int vis_chams;
	int vis_radar;
	// Misc
	int misc_rcs;
	int misc_scs;
	int misc_nospread;
	int misc_strafe;
	int misc_norecoil;
	int misc_novisrecoil;
	int misc_xhair;
	int misc_hitmarker;
	int misc_apistol;
	int misc_crosshair;
	int misc_bhop;
	int misc_noflash;
	int misc_triggerbot;
	int misc_triggerspot;
	int misc_recoilreduce;
	int aim_recoil_x;
	int aim_recoil_y;
	int misc_triggerkey = 3000;
	int misc_info;
	int misc_stats;
	int kills, deaths, headshots;
	// ESP colors	
	/* Counter-Terrorists */
	int ct_r = 30;
	int ct_g = 144;
	int ct_b = 255;
	int ct_a = 255;
	int vis_ct_r = 0;
	int vis_ct_g = 255;
	int vis_ct_b = 0;
	int vis_ct_a = 255;
	/* Terrorists */
	int t_r = 255;
	int t_g = 0;
	int t_b = 40;
	int t_a = 255;
	int vis_t_r = 255;
	int vis_t_g = 255;
	int vis_t_b = 0;
	int vis_t_a = 255;
	/* Independent */
	int esp_alpha = 255;
	// Chams colors
	/* Counter-Terrorists */
	int chams_ct_r = 30;
	int chams_ct_g = 144;
	int chams_ct_b = 255;
	int chams_vis_ct_r = 255;
	int chams_vis_ct_g = 255;
	int chams_vis_ct_b = 0;
	/* Terrorists */
	int chams_t_r = 255;
	int chams_t_g = 0;
	int chams_t_b = 40;
	int chams_vis_t_r = 255;
	int chams_vis_t_g = 255;
	int chams_vis_t_b = 0;
	int chams_alpha = 255;
	// Glow colors
	/* Counter-Terrorists */
	int glow_ct_r = 30;
	int glow_ct_g = 144;
	int glow_ct_b = 255;
	int glow_vis_ct_r = 0;
	int glow_vis_ct_g = 255;
	int glow_vis_ct_b = 0;
	/* Terrorists */
	int glow_t_r = 255;
	int glow_t_g = 0;
	int glow_t_b = 40;
	int glow_vis_t_r = 255;
	int glow_vis_t_g = 255;
	int glow_vis_t_b = 0;
	/*Alpha*/
	int glow_alpha = 140;

	// Crosshair colors
	int crosshair_r = 255;
	int crosshair_g = 0;
	int crosshair_b = 0;

	// Entity colors
	int ent_r = 255;
	int ent_g = 255;
	int ent_b = 0;

	// Bomb color
	int bomb_r = 255;
	int bomb_g = 0;
	int bomb_b = 0;

	//Menu colors
	/* Main Box */
	int mainframe_r;
	int mainframe_g;
	int mainframe_b;
	int mainframe_a;
	/* Main Box Frame */
	int mainbox_r;
	int mainbox_g;
	int mainbox_b;
	int mainbox_a;
	/* Input Box Frame */
	int input_r;
	int input_g;
	int input_b;
	int input_a;
	/* Header Box */
	int header_r;
	int header_g;
	int header_b;
	int header_a;
	/* Header Box Frame*/
	int headerframe_r;
	int headerframe_g;
	int headerframe_b;
	int headerframe_a;

	/* Font Colors */
	int stringcons_r;
	int stringcons_g;
	int stringcons_b;
	int cmdcolor_r;
	int cmdcolor_g;
	int cmdcolor_b;
	int printfont_r;
	int printfont_g;
	int printfont_b;

	int misc_menu_x;
	int misc_menu_y;
	int misc_menu_w;

	char* watermarkname = "SEARCHYWEBZ CS:GO";
};
extern cVars gVars;