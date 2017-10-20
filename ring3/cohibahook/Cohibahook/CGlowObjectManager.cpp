#include "main.h"

using namespace ValveSDK;

CGlowObjectManager::CGlowObjectManager( void )
{
	m_iFirstFreeSlot = 0;
	m_vGlowObjectDefinitions.clear();
}

CGlowObjectManager::~CGlowObjectManager( void )
{

}

ValveSDK::CGlowObjectManager* g_pGlowObjectManager = NULL;