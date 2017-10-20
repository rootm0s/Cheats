#ifndef PMEMORY_H
#define PMEMORY_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <stdarg.h>

struct PModule {
	DWORD dwBase;
	DWORD dwSize;
};

class PMemory {

public:
	PMemory();
	~PMemory();

	bool Attached;
	bool Attach(char* pName, DWORD rights = PROCESS_ALL_ACCESS);
	void Detach();

	PModule GetModule(char* moduleName);
	HMODULE LoadModule(const char* moduleName);

	template <class T>
	T Read(DWORD addr) {
		T _read;
		ReadProcessMemory(_process, (LPVOID)addr, &_read, sizeof(T), NULL);
		return _read;
	}

	template <class T>
	void Write(DWORD addr, T val) {
		WriteProcessMemory(_process, (LPVOID)addr, &val, sizeof(T), NULL);
	}

	template <class T>
	void WriteProtect(DWORD addr, T val) {
		PDWORD oldProtect;
		VirtualProtectEx(_process, (LPVOID)addr, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
		Write(addr, val);
		VirtualProtectEx(_process, (LPVOID)addr, sizeof(T), oldProtect, NULL);
	}

	DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask);
	DWORD FindPatternArr(DWORD start, DWORD size, const char* mask, int count, ...);

private:
	HANDLE _process;
	DWORD pID;
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask);
};

#endif