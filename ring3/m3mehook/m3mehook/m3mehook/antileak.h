class CAntiLeak
{
public:
	void ErasePE();
}; CAntiLeak *AntiLeak = new CAntiLeak;

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}