#include "stdafx.h"

struct tIO
{
    HANDLE ProcessHandle;
    DWORD ClientModuleBaseAddress;
    DWORD EngineModuleBaseAddress;
	bool Thead2Active;
	bool Thead3Active;
	bool Thead4Active;
	bool Thead5Active;
    struct tIOFunctions
    {
        D3DCOLOR WColor(std::wstring ColorInfo);
        HANDLE GetProcessHandleByName(std::wstring ProcessName);
        DWORD GetModuleAddressByName(std::wstring ModuleName, HANDLE Process);
        bool Attach();
        void Detach();
		bool IsProcessActive();
        bool ReadSettingsFile(std::wstring* Data);
        bool ParseSettings(std::wstring* Data);
        bool LoadSettings();
        template <typename ReadType> void Read(ReadType* Buffer,DWORD Address)
        {
			if (IsProcessActive())
			{ 
				ReadProcessMemory(pIO.ProcessHandle, (LPVOID)(Address), Buffer, sizeof(ReadType), 0);
			}
        }
        template <typename WriteType> void Write(WriteType* Data, DWORD Address)
        {
			if (IsProcessActive())
			{
				WriteProcessMemory(pIO.ProcessHandle, (LPVOID)(Address), Data, sizeof(WriteType), 0);
			}
        }
    } Functions;

};

extern tIO pIO;