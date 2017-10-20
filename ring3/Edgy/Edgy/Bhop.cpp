#include "Bhop.h"

void Bhop::Tick(CGeneralEntity e) {
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (e.Flags & (1 << 0)) {
			WriteProcessMemory(b.hProcess, (LPVOID)(I.ClientDLL + m_dwForceJump), &Jump, sizeof(BYTE), NULL);
			//WriteProcessMemory(b.hProcess, (LPVOID)(I.ClientDLL + m_dwForceJump), &Jump, sizeof(BYTE), NULL);
		}
	}
	//Sleep(10);
}