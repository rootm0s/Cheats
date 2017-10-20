#ifndef BUNNYHOP_H
#define BUNNYHOP_H
#include "Engine.h"
#include "Settings.h"
#include "Decrypt.h"

class Bunnyhop {
public:
	void run() {
		if (!(Settings()->bhop_key || Settings()->bhopEnabled)) {
			return;
		}
		while (true) {
			if (Engine()->me.team > 1 && Engine()->me.team < 4) {
				DWORD dwPla = Engine()->Memory()->Read<DWORD>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwLocalPlayer);
				int fhh = Engine()->Memory()->Read<DWORD>(dwPla + Engine()->Offsets()->dwFlags);
				if ((GetAsyncKeyState(Settings()->bhop_key) & 0x8000) && Engine()->me.hp > 0 && (fhh & 0x1 == 1)) {
					Engine()->Memory()->Write<int>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwJump, 5);
					Sleep(100);
					Engine()->Memory()->Write<int>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwJump, 4);
				}
			}
			else {
				Sleep(1000);
			}
			Sleep(1);
		}
	}
};

#endif