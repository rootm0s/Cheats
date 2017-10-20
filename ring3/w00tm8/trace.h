
#include <math.h>
enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};
struct cplane_t
{
	Vector  normal;
	float   dist;
	BYTE    type;
	BYTE    signbits;
	BYTE    pad[2];
};
struct csurface_t
{
	const char*             name;
	short                   surfaceProps;
	unsigned short  flags;
};


class CBaseTrace
{
public:
	Vector                  startpos;
	Vector                 endpos;
	cplane_t                plane;
	float                   fraction;
	int                             contents;
	unsigned short  dispFlags;
	bool                    allsolid;
	bool                    startsolid;
};



class CGameTrace : public CBaseTrace
{
public:


	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	csurface_t		surface;			// surface hit (impact surface)

	int				hitgroup;			// 0 == generic, non-zero is specific body part

	short			physicsbone;		// physics bone hit by trace in studio
	unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable

	C_BaseEntity*  m_pEnt;
	int			hitbox;



};




typedef CGameTrace trace_t;
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
		x = vOther.x; y = vOther.y; z = vOther.z;
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

	const   matrix3x4* m_pWorldAxisTransform;

	bool    m_IsRay;
	bool    m_IsSwept;

	Ray_t() : m_pWorldAxisTransform(NULL) { }

	void Init(Vector& vecStart, Vector& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;

		m_pWorldAxisTransform = NULL;

		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}
};
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(C_BaseEntity *pEntity, int contentsMask) = 0;
	virtual TraceType_t    GetTraceType() const = 0;
};



class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};
class IEngineTrace
{
public:

	void TraceRay(const Ray_t &ray, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace)
	{
		typedef void(__thiscall* oTraceRay)(PVOID, const Ray_t&, unsigned int, CTraceFilter *, trace_t*);
		getvfunc<oTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
};


#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_MONSTER		0x2000000
#define	CONTENTS_EMPTY			0		// No contents
#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	0x80
#define	CONTENTS_DEBRIS			0x4000000
#define CONTENTS_HITBOX			0x40000000
#define	MASK_SOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	CONTENTS_MONSTERCLIP	0x20000
#define MASK_NPCWORLDSTATIC (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE)