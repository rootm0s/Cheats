#include "Settings.h"
#include "Status.h"

void CSettings::run() {
    bhopEnabled = bhop_key != 0;
    
	while (true) {
        
        if (GetAsyncKeyState(trigger_key)) {
            triggerEnabled = !triggerEnabled;
            Status()->refresh();
        }
        
		if (GetAsyncKeyState(esp_key)) {
            espEnabled = !espEnabled;
            Status()->refresh();
        }
        
        if (GetAsyncKeyState(noflash_key)) {
			noflashEnabled = !noflashEnabled;
			Status()->refresh();
		}
		
		Sleep(200);
	}
}

bool CSettings::GetPrivateProfileBool(const char* section, const char* key, bool def_value, const char* filename) {
    return GetPrivateProfileInt(section, key, def_value ? 1 : 0, filename);
}

void CSettings::writeConfig() {
    bool setHotkeys = (bhop_key == -1) || (noflash_key == -1) || (esp_key == -1) || (trigger_key == -1) || (trigger_holdkey == -1) || (exit_key == -1);
    
    if (!setHotkeys) {
        return;
    }
    
    std::cout << "___set_binds___\n\n";
    std::cout << "press the key you wanna use for the feature\n";
    std::cout << "press ENTER to use default key\n";
    std::cout << "press ESC to disable the feature\n\n";
    
    Sleep(2000);
    
	if (bhop_key == -1) {
		std::cout << "bhop(hold) -> ";
		bhop_key = whichKeyIsPressed(VK_XBUTTON1);
		std::cout << vkToString(bhop_key) << "\n";
	}
    
	if (noflash_key == -1) {
		std::cout << "noflash -> ";
		noflash_key = whichKeyIsPressed(VK_F5);
		std::cout << vkToString(noflash_key) << "\n";
	}
    
    if (esp_key == -1) {
        std::cout << "glow -> ";
        esp_key = whichKeyIsPressed(VK_F6);
        std::cout << vkToString(esp_key) << "\n";
    }

	if (trigger_key == -1) {
        std::cout << "trigger(toggle) -> ";
        trigger_key = whichKeyIsPressed(VK_F7);
        std::cout << vkToString(trigger_key) << "\n";
	}
	
	if (trigger_holdkey == -1) {
        std::cout << "trigger(hold) -> ";
        trigger_holdkey = whichKeyIsPressed(VK_MENU);
        std::cout << vkToString(trigger_holdkey) << "\n";
    }
	
	if (exit_key == -1) {
        std::cout << "force quit -> ";
        exit_key = whichKeyIsPressed(VK_F9);
        std::cout << vkToString(exit_key) << "\n";
    }
    
    WritePrivateProfileString("binds", "glow", CStringify(esp_key), iniFile);
	WritePrivateProfileString("binds", "trigger", CStringify(trigger_key), iniFile);
    WritePrivateProfileString("binds", "trigger(hold)", CStringify(trigger_holdkey), iniFile);
    WritePrivateProfileString("binds", "noflash", CStringify(noflash_key), iniFile);
    WritePrivateProfileString("binds", "bhop", CStringify(bhop_key), iniFile);
    WritePrivateProfileString("binds", "force quit", CStringify(exit_key), iniFile);
    
    WritePrivateProfileString("autostart", "glow", "1", iniFile);
	WritePrivateProfileString("autostart", "trigger", "1", iniFile);
    WritePrivateProfileString("autostart", "noflash", "1", iniFile);
    WritePrivateProfileString("autostart", "bhop", "1", iniFile);
    
    WritePrivateProfileString("trigger", "delay", "5", iniFile);
    
    WritePrivateProfileString("glow", "FriendAlpha", "200", iniFile);
    WritePrivateProfileString("glow", "EnemyAlpha", "200", iniFile);
    WritePrivateProfileString("glow", "CrosshairAlpha", "255", iniFile);
    WritePrivateProfileString("glow", "EnemyHealth", "1", iniFile);
    WritePrivateProfileString("glow", "FriendHealth", "0", iniFile);
}

std::string CSettings::vkToString(int vk) {
    if (vk == 0) return std::string("NONE");
    char c[2] = {0};
    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z')) {
        c[0]=(char)vk; 
        return std::string(c); 
    }
    switch(vk) {
        caseStringify(VK_LBUTTON);
        caseStringify(VK_RBUTTON);
        caseStringify(VK_CANCEL);
        caseStringify(VK_MBUTTON);
        caseStringify(VK_XBUTTON1);
        caseStringify(VK_XBUTTON2);
        caseStringify(VK_BACK);
        caseStringify(VK_TAB);
        caseStringify(VK_CLEAR);
        caseStringify(VK_RETURN);
        caseStringify(VK_SHIFT);
        caseStringify(VK_CONTROL);
        caseStringify(VK_MENU);
        caseStringify(VK_PAUSE);
        caseStringify(VK_CAPITAL);
        caseStringify(VK_KANA);
        caseStringify(VK_JUNJA);
        caseStringify(VK_FINAL);
        caseStringify(VK_KANJI);
        caseStringify(VK_ESCAPE);
        caseStringify(VK_CONVERT);
        caseStringify(VK_NONCONVERT);
        caseStringify(VK_ACCEPT);
        caseStringify(VK_MODECHANGE);
        caseStringify(VK_SPACE);
        caseStringify(VK_PRIOR);
        caseStringify(VK_NEXT);
        caseStringify(VK_END);
        caseStringify(VK_HOME);
        caseStringify(VK_LEFT);
        caseStringify(VK_UP);
        caseStringify(VK_RIGHT);
        caseStringify(VK_DOWN);
        caseStringify(VK_SELECT);
        caseStringify(VK_PRINT);
        caseStringify(VK_EXECUTE);
        caseStringify(VK_SNAPSHOT);
        caseStringify(VK_INSERT);
        caseStringify(VK_DELETE);
        caseStringify(VK_HELP);
        caseStringify(VK_LWIN);
        caseStringify(VK_RWIN);
        caseStringify(VK_APPS);
        caseStringify(VK_SLEEP);
        caseStringify(VK_NUMPAD0);
        caseStringify(VK_NUMPAD1);
        caseStringify(VK_NUMPAD2);
        caseStringify(VK_NUMPAD3);
        caseStringify(VK_NUMPAD4);
        caseStringify(VK_NUMPAD5);
        caseStringify(VK_NUMPAD6);
        caseStringify(VK_NUMPAD7);
        caseStringify(VK_NUMPAD8);
        caseStringify(VK_NUMPAD9);
        caseStringify(VK_MULTIPLY);
        caseStringify(VK_ADD);
        caseStringify(VK_SEPARATOR);
        caseStringify(VK_SUBTRACT);
        caseStringify(VK_DECIMAL);
        caseStringify(VK_DIVIDE);
        caseStringify(VK_F1);
        caseStringify(VK_F2);
        caseStringify(VK_F3);
        caseStringify(VK_F4);
        caseStringify(VK_F5);
        caseStringify(VK_F6);
        caseStringify(VK_F7);
        caseStringify(VK_F8);
        caseStringify(VK_F9);
        caseStringify(VK_F10);
        caseStringify(VK_F11);
        caseStringify(VK_F12);
        caseStringify(VK_F13);
        caseStringify(VK_F14);
        caseStringify(VK_F15);
        caseStringify(VK_F16);
        caseStringify(VK_F17);
        caseStringify(VK_F18);
        caseStringify(VK_F19);
        caseStringify(VK_F20);
        caseStringify(VK_F21);
        caseStringify(VK_F22);
        caseStringify(VK_F23);
        caseStringify(VK_F24);
        caseStringify(VK_NUMLOCK);
        caseStringify(VK_SCROLL);
        caseStringify(VK_OEM_NEC_EQUAL);
        caseStringify(VK_OEM_FJ_MASSHOU);
        caseStringify(VK_OEM_FJ_TOUROKU);
        caseStringify(VK_OEM_FJ_LOYA);
        caseStringify(VK_OEM_FJ_ROYA);
        caseStringify(VK_LSHIFT);
        caseStringify(VK_RSHIFT);
        caseStringify(VK_LCONTROL);
        caseStringify(VK_RCONTROL);
        caseStringify(VK_LMENU);
        caseStringify(VK_RMENU);
        caseStringify(VK_BROWSER_BACK);
        caseStringify(VK_BROWSER_FORWARD);
        caseStringify(VK_BROWSER_REFRESH);
        caseStringify(VK_BROWSER_STOP);
        caseStringify(VK_BROWSER_SEARCH);
        caseStringify(VK_BROWSER_FAVORITES);
        caseStringify(VK_BROWSER_HOME);
        caseStringify(VK_VOLUME_MUTE);
        caseStringify(VK_VOLUME_DOWN);
        caseStringify(VK_VOLUME_UP);
        caseStringify(VK_MEDIA_NEXT_TRACK);
        caseStringify(VK_MEDIA_PREV_TRACK);
        caseStringify(VK_MEDIA_STOP);
        caseStringify(VK_MEDIA_PLAY_PAUSE);
        caseStringify(VK_LAUNCH_MAIL);
        caseStringify(VK_LAUNCH_MEDIA_SELECT);
        caseStringify(VK_LAUNCH_APP1);
        caseStringify(VK_LAUNCH_APP2);
        caseStringify(VK_OEM_1);
        caseStringify(VK_OEM_PLUS);
        caseStringify(VK_OEM_COMMA);
        caseStringify(VK_OEM_MINUS);
        caseStringify(VK_OEM_PERIOD);
        caseStringify(VK_OEM_2);
        caseStringify(VK_OEM_3);
        caseStringify(VK_OEM_4);
        caseStringify(VK_OEM_5);
        caseStringify(VK_OEM_6);
        caseStringify(VK_OEM_7);
        caseStringify(VK_OEM_8);
        caseStringify(VK_OEM_AX);
        caseStringify(VK_OEM_102);
        caseStringify(VK_ICO_HELP);
        caseStringify(VK_ICO_00);
        caseStringify(VK_PROCESSKEY);
        caseStringify(VK_ICO_CLEAR);
        caseStringify(VK_PACKET);
    }
    c[0]=(char)vk;
    return std::string(c);
}

int CSettings::whichKeyIsPressed(int iDefault) {
    while (true) {
        for (int i = 1; i < 255; i++) {
            if (KeyDown(i)) {
                Sleep(500);
                return i == VK_RETURN ? iDefault : i == VK_ESCAPE ? 0 : i;
            }
        }
        Sleep(10);
    }
}