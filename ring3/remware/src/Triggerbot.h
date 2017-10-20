#ifndef TRIGGERBOT_H
#define TRIGGERBOT_H

#include "Engine.h"
#include "Settings.h"
#include "Decrypt.h"

class Triggerbot {
public: 
	void run() {
		bool autoAttacking = false;
		if (!(Settings()->trigger_key || Settings()->triggerEnabled)) {
            return;
        }
		while (true) {
			if (autoAttacking && KeyUp(VK_LBUTTON)) {
                Engine()->Memory()->Write<int>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwAttack, 4);
                autoAttacking = false;
			}
			if (!Engine()->me.is_alive) {
                Sleep(1500);
                continue;
			}
			if (!isTriggerEnabled()) {
                continue;
            }
            if (Engine()->me.weapon_ammo > 0 && inCrosshair()) {
                Sleep(Settings()->triggerdelay);
                Engine()->Memory()->Write<int>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwAttack, 5);
                autoAttacking = true;
                Sleep(1);
            }
		}
	}
private:
    bool isTriggerEnabled() {
        if (!Settings()->triggerEnabled) {
            return false;       
        }
        // If we're not using the holdkey, then it's always enabled
        if (Settings()->trigger_holdkey == 0) {
            return true;
        }
        // Otherwise it's only enabled if holdkey is currently being held
        return KeyDown(Settings()->trigger_holdkey);
    }
	bool inCrosshair() {
		int idInX = Engine()->Memory()->Read<int>(Engine()->me.dwBase + Engine()->Offsets()->dwInCross);

		if (idInX != 0) {
			for (int i = 0; i < Engine()->iEnemies; i++) {
				if (Engine()->entEnemies[i].id == idInX) {
					return (Engine()->entEnemies[i].hp > 0);
				}
			}
		}
		return false;
	}
};
#endif