#include "stdafx.h"

void tBhop::tBhopFunctions::Jump(DWORD Input)
{
	keybd_event(VK_LCONTROL, MapVirtualKeyEx(VK_LCONTROL, MAPVK_VK_TO_VSC_EX, 0), KEYEVENTF_SCANCODE, 0);
	keybd_event(VK_SPACE, Input, KEYEVENTF_SCANCODE, 0);
	keybd_event(VK_SPACE, Input, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, MapVirtualKeyEx(VK_LCONTROL, MAPVK_VK_TO_VSC_EX, 0), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
}


void tBhop::tBhopFunctions::Tick()
{
	if (pGlobalVars.BhopSettings.Enabled)
	{
		if (GetAsyncKeyState(pGlobalVars.BhopSettings.ActivationKey) < 0)
		{
			if (pBhop.Input)
			{
				pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pBhop.LocalPlayerEntityInfo);
				if (pBhop.LocalPlayerEntityInfo.Valid)
				{
					if (!pLocalPlayerEntity.Functions.IsJumping(pBhop.LocalPlayerEntityInfo.Flags))
					{
						Jump(pBhop.Input);
					}
				}
			}
			else
			{
				pBhop.Input = MapVirtualKeyEx(VK_SPACE, MAPVK_VK_TO_VSC_EX, 0);
			}
		}
	}
}