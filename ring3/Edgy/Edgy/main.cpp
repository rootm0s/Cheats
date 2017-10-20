/*
	xIntent
	This code is VERY outdated, copy pasting will do nothing.  Use this to learn (or whatever idc).
*/

#include "Aimbot.h"
#include "Bhop.h"
#include "ProcMem.h"
#include "CGeneralEntity.h"
#include "Interfaces.h"
#include "Offsets.h"
#include <Windows.h>
#include <iostream>
#include <ctime> // For std::chrono
#include <thread> // For std::this_thread

Aimbot cAimbot;
Bhop cBhop;

int main() {
	b.Process("csgo.exe");
	while (!CInterfaces::UpdateInterfaces()) {/*Waiting for game*/ }
	CGeneralEntity LocalPlayer;
	while (!LocalPlayer.SetAndValidate()) {/*Waiting for localplayer to be valid*/
		std::cout << "Validating Local!\n";
		Sleep(500);
	}
	system("cls");
	std::cout << "\n\nPassed all tests!";
	while (true) {
		LocalPlayer.SetAndValidate();
		cAimbot.Tick(LocalPlayer);
		cBhop.Tick(LocalPlayer);
		Sleep(1);
	}

}