/*
HLClient rebuild
22.08.2014
*/
namespace ValveSDK
{
	class HLCLient
	{
	public:
		ClientClass* GetAllClasses(VOID)
		{
			typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 8)(this);
		}

		int GetScreenWidth(VOID)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 63)(this);
		}

		int GetScreenHeight(VOID)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 64)(this);
		}

		void IN_DeactivateMouse(VOID)
		{
			typedef void(__thiscall* OriginalFn)(PVOID);
			getvfunc<OriginalFn>(this, 16)(this);
		}

		/*int IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding)
		{
		typedef int(__thiscall* OriginalFn)(PVOID, int, ButtonCode_t, const char*);
		return getvfunc<OriginalFn>(this, 20)(this, eventcode, keynum, pszCurrentBinding);
		}*/
	};
}