#ifndef __CSPREAD_H__
#define __CSPREAD_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "DLLMain.h"


	class CSpread
	{
	public:
		//		void GetSpreadFix( IClientEntity* pLocal, UINT seed, Vec3& pflInAngles, Vec3& pflOutAngles );
		void GetSpreadFix(CBaseEntity* pLocal, UINT seed, Vector& pflInAngles);
	};

extern CSpread g_pSpread;

#endif
