#include "HMain.h"

int main( void )
{
    if( !pProcess->Attach( "csgo.exe" ) )
        return -1;

    if( !pNetVarManager->Load( ) )
        return -2;

    pNetVarManager->Dump( );
    pOffsetManager->Dump( );

    delete pOffsetManager;

    pNetVarManager->Release( );
    delete pNetVarManager;

    pProcess->Detach( );
    delete pProcess;
}