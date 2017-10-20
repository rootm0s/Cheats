#ifndef __SOURCESDK_C_DEBUGOVERLAY_H__
#define __SOURCESDK_C_DEBUGOVERLAY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "main.h"
#include "VTable.h"
#include "Vector.h"

namespace ValveSDK
{
	class CDebugOverlay
	{
	private:
		enum VTableIndex
		{
			VTABLE_SCREENPOSITION = 13,
		};

	public:
		CDebugOverlay( void );
		~CDebugOverlay( void );
		int ScreenPosition(const Vector& vIn, Vector& vOut);
	};
}

extern ValveSDK::CDebugOverlay* g_pDebugOverlay;

#endif