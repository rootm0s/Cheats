#include "main.h"
Base::Utils::CVMTHookManager g_pInKeyEventVMT;

int __stdcall hkdInKeyEvent(int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding)
{
	static DWORD dwOriginInKeyEvent = g_pInKeyEventVMT.dwGetMethodAddress(20);
	if (eventcode)
	{
		if (keynum == KEY_INSERT)
		{
			g_pConsole->HandleCommands("1aa");
			return 0;
		}

		if (g_pConsole->IsActive() == true)
		{
			if (keynum != 66)
				g_pConsole->HandleKeys(keynum);
			return 0;
		}
	}

	__asm
	{
		push pszCurrentBinding;
		push keynum;
		push eventcode;
		call dwOriginInKeyEvent;
	}
}