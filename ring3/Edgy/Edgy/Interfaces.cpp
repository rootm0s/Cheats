#include "Interfaces.h"
#include "Offsets.h"
#include "ProcMem.h"

Interfaces I;
bool CInterfaces::UpdateInterfaces() {
	b.Process("csgo.exe");
	if (b.Module("client.dll") == 0) {
		Sleep(300);
		return false;
	}
	if (b.Module("engine.dll") == 0) {
		Sleep(300);
		return false;
	}
	I.ClientDLL = b.Module("client.dll");
	I.EngineDLL = b.Module("engine.dll");
	ReadProcessMemory(b.hProcess, (LPVOID)(I.EngineDLL + m_dwClientState), &I.EnginePointer, sizeof(DWORD), 0);
	//I.EnginePointer = b.Read<DWORD>(I.EngineDLL + m_dwClientState);
	return true;
}