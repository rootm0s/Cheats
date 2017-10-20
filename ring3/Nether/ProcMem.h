#ifndef PROCMEM_H //If Not Defined
#define PROCMEM_H //Define Now

#include "Includes.h"

class ProcMem{

public:

	//STORAGE
	HANDLE hProcess;
	DWORD dwPID;

	//FUNCTION PROTOTYPES
	ProcMem();
	~ProcMem();
    __int32 GetLength(char *chArray);
	bool Locate(int iAry[], int iSize, int iVal);
	void Patch(DWORD dwAddress, char *Patch_Bts, char *Default_Bts);
	void GetProcess(char* ProcessName);
	void Inject(DWORD dwAddress, char *Inj_Bts, char *Def_Bts, BOOL Type);
	DWORD AOB_Scan(DWORD dwAddress, DWORD dwEnd, char *Bytes);
	DWORD Module(LPSTR ModuleName);

#pragma region TEMPLATE MEMORY FUNCTIONS

	//REMOVE READ/WRITE PROTECTION
	template <class cData>
	void Protection(DWORD dwAddress)
	{	   
		if(!bProt)
			VirtualProtectEx(hProcess, (LPVOID)dwAddress, sizeof(cData), PAGE_EXECUTE_READWRITE, &dwProtection); //Remove Read/Write Protection By Giving It New Permissions
		else
			VirtualProtectEx(hProcess, (LPVOID)dwAddress, sizeof(cData), dwProtection, &dwProtection); //Restore The Old Permissions After You Have Red The dwAddress
		
		bProt = !bProt;
	}

	//READ MEMORY 
	template <class cData>
	cData Read(DWORD dwAddress)
	{
		cData cRead; //Generic Variable To Store Data
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL); //Win API - Reads Data At Specified Location 
		return cRead; //Returns Value At Specified dwAddress
	}
    	
	//READ MEMORY - Pointer
	template <class cData>
	cData Read(DWORD dwAddress, char *Offset, BOOL Type)
	{
		//Variables
		__int32 iSize = GetLength(Offset) -1; //Size Of *Array Of Offsets 
		dwAddress = Read<DWORD>(dwAddress); //HEX VAL
	
		//Loop Through Each Offset & Store Hex Value (Address) In dwTMP
		for (__int32 i = 0; i < iSize; i++)	
			dwAddress = Read<DWORD>(dwAddress + Offset[i]);

		if (!Type)
			return dwAddress + Offset[iSize]; //FALSE - Return Address
		else
			return Read<cData>(dwAddress + Offset[iSize]); //TRUE - Return Value
	}

	//WRITE MEMORY
	template <class cData>
	void Write(DWORD dwAddress, cData Value)
	{ WriteProcessMemory(hProcess, (LPVOID)dwAddress, &Value, sizeof(cData), NULL); }

	//WRITE MEMORY - Pointer
	template <class cData>
	void Write(DWORD dwAddress, char *Offset, cData Value)
	{ Write<cData>(Read<cData>(dwAddress, Offset, false), Value); }
	
#pragma endregion	

private:

	//STORAGE
    DWORD dwProtection, dwCaveAddress;

	//MISC
	BOOL bPOn, bIOn, bProt;
};
#endif


