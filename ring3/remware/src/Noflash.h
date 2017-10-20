#ifndef NOFLASH_H
#define NOFLASH_H

#include "Engine.h"
#include "Settings.h"

class Noflash {
public: 
	void run() {
		if (!(Settings()->noflash_key || Settings()->noflashEnabled)) {
            return;
        }
		while (true) {
			if (!(Settings()->noflashEnabled && Engine()->me.is_alive)) {
                Sleep(5000);
			}
			Engine()->Memory()->Write<float>(Engine()->me.dwBase + Engine()->Offsets()->dwFlashDuration, 0.f);
			Sleep(50);
		}
	}
};
#endif