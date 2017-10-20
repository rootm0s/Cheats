namespace ValveSDK
{
	struct surfacephysicsparams_t
	{
		// vphysics physical properties
		float			friction;
		float			elasticity;				// collision elasticity - used to compute coefficient of restitution
		float			density;				// physical density (in kg / m^3)
		float			thickness;				// material thickness if not solid (sheet materials) in inches
		float			dampening;
	};

	struct surfaceaudioparams_t
	{
		// sounds / audio data
		float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
		float			hardnessFactor;	// like elasticity, but only affects impact sound choices
		float			roughnessFactor;	// like friction, but only affects scrape sound choices

		// audio thresholds
		float			roughThreshold;	// surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
		float			hardThreshold;	// surface hardness > this causes "hard" impacts, < this causes "soft" impacts
		float			hardVelocityThreshold;	// collision velocity > this causes "hard" impacts, < this causes "soft" impacts
		// NOTE: Hard impacts must meet both hardnessFactor AND velocity thresholds
	};

	struct surfacesoundnames_t
	{
		unsigned short	stepleft;
		unsigned short	stepright;

		unsigned short	impactSoft;
		unsigned short	impactHard;

		unsigned short	scrapeSmooth;
		unsigned short	scrapeRough;

		unsigned short	bulletImpact;
		unsigned short	rolling;

		unsigned short	breakSound;
		unsigned short	strainSound;
	};

	struct surfacesoundhandles_t
	{
		short	stepleft;
		short	stepright;

		short	impactSoft;
		short	impactHard;

		short	scrapeSmooth;
		short	scrapeRough;

		short	bulletImpact;
		short	rolling;

		short	breakSound;
		short	strainSound;
	};

	struct surfacegameprops_t
	{
		// game movement data
		float			maxSpeedFactor;			// Modulates player max speed when walking on this surface
		float			jumpFactor;				// Indicates how much higher the player should jump when on the surface
		//float padding;
		//float pad2;
		// Game-specific data
		unsigned short	material;

		float padding;
		float pad2;

		unsigned short realmaterial;

		// Indicates whether or not the player is on a ladder.
		unsigned char	climbable;
		unsigned char	pad;
	};

	struct surfacedata_t
	{
		surfacephysicsparams_t	physics;	// physics parameters
		surfaceaudioparams_t	audio;		// audio parameters
		surfacesoundnames_t		sounds;		// names of linked sounds
		surfacegameprops_t		game;		// Game data / properties

		surfacesoundhandles_t		soundhandles;
	};

	class IPhysicsSurfaceProps
	{
	public:
		surfacedata_t *GetSurfaceData(int surfaceDataIndex)
		{
			typedef surfacedata_t*(__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 5)(this, surfaceDataIndex);
		}
	};

	class CTrace
	{
	public:
		enum TraceType_t
		{
			TRACE_EVERYTHING = 0,
			TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
			TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
			TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
		};

		struct cplane_t
		{
			Vector normal;
			float dist;
			BYTE type;
			BYTE signBits;
			BYTE pad[2];
		};

		struct csurface_t
		{
			const char*		name;
			short			surfaceProps;
			unsigned short	flags;
		};

		struct Ray_t
		{
			VectorAligned  m_Start;	// starting point, centered within the extents
			VectorAligned  m_Delta;	// direction + length of the ray
			VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
			VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
			const matrix3x4 *m_pWorldAxisTransform;
			bool	m_IsRay;	// are the extents zero?
			bool	m_IsSwept;	// is delta != 0?

			Ray_t() : m_pWorldAxisTransform(NULL)	{}

			void Init(Vector const& start, Vector const& end)
			{
				Assert(&end);
				VectorSubtract(end, start, m_Delta);

				m_IsSwept = (m_Delta.LengthSqr() != 0);

				VectorClear(m_Extents);
				m_pWorldAxisTransform = NULL;
				m_IsRay = true;

				// Offset m_Start to be in the center of the box...
				VectorClear(m_StartOffset);
				VectorCopy(start, m_Start);
			}

			void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
			{
				Assert(&end);
				VectorSubtract(end, start, m_Delta);

				m_pWorldAxisTransform = NULL;
				m_IsSwept = (m_Delta.LengthSqr() != 0);

				VectorSubtract(maxs, mins, m_Extents);
				m_Extents *= 0.5f;
				m_IsRay = (m_Extents.LengthSqr() < 1e-6);

				// Offset m_Start to be in the center of the box...
				VectorAdd(mins, maxs, m_StartOffset);
				m_StartOffset *= 0.5f;
				VectorAdd(start, m_StartOffset, m_Start);
				m_StartOffset *= -1.0f;
			}

			// compute inverse delta
			Vector InvDelta() const
			{
				Vector vecInvDelta;
				for (int iAxis = 0; iAxis < 3; ++iAxis)
				{
					if (m_Delta[iAxis] != 0.0f)
					{
						vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
					}
					else
					{
						vecInvDelta[iAxis] = FLT_MAX;
					}
				}
				return vecInvDelta;
			}

		private:
		};

		struct trace_t
		{
			Vector	start;
			Vector	endpos;
			cplane_t	plane;
			float		fraction;
			int			contents;
			WORD		dispFlags;
			bool		allSolid;
			bool		startSolid;
			float		fractionLeftSolid;
			csurface_t	surface;
			int			hitgroup;
			short		physicsBone;
			WORD		m_nWorldSurfaceIndex;
			CBaseEntity* m_pEnt;
			int			hitbox;
		};

		class IHandleEntity
		{
		public:
			virtual ~IHandleEntity() {}
			virtual void Func01() = 0;
			virtual void Func02() = 0;
		};

		class ITraceFilter
		{
		public:
			virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask) = 0;
			virtual TraceType_t	GetTraceType() const = 0;
		};

		class CTraceFilter : public ITraceFilter
		{
		public:
			virtual TraceType_t	GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}
		};

		class CSimpleTraceFilter : public ITraceFilter
		{
		public:
			CSimpleTraceFilter(PVOID pEnt)
			{
				m_pPassEnt = pEnt;
			}

			virtual bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask)
			{
				return pHandleEntity != m_pPassEnt;
			}

			virtual TraceType_t	GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}

			PVOID m_pPassEnt;
		};

		class CTraceFilterSkipTwoEnts : public ITraceFilter
		{
		public:
			CTraceFilterSkipTwoEnts(PVOID pEnt, PVOID pEnt2)
			{
				m_pPassEnt = pEnt;
				m_pPassEnt2 = pEnt2;
			}

			virtual bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask)
			{
				return (pHandleEntity != m_pPassEnt && pHandleEntity != m_pPassEnt2);
			}

			virtual TraceType_t	GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}

			PVOID m_pPassEnt;
			PVOID m_pPassEnt2;
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

		int GetPointContents(const Vector &vecAbsPosition, unsigned int fMask, IHandleEntity** ppEntity = NULL)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, const Vector &, unsigned int, IHandleEntity**);
			return getvfunc<OriginalFn>(this, 0)(this, vecAbsPosition, fMask, ppEntity);
		}

		void TraceRay(const Ray_t &ray, unsigned int flMask, ITraceFilter *pTraceFilter, trace_t *ptrace)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const Ray_t &, unsigned int, ITraceFilter *, trace_t *);
			getvfunc<OriginalFn>(this, 5)(this, ray, flMask, pTraceFilter, ptrace);
		}

		//	virtual void	ClipRayToEntity( const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace ) = 0;
		// 	CEngineTrace::ClipRayToEntity(Ray_t  const&,unsigned int,IHandleEntity *,CGameTrace *);								// 0003  000C
		void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, CBaseEntity *pEnt, trace_t *pTrace)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const Ray_t &, unsigned int, CBaseEntity *, trace_t *);
			getvfunc<OriginalFn>(this, 3)(this, ray, fMask, pEnt, pTrace);
		}

	};
}