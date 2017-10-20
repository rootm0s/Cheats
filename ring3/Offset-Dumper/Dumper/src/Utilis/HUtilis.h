#ifndef _HUTILIS_H_
#define _HUTILIS_H_

#pragma once

#include "..\Include\HWin.h"

#include <time.h>

namespace Dumper
{
    namespace Utilis
    {
        inline const char* GetTime( )
        {
            time_t rawtime;
            struct tm* timeinfo;

            time( &rawtime );
            timeinfo = localtime( &rawtime );

            return asctime( timeinfo );
        }
    }
}

#endif /* _HUTILIS_H_ */
