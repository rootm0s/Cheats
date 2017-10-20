#ifndef PROCMEM_H //If Not Defined
#define PROCMEM_H //Define Now

#define WIN32_LEAN_AND_MEAN //Excludes Headers We Wont Use (Increase Compile Time)


#include <windows.h> //Standard Windows Functions/Data Types
#include <iostream> //Constains Input/Output Functions (cin/cout etc..)
#include <TlHelp32.h> //Contains Read/Write Functions
#include <string> //Support For Strings
#include <sstream> //Supports Data Conversion

class ProcMem{
protected:

	//STORAGE
	//HANDLE hProcess;
	DWORD dwPID, dwProtection, dwCaveAddress;

	//MISC
	BOOL bPOn, bIOn, bProt;

public:
	HANDLE hProcess;
	//MISC FUNCTIONS
	ProcMem();
	~ProcMem();
    int chSizeOfArray(char *chArray); //Return Size Of External Char Array
	int iSizeOfArray(int *iArray); //Return Size Of External Int Array
	bool iFind(int *iAry, int iVal); //Return Boolean Value To Find A Value Inside An Int Array

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
		int iSize = iSizeOfArray(Offset) -1; //Size Of *Array Of Offsets 
        dwAddress = Read<DWORD>(dwAddress); //HEX VAL

		//Loop Through Each Offset & Store Hex Value (Address)
		for (int i = 0; i < iSize; i++)	
			dwAddress = Read<DWORD>(dwAddress + Offset[i]);

		if (!Type)
			return dwAddress + Offset[iSize]; //FALSE - Return Address
		else
			return Read<cData>(dwAddress + Offset[iSize]); //TRUE - Return Value
	}

	//WRITE MEMORY
	template <class cData>
    void Write(DWORD dwAddress, cData Value)
	{ 	
		WriteProcessMemory(hProcess, (LPVOID)dwAddress, &Value, sizeof(cData), NULL); 
	}

	//WRITE MEMORY - Pointer
	template <class cData>
	void Write(DWORD dwAddress, char *Offset, cData Value)
	{ 
			Write<cData>(Read<cData>(dwAddress, Offset, false), Value); 
	}
	
	//MEMORY FUNCTION PROTOTYPES
	virtual void Process(char* ProcessName); //Return Handle To The Process
	virtual void Patch(DWORD dwAddress, char *chPatch_Bts, char *chDefault_Bts); //Write Bytes To Specified Address
	virtual void Inject(DWORD dwAddress, char *chInj_Bts, char *chDef_Bts, BOOL Type); //Jump To A Codecave And Write Memory
	virtual DWORD AOB_Scan(DWORD dwAddress, DWORD dwEnd, char *chPattern); //Find A Byte Pattern
	virtual DWORD Module(LPSTR ModuleName); //Return Module Base Address

#pragma endregion	

};
#endif


