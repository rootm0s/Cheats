#pragma once
#include "main.h"
#define IsNullPtr( ptr )							( ptr == nullptr )
/************************************************************************/
/* Interface: EngineTraceClient, Version: 004                           */
/************************************************************************/

//EngineTraceClient* g_pEngineTraceClient = nullptr;

namespace ValveSDK
{
	class EngineTraceClient
	{
	public:
		enum VTableIndex
		{
			VTABLE_CLIPRAYTOENTITY = 3,
			VTABLE_TRACERAY = 5,
		};
		/************************************************************************/
		/* TraceType defines                                                    */
		/************************************************************************/
		enum TraceType_t
		{
			TRACE_EVERYTHING = 0,
			TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
			TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
			TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
		};
		/************************************************************************/
		/* Structure: cplane_t                                                  */
		/************************************************************************/
		struct cplane_t
		{
			//float normal[3];
			Vector normal;
			float dist;
			BYTE type;
			BYTE signBits;
			BYTE pad[2];
		};
		/************************************************************************/
		/* Structure: csurface_t                                                */
		/************************************************************************/
		struct csurface_t
		{
			const char*		name;
			short			surfaceProps;
			unsigned short	flags;
		};
		/************************************************************************/
		/* Structure: Ray_t                                                     */
		/************************************************************************/
		struct Ray_t
		{
			VectorAligned		m_Start;
			VectorAligned		m_Delta;
			VectorAligned		m_StartOffset;
			VectorAligned		m_Extents;
			const matrix3x4*	m_pWorldAxisTransform;
			bool					m_IsRay;
			bool					m_IsSwept;

			Ray_t() : m_pWorldAxisTransform(NULL)
			{

			}

			void Init(Vector const& start, Vector const& end)
			{
				Assert(&end);
				VectorSubtract(end, start, m_Delta);

				m_IsSwept = (m_Delta.Length() != 0);

				VectorClear(m_Extents);
				m_pWorldAxisTransform = NULL;
				m_IsRay = true;

				VectorClear(m_StartOffset);
				VectorCopy(start, m_Start);
			}

			void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
			{
				Assert(&end);
				VectorSubtract(end, start, m_Delta);

				m_pWorldAxisTransform = NULL;
				m_IsSwept = (m_Delta.Length() != 0);

				VectorSubtract(maxs, mins, m_Extents);
				m_Extents *= 0.5f;
				m_IsRay = (m_Extents.Length() < 1e-6);

				VectorAdd(mins, maxs, m_StartOffset);
				m_StartOffset *= 0.5f;
				VectorAdd(start, m_StartOffset, m_Start);
				m_StartOffset *= -1.0f;
			}

			Vector InvDelta() const
			{
				Vector vecInvDelta;
				for (int iAxis = 0; iAxis < 3; ++iAxis)
				{
					if (m_Delta[iAxis] != 0.0f)
						vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
					else vecInvDelta[iAxis] = FLT_MAX;

				}
				return vecInvDelta;
			}
		};
		/************************************************************************/
		/* Structure: trace_t                                                   */
		/************************************************************************/
		struct trace_t
		{
			Vector		start;
			Vector		endpos;
			cplane_t			plane;
			float				fraction;
			int					contents;
			WORD				dispFlags;
			bool				allSolid;
			bool				startSolid;
			float				fractionLeftSolid;
			csurface_t			surface;
			int					hitgroup;
			short				physicsBone;
			WORD				m_nWorldSurfaceIndex;
			CBaseEntity*	m_pEnt;
			int					hitbox;
		};
		/************************************************************************/
		/* Interface: ITraceFilter                                              */
		/************************************************************************/
		class ITraceFilter
		{
		public:
			virtual bool ShouldHitEntity(CBaseEntity* pClientBaseEntityToHandle, int contentsMask) = 0;
			virtual TraceType_t	GetTraceType() const = 0;
		};
		/************************************************************************/
		/* TraceFilter: Skip entity                                             */
		/************************************************************************/
		class CTraceFilter : public ITraceFilter
		{
		public:
			CTraceFilter(PVOID pSkip)
			{
				this->pSkip = pSkip;
			}

			bool ShouldHitEntity(CBaseEntity* pClientBaseEntityToHandle, int contentsMask)
			{
				return !(pClientBaseEntityToHandle == pSkip);
			}

			virtual TraceType_t GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}

			PVOID pSkip;
		};

		/************************************************************************/
		/* TraceFilter: Skip two entities                                       */
		/************************************************************************/
		class CTraceFilterSkipTwoEntities : public ITraceFilter
		{
		public:
			CTraceFilterSkipTwoEntities(PVOID pLocalEntity, PVOID pLastEntity)
			{
				m_pLocalEntity = pLocalEntity;
				m_pLastEntity = pLastEntity;
			}

			virtual bool ShouldHitEntity(CBaseEntity* pClientBaseEntity, int contentsMask)
			{
				return !(pClientBaseEntity == m_pLocalEntity || pClientBaseEntity == m_pLastEntity);
			}

			virtual TraceType_t    GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}

		public:
			PVOID m_pLocalEntity;
			PVOID m_pLastEntity;
		};
		/************************************************************************/
		/* Virtual Function: ClipRayToEntity, Index: 3                          */
		/************************************************************************/
		void ClipRayToEntity(const Ray_t& ray, UINT iMask, CBaseEntity* pBaseEntity, trace_t* pTrace)
		{
			typedef void(__thiscall* ClipRayToEntity_t)(PVOID, const Ray_t&, UINT, CBaseEntity*, trace_t*);
			getvfunc<ClipRayToEntity_t>(this, VTABLE_CLIPRAYTOENTITY)(this, ray, iMask, pBaseEntity, pTrace);
		}
		/************************************************************************/
		/* Virtual Function: TraceRay, Index: 5                                 */
		/************************************************************************/
		void TraceRay(const Ray_t& ray, UINT iMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
		{
			typedef void(__thiscall* TraceRay_t)(PVOID, const Ray_t&, UINT, ITraceFilter*, trace_t*);
			//VTable::CallVirtualFunction< TraceRay_t >(this, VTABLE_TRACERAY)(this, ray, iMask, pTraceFilter, pTrace);
			getvfunc<TraceRay_t>(this, VTABLE_TRACERAY)(this, ray, iMask, pTraceFilter, pTrace);
		}
		/************************************************************************/
		/* Own Function: IsVisible                                              */
		/************************************************************************/
		bool IsVisible(CBaseEntity* pLocalClientBaseEntity, Vector vecOrigin, Vector vecFinal, CBaseEntity* pClientBaseEntity)
		{
			Ray_t ray;
			ray.Init(vecOrigin, vecFinal);

			CTraceFilter TraceFilter(pLocalClientBaseEntity);

			trace_t trace;
			TraceRay(ray, 0x4600400B, &TraceFilter, &trace);

			return(trace.m_pEnt == pClientBaseEntity || trace.fraction > 0.99f);
		}
		/************************************************************************/
		/* Own Function: IsVisible                                              */
		/************************************************************************/
		bool IsVisible2(CBaseEntity* pLocalClientBaseEntity, CBaseEntity* pClientBaseEntity, Vector vecOrigin, Vector vecFinal)
		{
			Ray_t ray;
			ray.Init(vecOrigin, vecFinal);

			CTraceFilterSkipTwoEntities TraceFilter(pLocalClientBaseEntity, pClientBaseEntity);

			trace_t trace;
			TraceRay(ray, 0x4600400B, (ITraceFilter*)&TraceFilter, &trace);

			return(!IsNullPtr(trace.m_pEnt) || trace.fraction == 1.f);
		}
		//SETUP BONES
		bool SetupBones(matrix3x4* pBoneToWorldOut, int iMaxBones, int iBoneMask, float flCurrentTime)
		{
			typedef bool(__thiscall* SetupBones_t)(PVOID, matrix3x4*, int, int, float);

			PVOID pClientRenderable = (PVOID)(this + 4);
			return getvfunc<SetupBones_t>(this, 4)(pClientRenderable, pBoneToWorldOut, iMaxBones, iBoneMask, flCurrentTime);
		}
		//Virtual Function: GetStudioModel, Index: 30  
		studiohdr_t* GetStudioModel(const model_t* pModel)
		{
			typedef studiohdr_t*(__thiscall* GetStudioModel_t)(PVOID, const model_t*);
			return getvfunc<GetStudioModel_t>(this, 30)(this, pModel);
		}
		//Virtual Function: GetModel, Index: 8 - returns model_t pointer 
		model_t* GetModel(void)
		{
			typedef model_t*(__thiscall* GetModel_t)(PVOID);
			PVOID pClientRenderable = (PVOID)(this + 4);

			//return Memory::VTable::CallVirtualFunction< GetModel_t >(pClientRenderable, VTABLE_GETMODEL)(pClientRenderable);
			return getvfunc<GetModel_t>(pClientRenderable, 8)(pClientRenderable);
		}
		//pWall ergänzung
		class IHandleEntity
		{
		public:
			virtual ~IHandleEntity() {}
			virtual void Func01() = 0;
			virtual void Func02() = 0;
		};
		class IClientUnknown : public IHandleEntity
		{
		public:
			virtual PVOID GetCollideable() = 0;
			virtual PVOID GetClientNetworkable() = 0;
			virtual PVOID GetClientRenderable() = 0;
			virtual PVOID GetIClientEntity() = 0;
			virtual CBaseEntity *GetBaseEntity() = 0;
			virtual PVOID GetClientThinkable() = 0;
			virtual void *class1(void) = 0;
			virtual void *class2(void) = 0;
		};
		/*
		class CTraceFilterNoPlayer : public CTraceFilter
		{
		public:
			CTraceFilterNoPlayer() {}
			virtual bool ShouldHitEntity(IHandleEntity *pServerEntity, int contentsMask)
			{
				IClientUnknown *pUnk = (IClientUnknown*)pServerEntity;
				CBaseEntity *pEnt = pUnk->GetBaseEntity();
				return !pEnt->IsPlayer();
			}
		};
		*/
	};
}

//extern ValveSDK::EngineTraceClient* g_pEngineTraceClient;