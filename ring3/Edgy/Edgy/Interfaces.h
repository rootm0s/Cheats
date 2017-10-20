#ifndef INTERFACES_H
#define INTERFACES_H

#include "windows.h"
struct Interfaces {
	DWORD ClientDLL;
	DWORD EngineDLL;
	DWORD EnginePointer;
}; extern Interfaces I;

namespace CInterfaces {
	bool UpdateInterfaces();
}

#endif