namespace ValveSDK
{
	class IGameEvent
	{
	public:
		const char* GetName()
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 1)(this);
		}

		int GetInt(const char *keyName, int defaultValue)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, const char *, int);
			return getvfunc<OriginalFn>(this, 6)(this, keyName, defaultValue);
		}

		const char *GetString(const char *keyName, const char *defaultValue)
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID, const char *, const char*);
			return getvfunc<OriginalFn>(this, 9)(this, keyName, defaultValue);
		}
	};
}