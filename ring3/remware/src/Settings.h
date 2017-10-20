#ifndef SETTINGS_H
#define SETTINGS_H

#include "Engine.h"

#include <windows.h>
#include <iostream>
#include <string>
#include "Decrypt.h"

#define KeyDown(vkey) ((GetAsyncKeyState(vkey) & 0x8000) ? true : false)
#define KeyUp(vkey) ((GetAsyncKeyState(vkey) & 0x8000) ? false : true)

#define caseStringify(x) case x: return std::string(#x)
#define CStringify(x) std::to_string(x).c_str()

class CSettings {
public:
    LPCTSTR iniFile = TEXT(".\\remware.ini");
	int bhop_key          = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"bhop", -1, iniFile);
    int noflash_key       = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"noflash", -1, iniFile);
    int esp_key           = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"glow", -1, iniFile); 
    int trigger_key       = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"trigger", -1, iniFile);
    int trigger_holdkey   = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"trigger(hold)", -1, iniFile); 
	int exit_key          = GetPrivateProfileInt((LPCTSTR)"binds", (LPCTSTR)"force quit", -1, iniFile);
    
    int triggerdelay      = GetPrivateProfileInt((LPCTSTR)"trigger", (LPCTSTR)"delay", 5, iniFile);
    
    int esp_friend        = GetPrivateProfileInt((LPCTSTR)"glow", (LPCTSTR)"team alpha", 200, iniFile);
    int esp_enemy         = GetPrivateProfileInt((LPCTSTR)"glow", (LPCTSTR)"enemy alpha", 200, iniFile);
    int esp_enemyInX      = GetPrivateProfileInt((LPCTSTR)"glow", (LPCTSTR)"crosshair alpha", 255, iniFile);
    bool esp_enemyhealth  = GetPrivateProfileInt((LPCTSTR)"glow", (LPCTSTR)"enemy health", true, iniFile);
    bool esp_friendhealth = GetPrivateProfileInt((LPCTSTR)"glow", (LPCTSTR)"team health", false, iniFile);

    bool bhopEnabled      = GetPrivateProfileBool("autostart", "bhop", true, iniFile);
    bool espEnabled       = GetPrivateProfileBool("autostart", "glow", true, iniFile);
	bool triggerEnabled   = GetPrivateProfileBool("autostart", "trigger", true, iniFile);
	bool noflashEnabled   = GetPrivateProfileBool("autostart", "noflash", true, iniFile);
	
    void writeConfig();
	void run();
	std::string vkToString(int vk);
	int whichKeyIsPressed(int iDefault);
private:
	bool GetPrivateProfileBool(const char* section, const char* key, bool def_value, const char* filename);
};

inline CSettings* Settings() {
	static CSettings instance;
	return &instance;
}
#endif