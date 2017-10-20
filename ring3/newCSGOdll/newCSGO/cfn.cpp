

#include "stdafx.h"


cfn_s cfn;

DWORD GetCLfn( PVOID Base, DWORD Index )
{
    PDWORD* VTablePointer = ( PDWORD* )Base;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[ Index ];
    return  dwAddress ;
}
void cfn_init()
{
	cfn.WorldToScreenMatrix = (fWorldToScreenMatrix)GetCLfn(pEngine,37);
	
	add_log("cfn.WorldToScreenMatrix = %x",cfn.WorldToScreenMatrix);
}

#include "my_trace.h"

float IsVisible( Vector  vLoc, Vector pEnt )
        {
 //           IClientEntity* pLocalEntity = g_pClientEntityList->GetClientEntity( g_pEngine->GetLocalPlayer() );
            IClientEntity* pLocalEntity = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
           if( !pLocalEntity )
                return 0.00001f;

            trace_t tr;
            Ray_t ray;
			Vector eyePos;
			eyePos.X=vLoc.X;eyePos.Y=vLoc.Y;eyePos.Z=vLoc.Z+90.0f;

			Vector ent;
			ent = pEnt;
			ent.Z += 30.0f;

            CTraceFilter traceFilter( pLocalEntity );

            ray.Init( eyePos , ent);

  //          g_pTrace->TraceRay( ray, 0x46004003, &traceFilter, &tr );
  //          g_pTrace->TraceRay( ray, 0x46004003, 0, &tr );
//			pEngineTrace->TraceRay(ray, 0x46004003, 0, &tr);
			pEngineTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);

            if( tr.allsolid || tr.startsolid )  return 0.00001f;              
//				return FALSE;
	//		if(tr.m_pEnt) return 1.0f;
            //if( pEnt && tr.m_pEnt )
            //{
              //  IClientEntity *pTraceEnt = g_pClientEntityList->GetClientEntity( tr.m_pEnt->Index() );
            //    return( pTraceEnt == pEnt || tr.fraction >= 1.0f );
            //}
			//add_log("tr.fraction = %f",tr.fraction);
            //return( tr.fraction >= 1.0f );
            return  tr.fraction;
        }
