#include "HNetVarManager.h"
#include "../Utilis/HUtilis.h"

#include <iomanip>
#include <sstream>
#include <fstream>

namespace Dumper
{
    namespace NetVarManager
    {
        RecvTable::RecvTable( const uintptr_t& base ) :
            CMemory( base, 0x10 )
        {
        }

        uintptr_t RecvTable::GetPropById( int id )
        {
            return Get<uintptr_t>() + id * 0x3C;
        }

        std::string RecvTable::GetTableName( void )
        {
            auto toReturn = std::string( "", 32 );
            pProcess->ReadMemory( Get<DWORD>( 0xC ), &toReturn.at( 0 ), 32 );
            return toReturn;
        }

        std::string RecvTable::GetClassNameA( void )
        {
            auto toReturn = GetTableName();
            toReturn.replace( toReturn.begin(), toReturn.begin() + 3, "C" );
            return toReturn;
        }

        int RecvTable::GetPropCount( void )
        {
            return Get<int>( 0x4 );
        }

        RecvProp::RecvProp( const uintptr_t& base, int level, int offset ) :
            CMemory( base, 0x3C ),
            _level( level ),
            _offset( offset )
        {
        }

        uintptr_t RecvProp::GetTable( void )
        {
            return Get<uintptr_t>( 0x28 );
        }

        std::string RecvProp::GetPropName( void )
        {
            auto toReturn = std::string( "", 64 );
            pProcess->ReadMemory( Get<DWORD>(), &toReturn.at( 0 ), 64 );
            return toReturn;
        }

        int RecvProp::GetPropOffset( void )
        {
            return _offset + Get<int>( 0x2C );
        }

        int RecvProp::GetPropType( void )
        {
            return Get<int>( 0x4 );
        }

        int RecvProp::GetPropElements( void )
        {
            return Get<int>( 0x34 );
        }

        int RecvProp::GetPropStringBufferCount()
        {
            return Get<int>( 0xC );
        }

        const int& RecvProp::GetLevel( void ) const
        {
            return _level;
        }

        ClientClass::ClientClass( const uintptr_t& base ) :
            CMemory( base, 0x28 )
        {
        }

        int ClientClass::GetClassId( void )
        {
            return Get<int>( 0x14 );
        }

        std::string ClientClass::GetClassNameA( void )
        {
            auto toReturn = std::string( "", 64 );
            pProcess->ReadMemory( Get<DWORD>( 0x8 ), &toReturn.at( 0 ), 64 );
            return toReturn;
        }

        uintptr_t ClientClass::GetNextClass( void )
        {
            return Get<uintptr_t>( 0x10 );
        }

        uintptr_t ClientClass::GetTable( void )
        {
            return Get<uintptr_t>( 0xC );
        }

        bool CNetVarManager::Load( void )
        {
            auto firstclass = pProcess->FindPattern( "client.dll", "44 54 5F 54 45 57 6F 72 6C 64 44 65 63 61 6C", 0, 0, 0 );

            std::stringstream ss;
            for( auto i = 0; i < 4; ++i ) {
                ss << std::hex << std::setw(2) << std::setfill('0') << ( ( firstclass >> 8 * i ) & 0xFF ) << " ";
            }

            firstclass = pProcess->FindPattern( "client.dll", ss.str().c_str(), Remote::SignatureType_t::READ, 0x2B, 0 );

            if( !firstclass )
                return false;

            for( auto Class = ClientClass( firstclass ); Class.Get(); Class = ClientClass( Class.GetNextClass() ) ) {

                auto table = RecvTable( Class.GetTable() );
                if( !table.Get() )
                    continue;

                ScanTable( table, 0, 0, table.GetTableName().c_str() );
            }
            return true;
        }

        void CNetVarManager::Dump( void ) const
        {
            std::stringstream ss;
            ss << "- - - - - - Tool by rootm0s ( uc ) - - - - - - " << std::endl;
            ss << "| -> https://github.com/rootm0s" << std::endl;
            ss << "| -> " << Utilis::GetTime();
            ss << "- -" << std::endl << std::endl;

            for( auto& table : _tables ) {

                auto first = table.second[0].first[0] == 'D';
                ss << table.first;

                for( auto& prop : table.second ) {

                    if( prop.first[ 0 ] == 'D' ) {
                        ss << (first ? " : public " : ", ") << prop.first.c_str();
                        first = false;
                        continue;
                    }
                    else if( !first ) {
                        ss << std::endl;
                        first = true;
                    }

                    ss << std::setw( 53 )
                        << std::setfill( '_' )
                        << std::left
                        << ( std::string( prop.second->GetLevel(), ' ' ) + "|__" + prop.first ).c_str()
                        << std::right
                        << std::hex
                        << " -> 0x"
                        << std::setw( 4 )
                        << std::setfill( '0' )
                        << std::uppercase
                        << prop.second->GetPropOffset()
                        << " ( " + PropType_t::toString( prop.second->GetPropType(),
                                                        prop.second->GetPropElements(),
                                                        prop.second->GetPropStringBufferCount() ) + " )"
                        << std::endl;
                }
            }

            std::ofstream( "NetVarManager.txt" ) << ss.str();
        }

        void CNetVarManager::Release( void )
        {
            for( auto& table : _tables ) {
                for( auto& prop : table.second ) {
                    delete prop.second;
                }
                table.second.clear();
            }
            _tables.clear();
        }

        int CNetVarManager::GetNetVar( const std::string& tablename, const std::string& varname )
        {
            auto table = _tables.find( tablename );
            if( table != _tables.end() ) {
                for( auto& prop : table->second ) {
                    if( prop.first == varname )
                        return prop.second->GetPropOffset();
                }
            }
            return 0;
        }

        void CNetVarManager::ScanTable( RecvTable& table, int level, int offset, const char* name )
        {
            auto count = table.GetPropCount();
            for( auto i = 0; i < count; ++i ) {

                auto prop = new RecvProp( table.GetPropById( i ), level, offset );
                auto propName = prop->GetPropName();

                if( isdigit( propName[ 0 ] ) )
                    continue;

                auto isBaseClass = !strcmp( propName.c_str(), "baseclass" );
                if( !isBaseClass ) {
                    _tables[ name ].push_back( {
                        propName.c_str(), prop
                    } );
                }

                auto child = prop->GetTable();
                if( !child )
                    continue;

                auto recvTable = RecvTable( child );

                if( isBaseClass ) {
                    _tables[ name ].push_back( {
                        recvTable.GetTableName(), prop
                    } );
                    --level;
                }

                ScanTable( recvTable, ++level, prop->GetPropOffset(), name );
            }
        }

        CNetVarManager* CNetVarManager::Singleton( void )
        {
            static auto g_pNetVarManager = new CNetVarManager();
            return g_pNetVarManager;
        }
    }
}