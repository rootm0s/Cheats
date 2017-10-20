#ifndef STATUS_H
#define STATUS_H
#include <iostream>
#include <windows.h>
#include "Engine.h"
#include "Settings.h"
#include "Decrypt.h"
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

class CStatus {
public: 
void CStatus::refresh() {
    SetColorAndBackground(WHITE, BLACK);
    system("cls");
    credits();
    main();
}
void CStatus::credits() {
    std::cout << "this cheat is made possible by n1BogOLlyQ, H3D35, Merccy, and TheoBaT\n\n";
}
void CStatus::main() {
	std::cout << "___offsets___\n";
	std::cout << "local player ------------------ 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwLocalPlayer) << "\n";
	std::cout << "entity list ------------------- 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwEntityList) << "\n";
	std::cout << "force attack ------------------ 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwAttack) << "\n";
	std::cout << "client state ------------------ 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwEnginePointer) << "\n";
	std::cout << "client state(set view angle) -- 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwSetViewAngle) << "\n";
	std::cout << "glow object manager ----------- 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwGlow) << "\n";
	std::cout << "force jump -------------------- 0x" << Engine()->Offsets()->toHex(Engine()->Offsets()->dwJump) << "\n";
    
	std::cout << "\n___binds___\n";
	printHotkey("noflash -------- ", Settings()->noflash_key);
	printHotkey("bhop(hold) ----- ", Settings()->bhop_key);
	printHotkey("glow ----------- ", Settings()->esp_key);
	printHotkey("trigger -------- ", Settings()->trigger_key);
	printHotkey("trigger(hold) -- ", Settings()->trigger_holdkey);
	printHotkey("force quit ----- ", Settings()->exit_key);
	
	std::cout << "\n___status___\n";
	printBool("glow ----------- ", Settings()->espEnabled);
	printBool("bhop ----------- ", Settings()->bhopEnabled);
	printBool("noflash -------- ", Settings()->noflashEnabled);
	printBool("trigger -------- ", Settings()->triggerEnabled);
}

private:
void printBool(char* msg, bool b) {
    SetColorAndBackground(WHITE, b ? GREEN : RED);
	std::cout << msg << " " << (b ? "ON " : "OFF") << "\n";
	SetColorAndBackground(WHITE, BLACK);
}
void printHotkey(char* msg, int key) {
    SetColorAndBackground(WHITE, key == 0 ? RED : BLACK);
	std::cout << msg << " " << Settings()->vkToString(key) << "\n";
	SetColorAndBackground(WHITE, BLACK);
}
void SetColorAndBackground(int ForgC, int BackC=0) {
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
};

inline CStatus* Status() {
	static CStatus instance;
	return &instance;
}
#endif