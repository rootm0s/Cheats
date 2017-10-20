#include "stdafx.h"

struct tTriggerbot
{
	int CurrentTargetIndex;
	float Timer;
	tLocalPlayerEntity::tLocalPlayerEntityInfo LocalPlayerEntityInfo;
	tGeneralPlayerEntity::tGeneralPlayerEntityInfo GeneralPlayerEntityInfo;
    struct tTriggerbotFunctions
    {
		int GetNewTarget();
        void Tick();
    } Functions;
};

extern tTriggerbot pTriggerbot;