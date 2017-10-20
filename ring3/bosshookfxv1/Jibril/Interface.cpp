#include "Interface.h"

CInterface::CInterface()
{
	Offsets = new COffsets();
	NetVars = new CNetvars();
	Settings = new CSettings();
	Menu = new CMenu();
}

CInterface Interface;