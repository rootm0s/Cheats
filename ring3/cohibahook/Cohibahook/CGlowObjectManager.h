#ifndef __SOURCESDK_C_GLOWOBJECTMANAGER_H__
#define __SOURCESDK_C_GLOWOBJECTMANAGER_H__

#ifdef _MSC_VER
#pragma once
#endif

namespace ValveSDK
{
	class CGlowObjectManager
	{
	public:
		struct GlowObjectDefinition_t
		{
			CBaseEntity*	m_pBaseEntity;
			Vector			m_vecGlowColor;
			float			m_flGlowAlpha;
			UCHAR			m_Pad00[ 16 ];
			bool			m_bShouldGlow;
			bool			m_bUnknown;
			bool			m_bFullBloomRender;
			UCHAR			m_Pad01[ 13 ];
		};

		std::vector< GlowObjectDefinition_t >	m_vGlowObjectDefinitions;
		int										m_iFirstFreeSlot;

	public:
		CGlowObjectManager( void );
		~CGlowObjectManager( void );
	};
}

extern ValveSDK::CGlowObjectManager* g_pGlowObjectManager;

#endif