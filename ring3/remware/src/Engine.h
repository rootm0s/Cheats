#ifndef ENGINE_H
#define ENGINE_H
#include <thread>
#include "PMemory.h"
#include "Offsets.h"
#include "Decrypt.h"

template <typename T, size_t N>
size_t countof(T (&array)[N]) {
    return N;
}

struct Entity {
	DWORD dwBase;
	int id;
	int hp;
	int team;
	int weapon_id;
	int weapon_ammo;
	bool is_dormant;
	bool is_alive;
};

class CEngine {
public:
	CEngine();

	void start();

	void update();

	bool initialized();

	PMemory* Memory();
	COffsets* Offsets();
	Entity* getEntityByBase(DWORD dwBase);

	PModule* getClientModule();
	PModule* getEngineModule();

	int iFriendlies;
	int iEnemies;
	Entity entEnemies[32];
	Entity entFriendlies[32];
	Entity me;
private:
	PMemory mem;
	PModule _modClient, _modEngine;
	COffsets _ofs;
	bool _init;

	void updateEntityData(Entity* e, DWORD dwBase);
};

inline CEngine* Engine() {
	static CEngine instance;
	return &instance;
}

#endif