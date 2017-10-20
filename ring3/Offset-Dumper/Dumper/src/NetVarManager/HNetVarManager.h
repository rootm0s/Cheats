#ifndef _HNETVARMANAGER_H_
#define _HNETVARMANAGER_H_

#include "..\Include\HWin.h"
#include "..\Remote\HRemote.h"

#include <unordered_map>

#pragma warning( disable : 4227 )
#pragma warning( disable : 4172 )

namespace Dumper
{
    namespace NetVarManager
    {
        class PropType_t
        {
        public:
            enum
            {
                DPT_Int = 0,
                DPT_Float,
                DPT_Vector,
                DPT_VectorXY,
                DPT_String,
                DPT_Array,
                DPT_DataTable,
                DPT_NUMSendPropTypes
            };

            static std::string toString( int v, int e, int c )
            {
                switch( v ) {
                    case DPT_Int:
                        return "int";
                    case DPT_Float:
                        return "float";
                    case DPT_Vector:
                        return "Vec3";
                    case DPT_VectorXY:
                        return "Vec2";
                    case DPT_String:
                        return "char[ " + std::to_string(c) + " ]";
                    case DPT_Array:
                        return "[ " + std::to_string(e) + " ]";
                    case DPT_DataTable:
                        return "void*";
                    default:
                        return "";
                }
            }
        };

        class RecvTable : public Remote::CMemory
        {
        public:

            explicit RecvTable( const uintptr_t& base );
            ~RecvTable( void ) = default;

            std::string                             GetTableName( void );
            std::string                             GetClassNameA( void );
            uintptr_t                               GetPropById( int id );
            int                                     GetPropCount( void );
        };

        class RecvProp : public Remote::CMemory
        {
        public:

            RecvProp( const uintptr_t& base, int level, int offset );
            ~RecvProp( void ) = default;

            uintptr_t                               GetTable( void );
            std::string                             GetPropName( void );
            int                                     GetPropOffset( void );
            int                                     GetPropType( void );
            int                                     GetPropElements( void );
            int                                     GetPropStringBufferCount( void );
            const int&                              GetLevel( void ) const;

        protected:

            int                                     _level;                       // level
            int                                     _offset;                       // level
        };

        class ClientClass : public Remote::CMemory
        {
        public:

            explicit ClientClass( const uintptr_t& base );
            ~ClientClass( void ) = default;

            int                                     GetClassId( void );
            std::string                             GetClassNameA( void );
            uintptr_t                               GetNextClass( void );
            uintptr_t                               GetTable( void );
        };

        typedef std::unordered_map< std::string, std::vector<std::pair<std::string, RecvProp*>> >   mapTable;

        class CNetVarManager
        {
        public:

            bool                                    Load( void );
            void                                    Dump( void ) const;
            void                                    Release( void );

            int                                     GetNetVar( const std::string& tablename, const std::string& varname );

        private:

            void                                    ScanTable( RecvTable& table, int level, int offset, const char* name );

        protected:

            mapTable                                _tables;                    // recvtables dumped

        public:
            static CNetVarManager* Singleton( void );
        };
    }
}

#ifndef pNetVarManager
#define pNetVarManager Dumper::NetVarManager::CNetVarManager::Singleton( )
#endif

#pragma warning( default : 4172 )
#pragma warning( default : 4227 )

#endif /* _HNETVARMANAGER_H_ */
