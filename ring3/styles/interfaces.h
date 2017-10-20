typedef long* (_cdecl* QueryInterfaceFn)();

class InterfaceList
{
public:
	void Init(char* bin)
	{
		void* fn = GetProcAddress(GetModuleHandleA(bin),"CreateInterface");

		long jmp = (long)fn + 4;

		base = **(InterfaceList***)((*(long*)(jmp + 1) + jmp + 5) + 6);
	}
	long* GetInterface(char* name)
	{
		InterfaceList* k = base;

		char str[64];

		int len = 0;

		while (k)
		{
			len = strlen(k->name);

			strcpy_s(str,k->name);

			int i = 3;

			while (i)
				str[len-i--] = 0;			

			if (cmp(str,name))
				return k->query();

			k = k->next;
		}

		return 0;
	}

	QueryInterfaceFn query;
	char* name;
	InterfaceList* next;
	InterfaceList* base;
};

#include "client.h"
#include "engine.h"
#include "vgui.h"