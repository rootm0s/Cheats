#pragma once
#include "main.h"

bool direction = false;
bool isshooting = false;
Vector viewAngles;
Vector punchAngles;
int fakelag = 0;
bool airstuck;
void doAir() {
	if (GetAsyncKeyState(VK_XBUTTON1) & 1) {
		airstuck = !airstuck;
	}
}

/*
static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
__asm {
push    ebp
mov     ebp, esp
and     esp, 0FFFFFFF8h
sub     esp, 44h
push    ebx
push    esi
push    edi
mov     edi, cvar
mov     esi, value
jmp     pfn
}
}

HMODULE GetModuleHandleSafe(const char* pszModuleName)
{
HMODULE hmModuleHandle = NULL;

do
{
hmModuleHandle = GetModuleHandleA(pszModuleName);
Sleep(1);
} while (hmModuleHandle == NULL);

return hmModuleHandle;
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
static DWORD dwEngineBase = (DWORD)GetModuleHandleSafe("engine.dll");
static DWORD setaddr = Utils::FindPattern(dwEngineBase, 0x640000, (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
if (setaddr != 0) {
void* pvSetConVar = (char*)setaddr;
Invoke_NET_SetConVar(pvSetConVar, cvar, value);
}
}

std::string random[6] {
"-ISES.xyz",
"ISES.xyz-",
"ISES.xyz",
"M8, u got rekt by Ises.xyz",
"Why are you so mad? ISES.xyz",
"ISES.xyz rekt's all"
};

void change_name(const char* name)
{
if (pEngine->IsInGame() && pEngine->IsConnected())
NET_SetConVar(name, "name");
}

void ExecuteName() {
if (misc_active.getValve()) {
if (misc_fakelag.getValve()) {
change_name((char*)random[rand() % 6 + 1].c_str());
}
}
}

*/


void doMisc(CUserCmd* pCmd, C_BaseEntity* Local, Vector anglesToSet, bool misc_active, bool misc_bunnyhop, bool misc_autostrafe, bool misc_fakelag, int fakelagFactor, bool noRecoil, bool noSpread, bool forceNoSpread, bool misc_teleport, bool aim_activ) {
	Vector oldView = pCmd->viewangles;
	C_BaseCombatWeapon* gWeapon = Local->GetActive();
	if (misc_active) {
		if (misc_bunnyhop) {
			if (pCmd->buttons & IN_JUMP)
			{
				if (!(Local->Flags() & FL_ONGROUND))
				{
					pCmd->buttons &= ~IN_JUMP;
				}
			}
		}
		if (misc_autostrafe) {
			if ((GetAsyncKeyState(VK_SPACE) && !(Local->Flags() & FL_ONGROUND)))
			{
				if (direction)
				{
					pCmd->viewangles.y -= 10;
					pCmd->sidemove = -450.f; // Leak

					direction = false;
				}
				else
				{
					pCmd->viewangles.y += 10;
					pCmd->sidemove = 450.f;

					direction = true;
				}
			}
		}

		if (noRecoil && (pCmd->buttons & IN_ATTACK)) {

			if (!gWeapon->IsKnife()) {
				Vector pRecoil = Local->AimPunch() * 2.0; // Roll Angle.
				pCmd->viewangles.x -= pRecoil.x;
				pCmd->viewangles.y -= pRecoil.y;
				ClampAngles(pCmd->viewangles);
			}

		}

		if (misc_active && misc_fakelag_factor.getValue() > 0) {
			if (Local->GetLifeState() == 0) { // Player is alive.
				BYTE* SendPack = (BYTE*)((DWORD)pEngine + 0xF6887);
				if (fakelag < misc_fakelag_factor.getValue()) {
					*SendPack = false;
					fakelag++;
				}
				else {
					fakelag = 0;
				}
			}
		}


		if (forceNoSpread) {
			if (!gWeapon->IsKnife()) {
				*(int*)((DWORD)GetModuleHandleA("client.dll") + 0xA984A8) = 1;
			}
		}

		if (misc_teleport) {
			doAir();
			if (airstuck) {
				//pCmd->TickCount = INT_MAX;
				//pCmd->sidemove = 0;
				//pCmd->forwardmove = 0;
				pCvar->FindVar("setpos")->SetValue("0, 0, 0");
			}
		}
	}
}