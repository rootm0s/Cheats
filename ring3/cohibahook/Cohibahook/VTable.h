#ifndef __MEMORY_VTABLE_H__
#define __MEMORY_VTABLE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <Windows.h>

namespace VTable
{
	template< typename VirtualFunction > VirtualFunction CallVirtualFunction( PVOID lpBase, DWORD dwIndex )
	{
		PDWORD* ppdwVTablePointer = ( PDWORD* )lpBase;
		PDWORD pdwVTableFuntion = *ppdwVTablePointer;

		return ( VirtualFunction )( pdwVTableFuntion[ dwIndex ] );
	}
}

#endif