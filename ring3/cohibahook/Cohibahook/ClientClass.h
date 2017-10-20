/*
Clientclass rebuild
22.08.2014
*/
namespace ValveSDK
{
	class ClientClass
	{
	public:
		const char* GetName(void)
		{
			return *(char**)(this + 0x8);
		}
		RecvTable* GetTable()
		{
			return *(RecvTable**)(this + 0xC);
		}
		ClientClass* NextClass()
		{
			return *(ClientClass**)(this + 0x10);
		}
		int GetClassID(void)
		{
			return *(int*)(this + 0x14);
		}
		ClientClass* GetAllClasses(void)
		{
			typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 8)(this);
		}
	};
}