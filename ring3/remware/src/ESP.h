#ifndef ESP_H
#define ESP_H

#include "Engine.h"
#include "Settings.h"

struct GlowObjectDefinition_t
{
    DWORD dwBase;
    float r;
    float g;
    float b;
    float a;
    uint8_t unk1[16];
    bool m_bRenderWhenOccluded;
    bool m_bRenderWhenUnoccluded;
    bool m_bFullBloom;
    uint8_t unk2[14];
};

class ESP {
public: 
	void run() {
		int objectCount;
		DWORD pointerGlow;
		float playerhp;
		bool enemyHealthBased  = Settings()->esp_enemyhealth;
		bool friendHealthBased = Settings()->esp_friendhealth;
        float alphaFriend = sanitizeChannel(Settings()->esp_friend);
        float alphaEnemy  = sanitizeChannel(Settings()->esp_enemy);
        float alphaInX    = sanitizeChannel(Settings()->esp_enemyInX);
        if (!(Settings()->esp_key || Settings()->espEnabled)) {
            return;
        }
		while (true) {
            
            if (!Settings()->espEnabled) {
                Sleep(3000);
                continue;
			}
            
            pointerGlow = Engine()->Memory()->Read<DWORD>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwGlow);
            objectCount = Engine()->Memory()->Read<DWORD>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwGlow + 0x4);
            if (!(pointerGlow != NULL && objectCount > 0)) {
                Sleep(3000);
                continue;
            }
            
            for (int i = 0; i < objectCount; i++) {
                
                DWORD mObj = pointerGlow + (i * sizeof(GlowObjectDefinition_t));
                GlowObjectDefinition_t glowObj = Engine()->Memory()->Read<GlowObjectDefinition_t>(mObj);
                Entity* Player = Engine()->getEntityByBase(glowObj.dwBase);
                if (!(glowObj.dwBase && Player && !Player->is_dormant && Player->is_alive)) {
                    continue;
                }
                
                // Player->hp can be greater than 100.0f on custom servers.
                playerhp = Player->hp >= 100 ? 100.0f : (float) Player->hp / 100.0f;
                if (Engine()->me.team == Player->team) {
                    glowPlayer(glowObj, 0, friendHealthBased ? (Player->hp > 75 ? 1 : playerhp * 0.75f) : 1, 0, alphaFriend);
                } else if (inCrosshair(Player->id)) {
                    
                } else {
                    glowPlayer(glowObj, 0, enemyHealthBased ? (Player->hp > 75 ? 1 : playerhp * 0.75f) : 1, 1, alphaEnemy);
                }
                Engine()->Memory()->Write<GlowObjectDefinition_t>(mObj, glowObj);
                
            }
            
			Sleep(1);
		}
		
	}

private:
    void glowPlayer(GlowObjectDefinition_t &glowObj, float r, float g, float b, float a) {
        glowObj.r = r;
        glowObj.g = g;
        glowObj.b = b;
        glowObj.a = a;
        glowObj.m_bRenderWhenOccluded = true;
        //glowObj.m_bRenderWhenUnoccluded = false;
        //glowObj.m_bFullBloom = false;
    }
	bool inCrosshair(int mObjId) {
		int idInX = Engine()->Memory()->Read<int>(Engine()->me.dwBase + Engine()->Offsets()->dwInCross);
		return (idInX == mObjId);
	}
	float sanitizeChannel(int value) {
        if (value > 255) value = 255;
        if (value < 0) value = 0;
        return (float) value / 255;
	}
};

#endif