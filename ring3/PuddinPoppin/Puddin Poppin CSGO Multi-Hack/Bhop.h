#include "stdafx.h"

struct tBhop
{
	tLocalPlayerEntity::tLocalPlayerEntityInfo LocalPlayerEntityInfo;
	DWORD Input;
	struct tBhopFunctions
	{
		void Tick();
		void Jump(DWORD Input);
	}Functions;
};

extern tBhop pBhop;