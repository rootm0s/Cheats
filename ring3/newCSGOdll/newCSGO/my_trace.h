
// **** 2014.9.12
struct Vec3
{
	float X;
	float Y;
	float Z;
};
	//typedef Vector Vec3;
    struct cplane_t
    {
        Vec3  normal;
        float   dist;
        BYTE    type;
        BYTE    signbits;
        BYTE    pad[ 2 ];
    };

    struct csurface_t
    {
        const char*             name;
        short                   surfaceProps;
        unsigned short          flags;
    };

    class CBaseTrace
    {
    public:
        Vec3                  startpos;
        Vec3                  endpos;
        cplane_t                plane;
        float                   fraction;
        int                     contents;
        unsigned short          dispFlags;
        bool                    allsolid;
        bool                    startsolid;
    };

    class CGameTrace : public CBaseTrace
    {
    public:
        bool                    DidHitWorld() const;
        bool                    DidHitNonWorldEntity() const;
        int                     GetEntityIndex() const;
        bool                    DidHit() const;

    public:
        float                   fractionleftsolid;
        csurface_t              surface;
        int                     hitgroup;
        short                   physicsbone;
        unsigned short          worldSurfaceIndex;
        IClientEntity*            m_pEnt;
        int                     hitbox;

        CGameTrace() { }

    private:
        CGameTrace( const CGameTrace& vOther );
    };

    inline bool CGameTrace::DidHit( VOID ) const
    {
        return fraction < 1.0f || allsolid || startsolid;
    }

    typedef CGameTrace trace_t;

    class ITraceFilter
    {
    public:
        virtual bool ShouldHitEntity( IClientEntity *pEntity, int contentsMask ) = 0;
        virtual DWORD     GetTraceType() const = 0;
    };

    class CTraceFilter : public ITraceFilter
    {
    public:
        CTraceFilter( IClientEntity* pPassEntity )
        {
            m_pClientEntityToSkip= pPassEntity;
        }

        bool ShouldHitEntity( IClientEntity* pHandleEntity, int contentsMask )
        {
            return !( pHandleEntity == m_pClientEntityToSkip);
        }

        virtual DWORD GetTraceType() const
        {
            return 1;
        }

    private:
        IClientEntity* m_pClientEntityToSkip;
    };

	

//===============
typedef float vec_t;
class  VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z) 
	{
//		Init(X,Y,Z);
//		X = 0.0f; Y=0.0f ;Z =0.0f;
	}

#ifdef VECTOR_NO_SLOW_OPERATIONS

private:
	// No copy constructors allowed if we're in optimal mode
	VectorAligned(const VectorAligned& vOther);
	VectorAligned(const Vector &vOther);

#else
public:
	explicit VectorAligned(const Vector &vOther) 
	{
//		Init(vOther.X, vOther.Y, vOther.Z);
//		vOther.X=0.0f;vOther.Y=0.0f;vOther.Z =0.0;
	}
	
	VectorAligned& operator=(const Vector &vOther)	
	{
//		Init(vOther.X, vOther.Y, vOther.Z);
		X=vOther.X;Y=vOther.Y;Z=vOther.Z;
		return *this;
	}
	
#endif
	float w;	// this space is used anyway
};

struct Ray_t
{
    VectorAligned        m_Start;
    VectorAligned        m_Delta;
    VectorAligned        m_StartOffset;
    VectorAligned        m_Extents;

    const matrix3x4_t*    m_WorldAxisTransform;

    bool                m_IsRay;
    bool                m_IsSwept;

    Ray_t ( void )
        :    m_WorldAxisTransform ( 0 )
    {
    
    }
#include "Math.h"

    void Init ( const Vector& start, const Vector& end )
    {
//        m_Delta = end-start;
		//2014.9.25
		m_Delta.X=end.X-start.X;
		m_Delta.Y=end.Y-start.Y;
		m_Delta.Z=end.Z-start.Z;

//        m_IsSwept = (m_Delta.LengthSqr () != 0);
		m_IsSwept =((m_Delta.X*m_Delta.X+m_Delta.Y*m_Delta.Y+m_Delta.Z*m_Delta.Z)  !=0);
  //      m_Extents.Init ();
		//2014.9.25
		m_Extents.X=0.0f;m_Extents.Y=0.0f;m_Extents.Z=0.0f;

        m_WorldAxisTransform = 0;

        m_IsRay = true;

   //     m_StartOffset.Init ();
		//2014.9.25
		m_StartOffset.X=0.0f;m_StartOffset.Y=0.0f;m_StartOffset.Z=0.0f;

        //m_Start = start;
		m_Start.X=start.X ;m_Start.Y =start.Y ;m_Start.Z=start.Z;
    }
};
//------- 2014.9.12
template< typename Function > Function CallVirtualFunction( PVOID Base, DWORD Index )
{
    PDWORD* VTablePointer = ( PDWORD* )Base;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[ Index ];
    return ( Function )( dwAddress );
}
    class IEngineTrace
    {
    public:

        void TraceRay( const Ray_t &ray, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace )
        {
            typedef void( __thiscall* vTraceRay )( PVOID, const Ray_t&, unsigned int, CTraceFilter*, trace_t* );
            CallVirtualFunction< vTraceRay >( this, 5 )( this, ray, fMask, pTraceFilter, pTrace );
			//return call_vfunc< oSetViewAngles >( this, 19 )( this, vAngles );

        }

    };

/* // cmodel.h
struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?

	void Init( Vector const& start, Vector const& end )
	{
		Assert( &end );
		VectorSubtract( end, start, m_Delta );

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorClear( m_Extents );
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		VectorClear( m_StartOffset );
		VectorCopy( start, m_Start );
	}

	void Init( Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs )
	{
		Assert( &end );
		VectorSubtract( end, start, m_Delta );

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract( maxs, mins, m_Extents );
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd( mins, maxs, m_StartOffset );
		m_StartOffset *= 0.5f;
		VectorAdd( start, m_StartOffset, m_Start );
		m_StartOffset *= -1.0f;
	}

	// compute inverse delta
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for ( int iAxis = 0; iAxis < 3; ++iAxis )
		{
			if ( m_Delta[iAxis] != 0.0f )
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
*/
	//==============