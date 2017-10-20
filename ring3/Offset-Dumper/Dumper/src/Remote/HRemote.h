#ifndef _HREMOTE_H_
#define _HREMOTE_H_

#pragma once

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

#include "..\Include\HWin.h"

#include <string>
#include <vector>
#include <unordered_map>

#pragma warning( disable : 4227 )

namespace Dumper
{
    namespace Remote
    {
        class CProcess;
        class CModule;

        class SignatureType_t
        {
        public:
            enum
            {
                NORMAL = 0x0, // normal
                READ = 0x1, // rpm at pattern
                SUBTRACT = 0x2, // subtract img base
            };
        };

        typedef std::vector<unsigned char>                      vecByte;
        typedef std::unordered_map<std::string, CModule*>       mapModule;

        class CMemory
        {
        public:

            CMemory( const uintptr_t& base, const size_t& size );
            ~CMemory( void );

            template <typename _Ty> _Ty             Get( const uintptr_t& off = 0x0 );
            const uintptr_t&                        Get( void ) const;

        protected:

            uintptr_t                               _base;                  // base
            vecByte                                 _bytes;                 // bytes
        };

        template <typename _Ty>
        _Ty CMemory::Get( const uintptr_t& off )
        {
            if( off < _bytes.size() ) {
                return *reinterpret_cast< _Ty* >( &_bytes.at( off ) );
            }
            return _Ty();
        }

        class CModule
        {
        public:

            CModule( const std::string& name, const std::string& path, const uintptr_t& imgsize, const intptr_t& imgbase );
            ~CModule( void );

            uintptr_t operator+( uintptr_t offset ) const;
            uintptr_t operator-( uintptr_t offset ) const;

            const std::string&                      GetName( void ) const;
            const std::string&                      GetPath( void ) const;
            const uintptr_t&                        GetImgSize( void ) const;
            const uintptr_t&                        GetImgBase( void ) const;
            const vecByte&                          GetDumpedBytes( void ) const;


        protected:

            std::string                             _name;                  // module name
            std::string                             _path;                  // module path

            uintptr_t                               _imgsize = 0;           // image size
            uintptr_t                               _imgbase = 0;           // image base

            vecByte                                 _bytes;                 // dumped byte of the module
        };

        class CProcess
        {
        public:

            bool                                    Attach( const std::string& procname,
                                                            const std::string& winname = std::string(),
                                                            const std::string& winclname = std::string(),
                                                            DWORD accessrights = PROCESS_ALL_ACCESS,
                                                            DWORD maxwtime = 0 );
            void                                    Detach( void );

            bool                                    ReadMemory( const uintptr_t& address, void* pBuffer, size_t size ) const;
            bool                                    WriteMemory( uintptr_t& address, const void* pBuffer, size_t size ) const;

            static bool                             CompareBytes( const unsigned char* bytes, const char* pattern );
            uintptr_t                               FindPattern( const std::string& module, const char* pattern, short type, uintptr_t patternOffset, uintptr_t addressOffset );

        private:

            bool                                    GetProcessID( void );
            bool                                    GetProcessHandle( void );
            bool                                    GetProcessModules( void );

        public:

            const mapModule&                        GetModules( void ) const;
            CModule*                                GetModuleByName( const std::string& name );

        protected:

            std::string                             _procname;              // process name
            std::string                             _winname;               // window name
            std::string                             _winclname;             // window class

            DWORD                                   _accessrights = 0;      // openprocess rights
            bool                                    _haswindow = false;     // has the process a window
            DWORD                                   _procid = 0;            // process id
            HANDLE                                  _hproc = nullptr;       // process handle

            mapModule                               _modules;               // unordered_map holds modules
        public:
            static CProcess* Singleton( void );
        };
    }
}

#ifndef pProcess
#define pProcess Dumper::Remote::CProcess::Singleton( )
#endif /* pProcess */

#pragma warning( default : 4227 )

#endif /* _HREMOTE_H_ */
