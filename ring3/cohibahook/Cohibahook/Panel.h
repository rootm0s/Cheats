#pragma once

extern Base::Utils::CVMTHookManager g_pPanelVMT;
void __stdcall hkdPaintTraverse(unsigned vguiPanel, bool forceRepaint, bool allowForce);