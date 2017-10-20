#include "Func.h"

__int32 main()
{
	//initalize
	Engine info;
	
	while(1)
	{
		//HOTKEYS
		if(GetAsyncKeyState(VK_F1)&1)
		{
			info.tOn = !info.tOn;
			_cwprintf(L"Triggerbot Toggled\n");
		}

		if(GetAsyncKeyState(VK_F2)&1)
		{
			info.aOn = !info.aOn;
			_cwprintf(L"Aimbot Toggled\n");
		}
		
		if(GetAsyncKeyState(VK_F3)&1)
		{
			info.eOn = !info.eOn;		
			_cwprintf(L"ESP Toggled\n");
		}

		if(GetAsyncKeyState(VK_F4)&1)
			return 0;


		//MISC
		info.i_Count = 1;
		info.Reading(info.eOn || info.aOn || info.tOn);

		//FUNCTIONS
		info.Trigger();//Triggerbot, Current xhair ID
		info.ESP();	//ESP
		info.Aimbot();

	}
	system("pause");
	return 0;
}

