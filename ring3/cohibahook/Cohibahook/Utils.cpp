#include "main.h"
namespace Base
{
	namespace Utils
	{
		ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask,
			ULONG uCodeBase, ULONG uSizeOfCode)
		{
			BOOL bPatternDidMatch = FALSE;
			HMODULE hModule = GetModuleHandle(sModuleName.c_str());

			if (!hModule)
				return 0x0;

			PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
			PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
			PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

			if (uCodeBase == 0x0)
				uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

			if (uSizeOfCode == 0x0)
				uSizeOfCode = pOptionalHeader->SizeOfCode;

			ULONG uArraySize = sMask.length();

			if (!uCodeBase || !uSizeOfCode || !uArraySize)
				return 0x0;

			for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
			{
				for (size_t t = 0; t < uArraySize; t++)
				{
					if (*((PBYTE)i + t) == pbPattern[t] || sMask.c_str()[t] == '?')
						bPatternDidMatch = TRUE;

					else
					{
						bPatternDidMatch = FALSE;
						break;
					}
				}

				if (bPatternDidMatch)
					return i;
			}

			return 0x0;
		}

		HMODULE GetModuleHandleSafe(const char* pszModuleName)
		{
			HMODULE hmModuleHandle = NULL;

			do
			{
				hmModuleHandle = GetModuleHandle(pszModuleName);
				Sleep(1);
			} while (hmModuleHandle == NULL);

			return hmModuleHandle;
		}

	}

	namespace Debug
	{
		bool AttachDebugConsole(void)
		{
			FILE* g_pConStream;

			if (!AttachConsole(ATTACH_PARENT_PROCESS))
			{
				if (!AllocConsole())
					return false;
			}

			if (!SetConsoleTitle("DebugMessages"))
				return false;

			errno_t err = freopen_s(&g_pConStream, "CONOUT$", "w", stdout);

			if (err != 0)
				return false;

			return true;
		}
		void LOG(const char* output)
		{
			printf(output);
			printf("\n");
		}
	}

}