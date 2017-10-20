#ifndef _HOFFSETMANAGER_H_
#define _HOFFSETMANAGER_H_

#include "..\Include\HWin.h"
#include "..\Remote\HRemote.h"

namespace Dumper
{
    namespace OffsetManager
    {
        class COffsetManager
        {
        public:

            static void                             Dump( void );

        private:

            static void                             DumpNetVar( const std::string& tablename, const std::string& varname, uintptr_t offset, std::stringstream& ss );
            static void                             DumpPatternOffset( const std::string& tablename, const std::string& varname, const std::string& module, const char* pattern, int type, uintptr_t pattern_offset, uintptr_t address_offset, std::stringstream& ss );
            static void                             LogToStringStream( const std::string& tablename, const std::string& varname, uintptr_t offset, std::stringstream& ss );

        public:
            static COffsetManager* Singleton( void );
        };
    }
}

#ifndef pOffsetManager
#define pOffsetManager Dumper::OffsetManager::COffsetManager::Singleton( )
#endif

#endif /* _HOFFSETMANAGER_H_ */
