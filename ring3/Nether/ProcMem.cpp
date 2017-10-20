#include "ProcMem.h"

using namespace std;

ProcMem::ProcMem(){
	//Constructor For Class, Do Not Remove!
}

__int32 ProcMem::GetLength(char *chArray){

	//Loop Through *chArray To Get Amount Of Bytes
	for(__int32 iLength = 1; iLength < MAX_PATH; iLength++)
		if (chArray[iLength] == '*')	
			return iLength;	

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
} 

bool ProcMem::Locate(int *iAry, int iSize, int iVal){

	for(__int32 i = 0; i != iSize; i++)
		if(iVal == iAry[i])
			return true;

	return false;
}

void ProcMem::Patch(DWORD dwAddress, char *Patch_Bts, char *Default_Bts){
 
	//Variables
	__int32 iSize = GetLength(Default_Bts);		

	//Loop Through Addresses Writing Bytes
	if (!bPOn)
		for(__int32 i = 0; i < iSize; i++)	
			Write<BYTE>(dwAddress + i, Patch_Bts[i]);		
	else
		for(__int32 i = 0; i < iSize; i++)	 	
			Write<BYTE>(dwAddress + i, Default_Bts[i]);        		        	

	bPOn = !bPOn; 
}

void ProcMem::GetProcess(char* ProcessName){

	//Variables
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //Snapshot To View All Active Processes
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry); //Declare Structure Size And Populate It

	//Loop Through All Running Processes To Find Process
	do
	    if(!strcmp(pEntry.szExeFile, ProcessName))
		{
			//Store Process ID
			dwPID = pEntry.th32ProcessID;
			CloseHandle(hPID);

			//Give Our Handle All Access Rights 
	        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); 
			return;
		}
	while(Process32Next(hPID, &pEntry));

	cout << "\nPROCESS: Process Not Found\n";
	system("pause");
    exit(0);
}

void ProcMem::Inject(DWORD dwAddress, char *Inj_Bts, char *Def_Bts, BOOL Type){
	
	//Variables
	__int32 i_ISize = GetLength(Inj_Bts);
	__int32 i_DSize = GetLength(Def_Bts); 

	if(!bIOn)
	{		
		//NOP All Bytes In The Array Past The 5th Byte	
		if(i_DSize >= 5)
			for (__int32 i = 6; i < i_DSize; i++)
				Write<BYTE>(dwAddress + i, 0x90);	
		else {cout << "\nINJECTION: Default Bytes Must Be More Than 5\n"; return;}

		//Create Codecave
		dwCaveAddress = (DWORD)VirtualAllocEx(hProcess, NULL,  i_ISize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		//Calculate Jmp/Return Distances In Bytes To Write
		DWORD dwRetJmp = (dwAddress + i_DSize) - dwCaveAddress - 5; //(NextInstruction - CaveAddress - 5) - is correct equation.
		DWORD dwBaseJmp = dwCaveAddress - dwAddress - 5; //Base Jmp

		//Loop Through Each Address Writing Inj_Bts Inside The Codecave
		for (__int32 i = 0; i <= i_ISize; i++)			
			Write<BYTE>(dwCaveAddress+i, Inj_Bts[i]);
		
		//Write The Return Distance In Bytes (E9 = Jmp | E8 = Call) To The Original Address
		Write<BYTE>(dwCaveAddress + i_ISize, Type ? 0xE9 : 0xE8);
		Write<DWORD>(dwCaveAddress + i_ISize + 1, dwRetJmp);
		
		//Write The Jump From The Original Address To The Codecave
		Write<BYTE>(dwAddress, Type ? 0xE9 : 0xE8);
		Write<DWORD>(dwAddress + 1, dwBaseJmp);
	}	

	if(bIOn)
	{			
		//Restore Original Bytes
		for(__int32 i = 0; i < i_DSize; i++)					
			Write<BYTE>(dwAddress + i, Def_Bts[i]);	

		//Clean Up! (DeAllocate CodeCave)
		VirtualFreeEx(hProcess, (LPVOID)dwCaveAddress, i_ISize + 5, MEM_DECOMMIT);
	}
    bIOn = !bIOn; 
} 

DWORD ProcMem::AOB_Scan(DWORD dwAddress, DWORD dwEnd, char *Bytes){

	//VARIABLES
	__int32 iBytesToRead(0);
	__int32 length = GetLength(Bytes);

	//Increase Start Address Till It Reaches The End Address
	for(;dwAddress < dwEnd; dwAddress++) 	
		if(Read<BYTE>(dwAddress) == Bytes[iBytesToRead] || Bytes[iBytesToRead] == '?') 	
			iBytesToRead++;
		else if(iBytesToRead >= length)
			return dwAddress - iBytesToRead;					
		else iBytesToRead = 0;		

	cout << "\nAOB_SCAN: Failed To Find Byte Pattern\n";
	return 0;
}

DWORD ProcMem::Module(LPSTR ModuleName){
       
	//Variables
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); //Take A Module Snapshot Of The Process (Grab All Loaded Modules)
	MODULEENTRY32 mEntry; //Declare Module Entry Structure
	mEntry.dwSize = sizeof(mEntry); //Declare Structure Size And Populate It With Loaded Modules
	
	//Scan For Module By Name
	do 
		if(!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);			
			return (DWORD)mEntry.modBaseAddr;	
		}
	while(Module32Next(hModule, &mEntry));
	
	cout << "\nMODULE: Process Platform Invalid\n";
	return 0;
}

ProcMem::~ProcMem(){

	//Clean Up! (Close Handle - Not Needed Anymore)
	CloseHandle(hProcess);
}