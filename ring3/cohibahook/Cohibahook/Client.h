#pragma once
extern Base::Utils::CVMTHookManager g_pClientVMT;
void __stdcall hkdCreateMove(int sequence_number, float input_sample_frametime, bool active);
void __stdcall hkdHudUpdate(bool active);