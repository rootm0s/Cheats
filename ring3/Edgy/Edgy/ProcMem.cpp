#include "ProcMem.h"

using namespace std;

#pragma region Misc Functions

ProcMem::ProcMem(){
	//Constructor For Class, Do Not Remove!
}

ProcMem::~ProcMem(){
	//De-Constructor
	//Clean Up! (Close Handle - Not Needed Anymore)
	CloseHandle(hProcess);
}

/* This Function Returns The Length Of External Char Arrays, SizeOf(Array) Fails For External Arrays. */
int ProcMem::chSizeOfArray(char *chArray){

	//Loop Through *chArray To Get Amount Of Bytes
	for(int iLength = 1; iLength < MAX_PATH; iLength++)
		if (chArray[iLength] == '*')	
			return iLength;	

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
} 

/* This Function Returns The Length Of External Int Arrays, SizeOf(Array) Fails For External Arrays. */
int ProcMem::iSizeOfArray(int *iArray){

	//Loop Through *chArray To Get Amount Of Bytes
	for(int iLength = 1; iLength < MAX_PATH; iLength++)
		if (iArray[iLength] == '*')	
			return iLength;	

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
} 

/* This Function Finds The Specified Value Inside Of Arrays And Returns A Boolean Value,
/* Used For Triggerbot To Find The Current Crosshair Entity i_NearEntity Inside The Enemy Array. */
bool ProcMem::iFind(int *iAry, int iVal){

	for(int i = 0; i < 64; i++)
		if(iVal == iAry[i] && iVal != 0)
			return true;

	return false;
}

#pragma endregion

#pragma region Memory Functions

/* This Function Will Return A Handle To The Process So We Can Write & Read Memeory From The Process. */
void ProcMem::Process(char* ProcessName){

	const WCHAR *procNameChar;
	int nChars = MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, NULL, 0);
	procNameChar = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, (LPWSTR)procNameChar, nChars);

	//Variables
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //Snapshot To View All Active Processes
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry); //Declare Structure Size And Populate It

	//Loop Through All Running Processes To Find Process
	do
		if (!wcscmp(ProcEntry.szExeFile, procNameChar))
		{
			//Store Process ID
			dwPID = ProcEntry.th32ProcessID;
			CloseHandle(hPID);

			//Give Our Handle All Access Rights 
	        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); 
			return;
		}
	while(Process32Next(hPID, &ProcEntry));

	//cout << "\nCouldn't find csgo.exe, make sure csgo.exe is running\n";
	//system("pause");
    //exit(0);
}

/* This Function Will Write Specified Bytes To The Address, And Can Also Be Reverted Back To Normal
/* Just Call It Again As It Works On A Boolean. */
void ProcMem::Patch(DWORD dwAddress, char *Patch_Bts, char *Default_Bts){
 
	//Variables
	int iSize = chSizeOfArray(Default_Bts);		

	//Loop Through Addresses Writing Bytes
	if (!bPOn)
		for(int i = 0; i < iSize; i++)	
			Write<BYTE>(dwAddress + i, Patch_Bts[i]);		
	else
		for(int i = 0; i < iSize; i++)	 	
			Write<BYTE>(dwAddress + i, Default_Bts[i]);        		        	

	bPOn = !bPOn; 
}

/* This Function Is Similiar To Cheat Engine's Code Injection Function, It's Able To Create JMP's 
/* To A Codecave And Write New Memory. Untested CALL Command */
void ProcMem::Inject(DWORD dwAddress, char *Inj_Bts, char *Def_Bts, BOOL Type){
	
	//Variables
	int i_ISize = chSizeOfArray(Inj_Bts);
	int i_DSize = chSizeOfArray(Def_Bts); 

	if(!bIOn)
	{		
		//NOP All Bytes In The Array Past The 5th Byte	
		if(i_DSize > 5)
			for (int i = 6; i < i_DSize; i++)
				Write<BYTE>(dwAddress + i, 0x90);	
		else {cout << "\nINJECTION: Default Bytes Must Be More Than 5\n"; return;}

		//Create Codecave
		dwCaveAddress = (DWORD)VirtualAllocEx(hProcess, NULL,  i_ISize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		//Calculate Jmp/Return Distances In Bytes To Write
		DWORD dwRetJmp = (dwAddress + i_DSize) - dwCaveAddress - 5; //(NextInstruction - CaveAddress - 5) - is correct equation.
		DWORD dwBaseJmp = dwCaveAddress - dwAddress - 5; //Base Jmp

		//Loop Through Each Address Writing Inj_Bts Inside The Codecave
		for (int i = 0; i <= i_ISize; i++)			
			Write<BYTE>(dwCaveAddress+i, Inj_Bts[i]);
		
		//Write The Return Distance In Bytes (E9 = Jmp | E8 = Call) To The Original Address
		Write<BYTE>(dwCaveAddress + i_ISize, Type ? 0xE9 : 0xE8);
		Write<DWORD>(dwCaveAddress + i_ISize + 1, dwRetJmp);
		
		//Write The Jump From The Original Address To The Codecave
		Write<BYTE>(dwAddress, Type ? 0xE9 : 0xE8);
		Write<DWORD>(dwAddress + 1, dwBaseJmp);

	}else{			
		//Restore Original Bytes
		for(int i = 0; i < i_DSize; i++)					
			Write<BYTE>(dwAddress + i, Def_Bts[i]);	

		//Clean Up! (DeAllocate CodeCave)
		VirtualFreeEx(hProcess, (LPVOID)dwCaveAddress, i_ISize + 5, MEM_DECOMMIT);
	}
    bIOn = !bIOn; 
} 

/* Basic Byte Scanner, Will Return The Start Address Of The Specififed Byte Pattern. 
/* To-Do: Re-Write Using Memory_Page Functions To Grab Blocks Of Memory And Scan
/* It Inside This Console, Maybe Study Multi-Threading For Faster Scanning. */
DWORD ProcMem::AOB_Scan(DWORD dwAddress, DWORD dwEnd, char *Bytes){

	//VARIABLES
	int iBytesToRead = 0, iTmp = 0;
	int length = chSizeOfArray(Bytes);
	bool bTmp = false;

	//Check If The Start Of The Array Has Wildcards, So We Can Change The Count
	if(Bytes[0] == '?')
	{
		for(; iBytesToRead < MAX_PATH; iBytesToRead++)
			if(Bytes[iBytesToRead] != '?')
			{
				iTmp = (iBytesToRead + 1);
				break;
			}
	}

	//Increase Start Address Till It Reaches The End Address While Reading Bytes
	for(;dwAddress < dwEnd; dwAddress++) 	
	{			
		if(iBytesToRead == length)
			return dwAddress - iBytesToRead;

		if(Read<BYTE>(dwAddress) == Bytes[iBytesToRead] || (bTmp && Bytes[iBytesToRead] == '?'))
		{
			iBytesToRead++;		
			bTmp = true;
		}
		else
		{
			iBytesToRead = iTmp;	
			bTmp = false;
		}
	}

	cout << "\nAOB_SCAN: Failed To Find Byte Pattern\n";
	return 0;
}

/* Returns The Base Address Of The Specified Module Inside The Target Process
/* e.g.[ Module("client.dll"); ]. */
DWORD ProcMem::Module(LPSTR ModuleName){
       
	const WCHAR *procNameChar;
	int nChars = MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, NULL, 0);
	procNameChar = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, (LPWSTR)procNameChar, nChars);


	//Variables
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); //Take A Module Snapshot Of The Process (Grab All Loaded Modules)
	MODULEENTRY32 mEntry; //Declare Module Entry Structure
	mEntry.dwSize = sizeof(mEntry); //Declare Structure Size And Populate It With Loaded Modules
	
	do
		std::cout << "finding module: " << *mEntry.szModule << std::endl;
	while (Module32Next(hModule, &mEntry));
	std::cout << "Done:: Next\n";
	//Scan For Module By Name
	do {
		std::cout << "finding module: " << *mEntry.szModule << std::endl;
		if (!wcscmp(mEntry.szModule, procNameChar))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	}
	while(Module32Next(hModule, &mEntry));
	
	cout << "\nCouldn't find client.dll, retrying...\n";
	while (true)
		Sleep(1000);
	return 0;
}

#pragma endregion