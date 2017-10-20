#include "CDebugOverlay.h"

using namespace ValveSDK;

CDebugOverlay::CDebugOverlay( VOID )
{

}

CDebugOverlay::~CDebugOverlay( VOID )
{

}

int CDebugOverlay::ScreenPosition( const Vector& vecOrigin, Vector& vecScreen )
{
	typedef int( __thiscall* ScreenPosition_t )( PVOID, const Vector&, Vector& );
	return VTable::CallVirtualFunction< ScreenPosition_t >( this, VTABLE_SCREENPOSITION )( this, vecOrigin, vecScreen );
}

ValveSDK::CDebugOverlay* g_pDebugOverlay = NULL;