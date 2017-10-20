namespace ValveSDK
{
	class IGameEventManager
	{
	public:
		bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, IGameEventListener2*, const char*, bool);
			return getvfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
		}

		bool FireEventClientSide(IGameEvent *event)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, IGameEvent*);
			return getvfunc<OriginalFn>(this, 8)(this, event);
		}
	};

}