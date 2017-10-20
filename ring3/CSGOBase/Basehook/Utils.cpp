#include "DllMain.h"

/*
static void sendMouseClick( int click, int release )
{
#ifdef WIN32
// Set up the input event struct
INPUT input_ev[1];

input_ev[0].type = INPUT_MOUSE;

PMOUSEINPUT mouse_evp;
mouse_evp = &input_ev[0].mi;

mouse_evp->dx = 0;
mouse_evp->dy = 0;
mouse_evp->time=0;

if( click )
{
if( click == 1 )
{
mouse_evp->dwFlags = MOUSEEVENTF_LEFTDOWN;
}
else if( click == 2 )
{
mouse_evp->dwFlags = MOUSEEVENTF_RIGHTDOWN;
}
else if( click == 3 )
{
mouse_evp->dwFlags = MOUSEEVENTF_MIDDLEDOWN;
}

SendInput( 1, input_ev, sizeof( INPUT ) );
}

if( release )
{
if( release == 1 )
{
mouse_evp->dwFlags = MOUSEEVENTF_LEFTUP;
}
else if( release == 2 )
{
mouse_evp->dwFlags = MOUSEEVENTF_RIGHTUP;
}
else if( release == 3 )
{
mouse_evp->dwFlags = MOUSEEVENTF_MIDDLEUP;
}

SendInput( 1, input_ev, sizeof( INPUT ) );
}
#endif
}

static void sendKeyState( char key, int press, int release )
{
#ifdef WIN32
// Set up the input event struct
INPUT input_ev[1];

input_ev[0].type = INPUT_KEYBOARD;

PKEYBDINPUT key_evp;
key_evp = &input_ev[0].ki;

key_evp->wScan = key;
key_evp->time = 0;

unsigned short extended = 0;

if( key & 0x80 ) {
extended = KEYEVENTF_EXTENDEDKEY;
key_evp->wScan =  key & ~0xff80;
}

if( press ) {
key_evp->dwFlags = extended | KEYEVENTF_SCANCODE;

SendInput( 1, input_ev, sizeof( INPUT ) );
}

if( release ) {
key_evp->dwFlags = extended | KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

SendInput( 1, input_ev, sizeof( INPUT ) );
}
#endif
}

static void sendAbsoluteMouseMove( float x, float y )
{
#ifdef WIN32
// Set up the input event struct
INPUT input_ev[1];

input_ev[0].type = INPUT_MOUSE;

PMOUSEINPUT mouse_evp;
mouse_evp = &input_ev[0].mi;

mouse_evp->time=0;

mouse_evp->dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | 0x2000;

mouse_evp->dx = (int)(65535.0f*x);
mouse_evp->dy = 65535-(int)(65535.0f*y);

SendInput( 1, input_ev, sizeof( INPUT ) );
#endif
}*/

namespace Base
{
	namespace Utils
	{
		ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask,
			ULONG uCodeBase, ULONG uSizeOfCode )
		{
			BOOL bPatternDidMatch = FALSE;
			HMODULE hModule = GetModuleHandle ( sModuleName.c_str( ) );

			if ( !hModule )
				return 0x0;

			PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER ( hModule );
			PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS ( LONG ( hModule ) + pDsHeader->e_lfanew );
			PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

			if ( uCodeBase == 0x0 )
				uCodeBase = ( ULONG ) hModule + pOptionalHeader->BaseOfCode;

			if ( uSizeOfCode == 0x0 )
				uSizeOfCode = pOptionalHeader->SizeOfCode;

			ULONG uArraySize = sMask.length( );

			if ( !uCodeBase || !uSizeOfCode || !uArraySize )
				return 0x0;

			for ( size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++ )
			{
				for ( size_t t = 0; t < uArraySize; t++ )
				{
					if ( *( ( PBYTE ) i + t ) == pbPattern [ t ] || sMask.c_str( ) [ t ] == '?' )
						bPatternDidMatch = TRUE;

					else
					{
						bPatternDidMatch = FALSE;
						break;
					}
				}

				if ( bPatternDidMatch )
					return i;
			}

			return 0x0;
		} 

		HMODULE GetModuleHandleSafe( const char* pszModuleName )
		{
			HMODULE hmModuleHandle = NULL;

			do
			{
				hmModuleHandle = GetModuleHandle( pszModuleName );
				Sleep( 1 );
			}
			while(hmModuleHandle == NULL);

			return hmModuleHandle;
		}

	}

	namespace Debug
	{
		bool AttachDebugConsole( void )
		{
			FILE* g_pConStream;

			if ( !AttachConsole( ATTACH_PARENT_PROCESS ) )
			{
				if ( !AllocConsole() )
					return false;
			}

			if ( !SetConsoleTitle( "DebugMessages" ) )
				return false;

			errno_t err = freopen_s( &g_pConStream, "CONOUT$", "w", stdout );

			if ( err != 0 )
				return false;

			return true;
		}
		void LOG(const char* output)
		{
			printf(output);
			printf("\n");
		}
	}
}