#include "main.h"

bool INSERT_KEY_PRESSED;

UICheckBox aim_active;
UICheckBox aim_autowall;
UICheckBox aim_autoshoot;
UICheckBox aim_smooth;
UICheckBox aim_silent;
UICheckBox aim_psilent;

UISlider aim_smoothBar;
UISlider aim_fov;
UISlider aim_bone;

UICheckBox vis_active;
UICheckBox vis_esp_box;
UICheckBox vis_esp_name;
UICheckBox vis_esp_healthbar;
UICheckBox vis_chams;
UICheckBox vis_activeweapon;
UICheckBox vis_xqz;
UICheckBox vis_skeleton;

UICheckBox misc_active;
UICheckBox misc_bunnyhop;
UICheckBox misc_norecoil;
UICheckBox misc_novisualrecoil;
UICheckBox misc_nospread;
UICheckBox misc_fakelag;
UICheckBox misc_autostrafe;
UICheckBox misc_force_nospread;
UICheckBox misc_teleport;

UISlider misc_fakelag_factor;
UISlider misc_viewfov;
UISlider hvh_x;
UISlider hvh_y;
UISlider hvh_choke;

UICheckBox edge_antiaim;


UISlider color_r;
UISlider color_g;
UISlider color_b;

UISlider glow_r;
UISlider glow_g;
UISlider glow_b;
UISlider glow_a;

void DrawpCursor(int x, int y) {
	FillRGB(x + 1, y, 1, 17, Color(3, 6, 26, 255));
	for (int i = 0; i < 11; i++)
	FillRGB(x + 2 + i, y + 1 + i, 1, 1, Color(3, 6, 26, 255));
	FillRGB(x + 8, y + 12, 5, 1, Color(3, 6, 26, 255));
	FillRGB(x + 8, y + 13, 1, 1, Color(3, 6, 26, 255));
	FillRGB(x + 9, y + 14, 1, 2, Color(3, 6, 26, 255));
	FillRGB(x + 10, y + 16, 1, 2, Color(3, 6, 26, 255));
	FillRGB(x + 8, y + 18, 2, 1, Color(3, 6, 26, 255));
	FillRGB(x + 7, y + 16, 1, 2, Color(3, 6, 26, 255));
	FillRGB(x + 6, y + 14, 1, 2, Color(3, 6, 26, 255));
	FillRGB(x + 5, y + 13, 1, 1, Color(3, 6, 26, 255));
	FillRGB(x + 4, y + 14, 1, 1, Color(3, 6, 26, 255));
	FillRGB(x + 3, y + 15, 1, 1, Color(3, 6, 26, 255));
	FillRGB(x + 2, y + 16, 1, 1, Color(3, 6, 26, 255));
	for (int i = 0; i < 4; i++)
		FillRGB(x + 2 + i, y + 2 + i, 1, 14 - (i * 2), Color(255 - (i * 4), 255 - (i * 4), 255 - (i * 4), 255));
	FillRGB(x + 6, y + 6, 1, 8, Color(235, 235, 235, 255));
	FillRGB(x + 7, y + 7, 1, 9, Color(231, 231, 231, 255));
	for (int i = 0; i < 4; i++)
		FillRGB(x + 8 + i, y + 8 + i, 1, 4 - i, Color(227 - (i * 4), 227 - (i * 4), 227 - (i * 4), 255));
	FillRGB(x + 8, y + 14, 1, 4, Color(207, 207, 207, 255));
	FillRGB(x + 9, y + 16, 1, 2, Color(203, 203, 203, 255));

}

void initMenu() {
	mw = 900;
	mh = 400;
	mx = 90;
	my = 90;
	SelectedTab = 0;
	/* INIT BUTTONS AND SHIT*/

	

	aim_active.SetCoord(mx + 11, my + 40); aim_active.SetText("Active");
	aim_fov.SetActive(true); aim_fov.setPos(mx + 11, my + 100); aim_fov.initColor(); aim_fov.setMaxSliderValue(181); aim_fov.SetWidth(190); aim_fov.SetText("FOV");
	aim_smooth.SetCoord(mx + 11, my + 70); aim_smooth.SetText("Smooth");
	aim_smoothBar.SetActive(true); aim_smoothBar.setPos(mx + 11, my + 130); aim_smoothBar.initColor(); aim_smoothBar.setMaxSliderValue(51); aim_smoothBar.SetText("Smooth"); aim_smoothBar.SetWidth(190);
	aim_silent.SetCoord(mx + 11, my + 160); aim_silent.SetText("Silent Aim");
	aim_psilent.SetCoord(mx + 11, my + 190); aim_psilent.SetText("Perfect Silent Aim");
	aim_autoshoot.SetCoord(mx + 11, my + 220); aim_autoshoot.SetText("Auto-Shoot");
	aim_autowall.SetCoord(mx + 11, my + 250); aim_autowall.SetText("Auto-Wall");
	aim_bone.SetActive(true); aim_bone.setPos(mx + 11, my + 280); aim_bone.initColor(); aim_bone.setMaxSliderValue(81); aim_bone.SetWidth(190); aim_bone.SetText("Bone");

	vis_active.SetCoord(mx + 11, my + 40); vis_active.SetText("Active");
	vis_esp_box.SetCoord(mx + 11, my + 70); vis_esp_box.SetText("ESP Box");
	vis_esp_name.SetCoord(mx + 11, my + 100); vis_esp_name.SetText("ESP Name");
	vis_esp_healthbar.SetCoord(mx + 11, my + 130); vis_esp_healthbar.SetText("ESP Healthbar");
	glow_r.SetActive(true); glow_r.setPos(mx + 11, my + 160);  glow_r.initColor(); glow_r.setMaxSliderValue(256); glow_r.SetWidth(880); glow_r.SetText("Glow R");
	glow_g.SetActive(true); glow_g.setPos(mx + 11, my + 190);  glow_g.initColor(); glow_g.setMaxSliderValue(256); glow_g.SetWidth(880); glow_g.SetText("Glow G");
	glow_b.SetActive(true); glow_b.setPos(mx + 11, my + 220);  glow_b.initColor(); glow_b.setMaxSliderValue(256); glow_b.SetWidth(880); glow_b.SetText("Glow B");
	glow_a.SetActive(true); glow_a.setPos(mx + 11, my + 250);  glow_a.initColor(); glow_a.setMaxSliderValue(256); glow_a.SetWidth(880); glow_a.SetText("Glow A");
	vis_chams.SetCoord(mx + 11, my + 280); vis_chams.SetText("Chams");
	//vis_xqz.SetCoord(mx + 11, my + 310); vis_xqz.SetText("XQZ");
	//vis_skeleton.SetCoord(mx + 11, my + 340); vis_skeleton.SetText("Skeleton");

	misc_active.SetCoord(mx + 11, my + 40); misc_active.SetText("Active");
	misc_bunnyhop.SetCoord(mx + 11, my + 70); misc_bunnyhop.SetText("Bunnyhop");
	misc_autostrafe.SetCoord(mx + 11, my + 100); misc_autostrafe.SetText("Auto Strafe");
	misc_fakelag.SetCoord(mx + 11, my + 130); misc_fakelag.SetText("No Hands");
	misc_fakelag_factor.SetActive(true); misc_fakelag_factor.setPos(mx + 11, my + 160); misc_fakelag_factor.initColor(); misc_fakelag_factor.setMaxSliderValue(31); misc_fakelag_factor.SetWidth(190); misc_fakelag_factor.SetText("Factor");
	misc_norecoil.SetCoord(mx + 11, my + 190); misc_norecoil.SetText("No Recoil");
	misc_novisualrecoil.SetCoord(mx + 11, my + 220); misc_novisualrecoil.SetText("No Visual Recoil");
	misc_nospread.SetCoord(mx + 11, my + 250); misc_nospread.SetText("No Spread HvH");
	misc_force_nospread.SetCoord(mx + 11, my + 280); misc_force_nospread.SetText("Force No Spread");
	//misc_teleport.SetCoord(mx + 11, my + 310); misc_teleport.SetText("Teleport");
	//misc_viewfov.SetActive(true); misc_viewfov.setPos(mx + 11, my + 340); misc_viewfov.initColor(); misc_viewfov.setMaxSliderValue(181); misc_viewfov.SetWidth(190); misc_viewfov.SetText("View FOV");

	color_r.SetActive(true); color_r.setPos(mx + 11, my + 40); color_r.initColor(); color_r.setMaxSliderValue(256); color_r.SetWidth(880); color_r.SetText("Menu R");
	color_g.SetActive(true); color_g.setPos(mx + 11, my + 70); color_g.initColor(); color_g.setMaxSliderValue(256); color_g.SetWidth(880); color_g.SetText("Menu G");
	color_b.SetActive(true); color_b.setPos(mx + 11, my + 100); color_b.initColor(); color_b.setMaxSliderValue(256); color_b.SetWidth(880); color_b.SetText("Menu B");

	hvh_x.SetActive(true); hvh_x.setPos(mx + 11, my + 40); hvh_x.initColor(); hvh_x.setMaxSliderValue(6); hvh_x.SetWidth(880); hvh_x.SetText("Anti-Aim X");
	hvh_y.SetActive(true); hvh_y.setPos(mx + 11, my + 70); hvh_y.initColor(); hvh_y.setMaxSliderValue(10); hvh_y.SetWidth(880); hvh_y.SetText("Anti-Aim Y");
	//hvh_choke.SetActive(true); hvh_choke.setPos(mx + 11, my + 100); hvh_choke.initColor(); hvh_choke.setMaxSliderValue(51); hvh_choke.SetWidth(880); hvh_choke.SetText("Anti-Aim Choke");

	int r, g, b;
	r = 153;
	g = 153;
	b = 0;

	int aR, aG, aB, aA;
	aR = 0, aG = 0, aB = 0, aA = 0;
	glow_r.setValue(aR);
	glow_g.setValue(aG);
	glow_b.setValue(aB);
	glow_a.setValue(aA);

	color_r.setValue(r);
	color_g.setValue(g);
	color_b.setValue(b);

	
}



void DrawMenu() {
	if (MenuOpend) {
		FillRGB(mx, my, mw, mh, Color(26, 26, 26, 255));
		DrawBox(mx - 1, my - 31, mw + 1, mh + 31, themeColor);
		FillRGB(mx, my - 30, mw, 30, themeColor);

		if (SelectedTab == 0) {
			FillRGB(mx, my, 180, 30, evenDarker);
			FillRGB(mx + 180, my, 180, 30, themeColor);
			FillRGB(mx + 360, my, 180, 30, themeColor);
			FillRGB(mx + 540, my, 180, 30, themeColor);
			FillRGB(mx + 720, my, 180, 30, themeColor);
		}
		if (SelectedTab == 1) {
			FillRGB(mx, my, 180, 30, themeColor);
			FillRGB(mx + 180, my, 180, 30, evenDarker);
			FillRGB(mx + 360, my, 180, 30, themeColor);
			FillRGB(mx + 540, my, 180, 30, themeColor);
			FillRGB(mx + 720, my, 180, 30, themeColor);
		}
		if (SelectedTab == 2) {
			FillRGB(mx, my, 180, 30, themeColor);
			FillRGB(mx + 180, my, 180, 30, themeColor);
			FillRGB(mx + 360, my, 180, 30, evenDarker);
			FillRGB(mx + 540, my, 180, 30, themeColor);
			FillRGB(mx + 720, my, 180, 30, themeColor);
		}
		if (SelectedTab == 3) {
			FillRGB(mx, my, 180, 30, themeColor);
			FillRGB(mx + 180, my, 180, 30, themeColor);
			FillRGB(mx + 360, my, 180, 30, themeColor);
			FillRGB(mx + 540, my, 180, 30, evenDarker);
			FillRGB(mx + 720, my, 180, 30, themeColor);
		}
		if (SelectedTab == 4) {
			FillRGB(mx, my, 180, 30, themeColor);
			FillRGB(mx + 180, my, 180, 30, themeColor);
			FillRGB(mx + 360, my, 180, 30, themeColor);
			FillRGB(mx + 540, my, 180, 30, themeColor);
			FillRGB(mx + 720, my, 180, 30, evenDarker);
		}
		DrawStringMiddle(pFont, true, mx + 90, my + 15, Color(255, 255, 255, 255), "Aimbot");
		DrawStringMiddle(pFont, true, mx + 270, my + 15, Color(255, 255, 255, 255), "Visuals");
		DrawStringMiddle(pFont, true, mx + 450, my + 15, Color(255, 255, 255, 255), "Misc");
		DrawStringMiddle(pFont, true, mx + 630, my + 15, Color(255, 255, 255, 255), "HvH");
		DrawStringMiddle(pFont, true, mx + 810, my + 15, Color(255, 255, 255, 255), "Colors");
		DrawStringMiddle(pFont, true, mx + mw / 2, my - 15, Color(255, 255, 255, 255), "ISES Source Code");

		if (SelectedTab == 0) {
			aim_active.render();
			aim_fov.Render();
			aim_smooth.render();
			aim_smoothBar.Render();
			aim_silent.render();
			aim_psilent.render();
			aim_autoshoot.render();
			aim_autowall.render();
			aim_bone.Render();
		}
		if (SelectedTab == 1) {
			vis_active.render();
			vis_esp_box.render();
			vis_esp_name.render();
			vis_esp_healthbar.render();
			vis_chams.render();
			//vis_xqz.render();
			//vis_skeleton.render();
			glow_r.Render();
			glow_g.Render();
			glow_b.Render();
			glow_a.Render();
		}
		if (SelectedTab == 2) {
			misc_active.render();
			misc_bunnyhop.render();
			misc_autostrafe.render();
			misc_fakelag.render();
			misc_fakelag_factor.Render();
			misc_norecoil.render();
			//misc_teleport.render();
			misc_novisualrecoil.render();
			misc_nospread.render();
			misc_force_nospread.render();
			//misc_viewfov.Render();
		}
		if (SelectedTab == 4) {
			color_r.Render();
			color_g.Render();
			color_b.Render();
		}
		if (SelectedTab == 3) {
			hvh_x.Render();
			hvh_y.Render();
			//hvh_choke.Render();
		}
	}
}

void UpdateMenu() {
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		if (MenuOpend) {
			pEngine->ExecuteClientCmd("cl_mouseenable 1");
			MenuOpend = false;
		}
		else {
			pEngine->ExecuteClientCmd("cl_mouseenable 0");
			MenuOpend = true;
			
		}
	}
	else {
		INSERT_KEY_PRESSED = false;
	}

	int tabX, tabY;
	if (SelectedTab == 0) {
		aim_active.onMouseMove();
		aim_smooth.onMouseMove();
		aim_silent.onMouseMove();
		aim_psilent.onMouseMove();
		aim_autoshoot.onMouseMove();
		aim_autowall.onMouseMove();
	}
	if (SelectedTab == 1) {
		vis_active.onMouseMove();
		vis_esp_box.onMouseMove();
		vis_esp_name.onMouseMove();
		vis_esp_healthbar.onMouseMove();
		vis_chams.onMouseMove();
		//vis_xqz.onMouseMove();
		//vis_skeleton.onMouseMove();
	}
	if (SelectedTab == 2) {
		misc_active.onMouseMove();
		misc_bunnyhop.onMouseMove();
		misc_autostrafe.onMouseMove();
		misc_fakelag.onMouseMove();
		misc_norecoil.onMouseMove();
		misc_teleport.onMouseMove();
		misc_novisualrecoil.onMouseMove();
		misc_nospread.onMouseMove();
		//misc_force_nospread.onMouseMove();
	}
}

void onMouseClick() {
	if (CursorInArea(mx, my, 180, 30)) {
		SelectedTab = 0;
	}
	if (CursorInArea(mx + 180, my, 180, 30)) {
		SelectedTab = 1;
	}
	if (CursorInArea(mx + 360, my, 180, 30)) {
		SelectedTab = 2;
	}
	if (CursorInArea(mx + 540, my, 180, 30)) {
		SelectedTab = 3;
	}
	if (CursorInArea(mx + 720, my, 180, 30)) {
		SelectedTab = 4;
	}
	if (SelectedTab == 0) {
		aim_active.onMouseClick();
		aim_smooth.onMouseClick();
		aim_silent.onMouseClick();
		aim_psilent.onMouseClick();
		aim_autoshoot.onMouseClick();
		aim_autowall.onMouseClick();
	}
	if (SelectedTab == 1) {
		vis_active.onMouseClick();
		vis_esp_box.onMouseClick();
		vis_esp_name.onMouseClick();
		vis_esp_healthbar.onMouseClick();
		vis_chams.onMouseClick();
	//	vis_xqz.onMouseClick();
	//	vis_skeleton.onMouseClick();
	}
	if (SelectedTab == 2) {
		misc_active.onMouseClick();
		misc_bunnyhop.onMouseClick();
		misc_autostrafe.onMouseClick();
		misc_fakelag.onMouseClick();
		misc_norecoil.onMouseClick();
	//	misc_teleport.onMouseClick();
		misc_novisualrecoil.onMouseClick();
		misc_nospread.onMouseClick();
		misc_force_nospread.onMouseClick();
	}
}

void onMouseHold() {
	if (SelectedTab == 0) {
		aim_fov.onMouseHold(); AimbotFov = aim_fov.getValue();
		aim_smoothBar.onMouseHold();
		aim_bone.onMouseHold(); selectedAimbotBone = aim_bone.getValue();
	}
	if (SelectedTab == 1) {
		glow_r.onMouseHold();
		glow_g.onMouseHold();
		glow_b.onMouseHold();
		glow_a.onMouseHold();
	}
	if (SelectedTab == 2) {
		misc_fakelag_factor.onMouseHold();
		misc_viewfov.onMouseHold();
	}
	if (SelectedTab == 4) {
		color_r.onMouseHold(); themeColor = Color(color_r.getValue(), color_g.getValue(), color_b.getValue(), 255);
		color_g.onMouseHold();
		color_b.onMouseHold();
		int newR = color_r.getValue() - 30;
		int newG = color_g.getValue() - 30;
		int	newB = color_b.getValue() - 30;

		if (newR < 0) {
			newR = abs(newR);
		}
		if (newG < 0) {
			newG = abs(newG);
		}
		if (newB < 0) {
			newB = abs(newB);
		}

		evenDarker = Color(newR, newG, newB, 255);
	}
	if (SelectedTab == 3) {
		hvh_x.onMouseHold();
		hvh_y.onMouseHold();
		//hvh_choke.onMouseHold();
	}
}