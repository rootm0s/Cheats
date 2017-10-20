#pragma once
#include <Windows.h>
#include <iostream>
#include <Shlwapi.h>
#include <vector>
#include <algorithm>
#include "checksum_crc.h"

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef GetProp
#undef GetProp
#endif

struct ScreenSize_t;
class CVMTHook;
class RecvProp;
class RecvTable;
class ClientClass;
class CRecvProxyData;
class Vector;
struct matrix3x4_t;
class IBaseClient;
class IClientEntityList;
class CPrediction;
class CGameMovement;
class IMoveHelper;
class IVEngineClient;
class IEngineTrace;
class IVModelInfo;
class IVModelRender;
class IVRenderView;
class IPanel;
class ISurface;
class IMaterial;
class KeyValues;
class IMaterialSystem;
class CGlobalVarsBase;
class COffsets;
class CEntity;
class CBaseHandle;

extern IMaterial* ShaderMat;
extern IMaterial* ShaderMatNoZ;
extern IMaterial* TexturedMat;
extern IMaterial* TexturedMatNoZ;
extern ScreenSize_t ScreenSize;
extern unsigned long Font;
extern DWORD dwGameResources;
extern DWORD dwGlowBase;
extern BYTE bSendPacket;
extern int iKnife;

typedef void*(*CreateClientClassFn)(int entnum, int serialNum);
typedef void*(*CreateEventFn)();
typedef void*(*CreateInterfaceFn)(const char *pName, int *pReturnCode);

typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);
typedef void(*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);
typedef void(*ArrayLengthRecvProxyFn)(void *pStruct, int objectID, int currentArrayLength);
typedef bool(*GetSymbolProc_t)(const char *pKey);

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define M_PI		((float)(3.14159265358979323846))
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

#define NUM_ENT_ENTRY_BITS		(11 + 2)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX	0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS		16
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK			(( 1 << NUM_SERIAL_NUM_BITS) - 1)

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

#define	FL_ONGROUND		(1<<0)
#define FL_DUCKING		(1<<1)
#define	FL_WATERJUMP	(1<<3)
#define FL_ONTRAIN		(1<<4)
#define FL_INRAIN		(1<<5)
#define FL_FROZEN		(1<<6)
#define FL_ATCONTROLS	(1<<7)
#define	FL_CLIENT		(1<<8)
#define FL_FAKECLIENT	(1<<9)
#define	FL_INWATER		(1<<10)	

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define	IN_LOOKSPIN		(1 << 25)

namespace Math
{
	FORCEINLINE float FastSqrt(float x);
	FORCEINLINE float FastRSqrtFast(float x);
	FORCEINLINE void SinCos(float radians, float *sine, float *cosine);
	FORCEINLINE float DotProduct(const Vector& a, const Vector& b);
	FORCEINLINE void CrossProduct(const Vector& a, const Vector& b, Vector& result);

	FORCEINLINE void VectorDivide(const Vector& a, float b, Vector& c);
	FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c);
	FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c);
	FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c);
	FORCEINLINE void VectorMultiply(const Vector& a, float b, Vector& c);
	FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c);
	FORCEINLINE float VectorLength(const Vector& v);

	float VectorNormalize(Vector& vec);

	void VectorTransform(Vector in, matrix3x4_t &matrix, Vector &out);
	void VectorAngles(Vector forward, Vector &angles);
	void AngleVectors(Vector& angles, Vector *forward);

	void MatrixAngles(const matrix3x4_t& matrix, Vector &angles);
	void MatrixPosition(const matrix3x4_t &matrix, Vector &position);
	void MatrixAngles(const matrix3x4_t &matrix, Vector &angles, Vector &position);

	void NormalizeAngles(Vector& angles);
	void ClampAngles(Vector& angles);
}

enum FontFlags_t
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,

	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,

	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum WeaponIDs
{
	weapon_none,
	weapon_deagle,
	weapon_elite,
	weapon_fiveseven,
	weapon_glock,
	weapon_ak47 = 7,
	weapon_aug,
	weapon_awp,
	weapon_famas,
	weapon_g3sg1,
	weapon_galilar = 13,
	weapon_m249,
	weapon_m4a4 = 16,
	weapon_mac10,
	weapon_p90 = 19,
	weapon_ump45 = 24,
	weapon_xm1014,
	weapon_bizon,
	weapon_mag7,
	weapon_negev,
	weapon_sawedoff,
	weapon_tec9,
	weapon_taser,
	weapon_p2000,
	weapon_mp7,
	weapon_mp9,
	weapon_nova,
	weapon_p250,
	weapon_scar20 = 38,
	weapon_sg556,
	weapon_ssg08,
	weapon_knife_ct = 42,
	weapon_flashbang,
	weapon_hegrenade,
	weapon_smokegrenade,
	weapon_molotov,
	weapon_decoy,
	weapon_incgrenade,
	weapon_c4,
	weapon_knife_t = 59,
	weapon_m4a1s = 60,
	weapon_usp = 61,
	weapon_cz75 = 63,
	weapon_r8,
	weapon_bayonet = 500,
	weapon_flip = 505,
	weapon_gut,
	weapon_karambit,
	weapon_m9bayonet,
	weapon_huntsman,
	weapon_falchion = 512,
	weapon_butterfly = 515,
	weapon_pushdagger
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

struct ScreenSize_t
{
	int Width, Height;
};

typedef struct player_info_s
{
	BYTE					pad00[16];
	char					name[128];
	BYTE					pad01[4];
	char					guid[33];
	BYTE					pad02[151];
} player_info_t;

struct ModelRenderInfo_t
{
	BYTE pad00[28];
	const void* pModel;
	BYTE pad01[16];
	int entity_index;
	BYTE pad02[14];
};

struct GlowObjectDefinition
{
	CEntity* pEntity;
	float m_flRed;
	float m_flGreen;
	float m_flBlue;
	float m_flAlpha;
	BYTE pad0[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	BYTE pad1[15];
};

class Utilities
{
public:
	static void DebugPrint(const char *pszText, ...)
	{
		va_list va_alist;
		char* szBuffer = new char[2048];

		va_start(va_alist, pszText);
		int len = vsprintf(szBuffer, pszText, va_alist);
		va_end(va_alist);

		szBuffer[len + 0] = '\r';
		szBuffer[len + 1] = '\n';
		szBuffer[len + 2] = '\0';
		len = len + 2;

		printf(szBuffer);

		delete[] szBuffer;
	}

	template<typename T> T static GetVFunction(void* pTable, unsigned int iIndex)
	{
		void* pFunction = nullptr;

		if (pTable != nullptr)
		{
			pFunction = (void*)((DWORD*)(*(DWORD***)pTable))[iIndex];
		}
		else
		{
#ifdef DEBUG
			DebugPrint("Table == nullptr!");
#endif
			return nullptr;
		}

		return reinterpret_cast<T>(pFunction);
	}

	static DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
	{
		for (DWORD i = 0; i < dwLen; i++)
			if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i);

		return 0;
	}
private:
	static bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == NULL;
	}
};

class CVMTHook
{
public:
	CVMTHook(DWORD *Instance)
	{
		ppInstance = (*(PDWORD**)Instance);
		pOldVMT = *ppInstance;

		VMTSize = dwGetVMTCount(pOldVMT);

		if (VMTSize <= 0) return;

		pNewVMT = new DWORD[VMTSize];
		memcpy(pNewVMT, pOldVMT, sizeof(DWORD) * VMTSize);

		*ppInstance = (pNewVMT);

		Instance = pNewVMT;
	}

	void Hook(DWORD dwNewFunc, unsigned int iIndex)
	{
		if (pNewVMT && pOldVMT && iIndex <= VMTSize && iIndex >= 0)
		{
			pNewVMT[iIndex] = dwNewFunc;
		}
	}

	void Unhook(unsigned int iIndex)
	{
		if (pNewVMT && pOldVMT && iIndex <= VMTSize && iIndex >= 0)
		{
			pNewVMT[iIndex] = pOldVMT[iIndex];
		}
	}

	template<typename T> T Function(int Index)
	{
		return (T)(pOldVMT[Index]);
	}

private:
	DWORD dwGetVMTCount(PDWORD pdwVMT)
	{
		DWORD dwIndex = 0;

		for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
		{
			if (IsBadCodePtr((FARPROC)pdwVMT[dwIndex]))
				break;
		}
		return dwIndex;
	}

	DWORD** ppInstance;
	DWORD*  pNewVMT, *pOldVMT;
	DWORD   VMTSize;
};

struct COffsets
{
	DWORD Armor,
		LifeState,
		Flags,
		Health,
		PunchAngle,
		ViewPunchAngle,
		IsDefusing,
		TeamNum,
		TickBase,
		Clip1,
		NextPrimaryAttack,
		Velocity,
		ViewOffset,
		Origin,
		ActiveWeapon,
		Angles,
		HitboxSet,
		clrRender,
		ShotsFired,
		OwnerXuidLow,
		OwnerXuidHigh,
		PaintKit,
		Wear,
		StatTrak,
		Seed,
		ItemIDHigh,
		CustomName,
		ItemID,
		ModelIndex,
		ViewModelIndex,
		WorldModelIndex,
		hOwnerEntity,
		IsScoped,
		GlowIndex,
		Min,
		Max,
		CompetitiveRank,
		Rotation,
		Spotted,
		C4Blow;
};

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
		long long	m_Int64;
	};
	int	m_Type;
};

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};

class RecvProp
{
public:
	int GetFlags() const
	{
		return m_Flags;
	}

	const char* GetName() const
	{
		return m_pVarName;
	}

	int GetType() const
	{
		return m_RecvType;
	}

	RecvTable* GetDataTable() const
	{
		return m_pDataTable;
	}

	RecvVarProxyFn GetProxyFn() const
	{
		return m_ProxyFn;
	}

	void SetProxyFn(RecvVarProxyFn fn)
	{
		m_ProxyFn = fn;
	}

	int GetOffset() const
	{
		return m_Offset;
	}

	void SetOffset(int o)
	{
		m_Offset = o;
	}

public:

	char					*m_pVarName;
	int						m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;

	bool					m_bInsideArray;

	const void*				m_pExtraData;

	RecvProp				*m_pArrayProp;
	ArrayLengthRecvProxyFn	m_ArrayLengthProxy;

	RecvVarProxyFn			m_ProxyFn;
	DataTableRecvVarProxyFn	m_DataTableProxyFn;

	RecvTable				*m_pDataTable;
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	const char				*m_pParentArrayPropName;
};

class RecvTable
{
public:
	int GetNumProps()
	{
		return m_nProps;
	}

	RecvProp* GetProp(int i)
	{
		return &m_pProps[i];
	}

	const char* GetName()
	{
		return m_pNetTableName;
	}

	RecvProp		*m_pProps;
	int				m_nProps;

	void*			m_pDecoder;

	char			*m_pNetTableName;
private:

	bool			m_bInitialized;
	bool			m_bInMainList;
};

class ClientClass
{
public:
	ClientClass(char *pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable *pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;
	char					*m_pNetworkName;
	RecvTable				*m_pRecvTable;
	ClientClass				*m_pNext;
	int						m_ClassID;
};

class Color
{
public:
	Color()
	{
		SetColor(0, 0, 0, 0);
	}

	Color(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 0);
	}

	Color(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	void SetColor(int _r, int _g, int _b, int _a = 0)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }

	FORCEINLINE unsigned char &operator[](int index)
	{
		return _color[index];
	}

	FORCEINLINE const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	FORCEINLINE bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	FORCEINLINE bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	FORCEINLINE Color &operator=(const Color &rhs)
	{
		SetColor(rhs.r(), rhs.g(), rhs.b(), rhs.a());
		return *this;
	}

	unsigned char _color[4];
};

class Vector
{
public:
	float x, y, z;

	inline Vector::Vector(void){}
	inline Vector::Vector(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}

	inline float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	inline float* Base()
	{
		return (float*)this;
	}

	inline float const* Base() const
	{
		return (float const*)this;
	}

	inline void Random(float minVal, float maxVal)
	{
		x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	}

	inline void Zero()
	{
		x = y = z = 0.0f;
	}

	inline bool operator==(const Vector& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	inline bool operator!=(const Vector& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	FORCEINLINE Vector& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	FORCEINLINE Vector& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	FORCEINLINE Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	FORCEINLINE Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	FORCEINLINE  Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	FORCEINLINE  Vector& operator/=(float fl)
	{
		float oofl = 1.0f / fl;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		return *this;
	}

	FORCEINLINE Vector&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	FORCEINLINE Vector&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	inline void Negate()
	{
		x = -x; y = -y; z = -z;
	}

	inline float Length(void) const
	{
		return Math::VectorLength(*this);
	}

	FORCEINLINE float LengthSqr(void) const
	{
		return (x*x + y*y + z*z);
	}

	inline float LengthRecipFast(void) const
	{
		return Math::FastRSqrtFast(LengthSqr());
	}

	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	inline bool IsZeroFast() const __restrict
	{
		return (*reinterpret_cast<const int *>(&x) == 0 &&
			*reinterpret_cast<const int *>(&y) == 0 &&
			*reinterpret_cast<const int *>(&z) == 0);
	}

	inline float NormalizeInPlace()
	{
		return Math::VectorNormalize(*this);
	}

	inline Vector Normalized() const
	{
		Vector norm = *this;
		Math::VectorNormalize(norm);
		return norm;
	}

	inline bool IsLengthGreaterThan(float val) const
	{
		return LengthSqr() > val*val;
	}

	inline bool IsLengthLessThan(float val) const
	{
		return LengthSqr() < val*val;
	}

	FORCEINLINE bool WithinAABox(Vector const &boxmin, Vector const &boxmax)
	{
		return (
			(x >= boxmin.x) && (x <= boxmax.x) &&
			(y >= boxmin.y) && (y <= boxmax.y) &&
			(z >= boxmin.z) && (z <= boxmax.z)
			);
	}

	inline float DistTo(const Vector &vOther) const
	{
		Vector delta;
		Math::VectorSubtract(*this, vOther, delta);
		return delta.Length();
	}

	FORCEINLINE float DistToSqr(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	inline void	CopyToArray(float* rgfl) const
	{
		rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
	}

	inline void	MulAdd(const Vector& a, const Vector& b, float scalar)
	{
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	inline float Dot(const Vector& vOther) const
	{
		return Math::DotProduct(*this, vOther);
	}

	inline Vector& operator=(const Vector &vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	inline int LargestComponent() const
	{
		float flAbsx = fabs(x);
		float flAbsy = fabs(y);
		float flAbsz = fabs(z);
		if (flAbsx > flAbsy)
		{
			if (flAbsx > flAbsz)
				return 0;
			return 2;
		}
		if (flAbsy > flAbsz)
			return 1;
		return 2;
	}

	inline float Length2D(void) const
	{
		return (float)Math::FastSqrt(x*x + y*y);
	}

	inline float Length2DSqr(void) const
	{
		return (x*x + y*y);
	}

	inline Vector ProjectOnto(const Vector& onto)
	{
		return onto * (this->Dot(onto) / (onto.LengthSqr()));
	}

	inline Vector operator-(void) const
	{
		return Vector(-x, -y, -z);
	}

	inline Vector operator+(const Vector& v) const
	{
		Vector res;
		Math::VectorAdd(*this, v, res);
		return res;
	}

	inline Vector operator-(const Vector& v) const
	{
		Vector res;
		Math::VectorSubtract(*this, v, res);
		return res;
	}

	inline Vector operator*(const Vector& v) const
	{
		Vector res;
		Math::VectorMultiply(*this, v, res);
		return res;
	}

	inline Vector operator/(const Vector& v) const
	{
		Vector res;
		Math::VectorDivide(*this, v, res);
		return res;
	}

	inline Vector operator*(float fl) const
	{
		Vector res;
		Math::VectorMultiply(*this, fl, res);
		return res;
	}

	inline Vector operator/(float fl) const
	{
		Vector res;
		Math::VectorDivide(*this, fl, res);
		return res;
	}

	inline Vector Cross(const Vector& vOther) const
	{
		Vector res;
		Math::CrossProduct(*this, vOther, res);
		return res;
	}

	inline Vector Min(const Vector &vOther) const
	{
		return Vector(x < vOther.x ? x : vOther.x,
			y < vOther.y ? y : vOther.y,
			z < vOther.z ? z : vOther.z);
	}

	inline Vector Max(const Vector &vOther) const
	{
		return Vector(x > vOther.x ? x : vOther.x,
			y > vOther.y ? y : vOther.y,
			z > vOther.z ? z : vOther.z);
	}
};

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned &vOther)
	{
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
		return *this;
	}

	float w;
};

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	void Init(Vector xAxis, Vector yAxis, Vector zAxis, Vector vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	matrix3x4_t(Vector xAxis, Vector yAxis, Vector zAxis, Vector vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector const & p)
	{
		m_flMatVal[0][3] = p.x;
		m_flMatVal[1][3] = p.y;
		m_flMatVal[2][3] = p.z;
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_flMatVal[i][j] = (float)0x7FC00000;
			}
		}
	}

	float *operator[](int i)				{ return m_flMatVal[i]; }
	const float *operator[](int i) const	{ return m_flMatVal[i]; }
	float *Base()							{ return &m_flMatVal[0][0]; }
	const float *Base() const				{ return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

struct VMatrix
{
public:
	float* operator[](int i)				{ return m[i]; }
	const float* operator[](int i) const	{ return m[i]; }
	float *Base()							{ return &m[0][0]; }
	const float *Base() const				{ return &m[0][0]; }

	float	m[4][4];
};

struct csurface_t
{
	const char		*name;
	short			surfaceProps;
	unsigned short	flags;
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class CBaseTrace
{
public:
	bool IsDispSurface(void)				{ return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void)		{ return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void)		{ return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void)			{ return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void)			{ return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;
	unsigned short	worldSurfaceIndex;

	CEntity*		m_pEnt;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& vOther);
};

struct Ray_t
{
	VectorAligned m_Start;
	VectorAligned m_Delta;
	VectorAligned m_StartOffset;
	VectorAligned m_Extents;

	const matrix3x4_t* m_pWorldAxisTransform;

	bool m_IsRay;
	bool m_IsSwept;

	Ray_t() : m_pWorldAxisTransform(NULL) { }

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		m_StartOffset.Zero();
		m_Start = start;
	}

	void Init(Vector& start, Vector& end, Vector& mins, Vector& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start - m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

class CBaseHandle
{
public:
	inline CBaseHandle::CBaseHandle()
	{
		m_Index = INVALID_EHANDLE_INDEX;
	}

	inline CBaseHandle::CBaseHandle(const CBaseHandle &other)
	{
		m_Index = other.m_Index;
	}

	inline CBaseHandle::CBaseHandle(unsigned long value)
	{
		m_Index = value;
	}

	inline CBaseHandle::CBaseHandle(int iEntry, int iSerialNumber)
	{
		Init(iEntry, iSerialNumber);
	}

	inline void CBaseHandle::Init(int iEntry, int iSerialNumber)
	{
		m_Index = iEntry | (iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS);
	}

	inline void CBaseHandle::Term()
	{
		m_Index = INVALID_EHANDLE_INDEX;
	}

	inline bool CBaseHandle::IsValid() const
	{
		return m_Index != INVALID_EHANDLE_INDEX;
	}

	inline int CBaseHandle::GetEntryIndex() const
	{
		if (!IsValid())
			return NUM_ENT_ENTRIES - 1;

		return m_Index & ENT_ENTRY_MASK;
	}

	inline int CBaseHandle::GetSerialNumber() const
	{
		return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	inline int CBaseHandle::ToInt() const
	{
		return (int)m_Index;
	}

	inline bool CBaseHandle::operator !=(const CBaseHandle &other) const
	{
		return m_Index != other.m_Index;
	}

	inline bool CBaseHandle::operator ==(const CBaseHandle &other) const
	{
		return m_Index == other.m_Index;
	}

	inline bool CBaseHandle::operator ==(const void* pEnt) const
	{
		return Get() == pEnt;
	}

	inline bool CBaseHandle::operator !=(const void* pEnt) const
	{
		return Get() != pEnt;
	}

	inline const CBaseHandle& CBaseHandle::operator=(const void* pEntity)
	{
		return Set(pEntity);
	}

	inline const CBaseHandle& CBaseHandle::Set(const void* pEntity)
	{
		if (!pEntity)
		{
			m_Index = INVALID_EHANDLE_INDEX;
		}

		return *this;
	}

	inline void* CBaseHandle::Get() const
	{
		return 0;
	}

	unsigned long	m_Index;
};

template< class T >
class CHandle : public CBaseHandle
{
public:
	template<class T>
	CHandle()
	{
	}

	template<class T>
	CHandle(int iEntry, int iSerialNumber)
	{
		Init(iEntry, iSerialNumber);
	}

	template<class T>
	CHandle(const CBaseHandle &handle)
		: CBaseHandle(handle)
	{
	}

	template<class T>
	CHandle(T *pObj)
	{
		Term();
		Set(pObj);
	}

	template<class T>
	inline CHandle<T> FromIndex(int index)
	{
		CHandle<T> ret;
		ret.m_Index = index;
		return ret;
	}

	template<class T>
	inline T* Get() const
	{
		return (T*)CBaseHandle::Get();
	}

	template<class T>
	inline operator T *()
	{
		return Get();
	}

	template<class T>
	inline operator T *() const
	{
		return Get();
	}

	template<class T>
	inline bool operator !() const
	{
		return !Get();
	}

	template<class T>
	inline bool operator==(T *val) const
	{
		return Get() == val;
	}

	template<class T>
	inline bool operator!=(T *val) const
	{
		return Get() != val;
	}

	template<class T>
	void Set(const T* pVal)
	{
		CBaseHandle::Set(reinterpret_cast<const void*>(pVal));
	}

	template<class T>
	inline const CBaseHandle& operator=(const T *val)
	{
		Set(val);
		return *this;
	}

	template<class T>
	T* operator -> () const
	{
		return Get();
	}
};

typedef CHandle<CEntity> EHANDLE;

class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	virtual ~CUserCmd() { };

	void Reset()
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Zero();
		aimdirection.Zero();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;

		// TrackIR
		headangles.Zero();
		headoffset.Zero();
	}

	CUserCmd& operator =(const CUserCmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		aimdirection = src.aimdirection;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

		// TrackIR
		headangles = src.headangles;
		headoffset = src.headoffset;

		return *this;
	}

	CUserCmd(const CUserCmd& src)
	{
		*this = src;
	}

	CRC32_t GetChecksum(void)
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_Final(&crc);

		return crc;
	}

	int		command_number;

	int		tick_count;

	Vector	viewangles;

	Vector	aimdirection;

	float	forwardmove;
	float	sidemove;
	float	upmove;
	int		buttons;
	byte    impulse;
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;

	short	mousedx;
	short	mousedy;

	bool	hasbeenpredicted;

	Vector headangles;
	Vector headoffset;
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

struct mstudiobbox_t
{
	int					bone;
	int					group;
	Vector				bbmin;
	Vector				bbmax;
	int					szhitboxnameindex;
	int					unused[3];
	float				pillradius;
	int					unused1[4];

	const char* pszHitboxName() const
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	// No copy constructors allowed
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiohitboxset_t
{
	int					sznameindex;
	inline char * const	pszName(void) const { return ((char *)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t *pHitbox(int i) const { return (mstudiobbox_t*)(((byte *)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	BYTE				pad00[12];
	char				name[64];
	BYTE				pad01[80];
	int					numbones;
	int					boneindex;
	BYTE				pad02[12];
	int					hitboxsetindex;
	BYTE				pad03[228];

	// Look up hitbox set by index
	inline mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((BYTE *)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual int	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual int GetTraceType() const
	{
		return 0;
	}

	void* pSkip;
};

class IBaseClient
{
public:
	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass*(__thiscall* GetAllClasses_t)(void*);
		return Utilities::GetVFunction<GetAllClasses_t>(this, 8)(this);
	}
};

class CInput
{
public:
	CUserCmd* GetUserCmd(int sequence_number)
	{
		CUserCmd *vcmd = *(CUserCmd**)((DWORD)this + 0xEC);
		CUserCmd *result = &vcmd[sequence_number % 150];

		return result;
	}

	CVerifiedUserCmd* GetVerifiedUserCmd(int sequence_number)
	{
		CVerifiedUserCmd* vcmd = *(CVerifiedUserCmd**)((DWORD)this + 0xF0);
		CVerifiedUserCmd* result = &vcmd[sequence_number % 150];

		return result;
	}
};

class IClientEntityList
{
public:
	CEntity* GetClientEntity(int entnum)
	{
		typedef void*(__thiscall* GetClientEntity_t)(void*, int);
		return reinterpret_cast<CEntity*>(Utilities::GetVFunction<GetClientEntity_t>(this, 3)(this, entnum));
	}

	CEntity* GetClientEntityFromHandle(CBaseHandle hEnt)
	{
		typedef void*(__thiscall* GetClientEntity_t)(void*, CBaseHandle);
		return reinterpret_cast<CEntity*>(Utilities::GetVFunction<GetClientEntity_t>(this, 4)(this, hEnt));
	}

	int	GetHighestEntityIndex(void)
	{
		typedef int(__thiscall* GetHighestEntityIndex_t)(void*);
		return Utilities::GetVFunction<GetHighestEntityIndex_t>(this, 6)(this);
	}
};

class IMoveHelper
{
public:
	void SetHost(CEntity* host)
	{
		typedef void(__thiscall* SetHost_t)(void*, CEntity*);
		Utilities::GetVFunction<SetHost_t>(this, 1)(this, host);
	}
};

class CPrediction
{
public:
	void SetupMove(CEntity* player, CUserCmd* cmd, IMoveHelper* helper, void* move)
	{
		typedef void(__thiscall* SetupMove_t)(void*, CEntity*, CUserCmd*, IMoveHelper*, void*);
		Utilities::GetVFunction<SetupMove_t>(this, 20)(this, player, cmd, helper, move);
	}

	void FinishMove(CEntity* player, CUserCmd* ucmd, void* move)
	{
		typedef void(__thiscall* FinishMove_t)(void*, CEntity*, CUserCmd*, void*);
		Utilities::GetVFunction<FinishMove_t>(this, 21)(this, player, ucmd, move);
	}
};

class CGameMovement
{
public:
	void ProcessMovement(CEntity* player, void* move)
	{
		typedef void(__thiscall* ProcessMovement_t)(void*, CEntity*, void*);
		Utilities::GetVFunction<ProcessMovement_t>(this, 1)(this, player, move);
	}

	void StartTrackPredictionErrors(CEntity* player)
	{
		typedef void(__thiscall* SetHost_t)(void*, CEntity*);
		Utilities::GetVFunction<SetHost_t>(this, 3)(this, player);
	}

	void FinishTrackPredictionErrors(CEntity* player)
	{
		typedef void(__thiscall* SetHost_t)(void*, CEntity*);
		Utilities::GetVFunction<SetHost_t>(this, 4)(this, player);
	}
};

class IVEngineClient
{
public:
	void GetScreenSize(int &width, int &height)
	{
		typedef void(__thiscall* GetScreenSize_t)(void*, int &, int &);
		Utilities::GetVFunction<GetScreenSize_t>(this, 5)(this, width, height);
	}

	void ClientCmd(const char* szCommand)
	{
		typedef void(__thiscall* GetScreenSize_t)(void*, const char*);
		Utilities::GetVFunction<GetScreenSize_t>(this, 7)(this, szCommand);
	}

	bool GetPlayerInfo(int index, player_info_s* playerInfo)
	{
		typedef bool(__thiscall* GetPlayerInfo_t)(void*, int, player_info_s*);
		return Utilities::GetVFunction<GetPlayerInfo_t>(this, 8)(this, index, playerInfo);
	}

	int GetLocalPlayer(void)
	{
		typedef int(__thiscall* GetLocalPlayer_t)(void*);
		return Utilities::GetVFunction<GetLocalPlayer_t>(this, 12)(this);
	}

	bool IsInGame(void)
	{
		typedef bool(__thiscall* IsInGame_t)(void*);
		return Utilities::GetVFunction<IsInGame_t>(this, 26)(this);
	}

	void SetViewAngles(Vector &ang)
	{
		typedef void(__thiscall* SetViewAngles_t)(void*, Vector&);
		Utilities::GetVFunction<SetViewAngles_t>(this, 19)(this, ang);
	}

	const VMatrix& WorldToScreenMatrix(void)
	{
		typedef VMatrix&(__thiscall* GetLocalPlayer_t)(void*);
		return Utilities::GetVFunction<GetLocalPlayer_t>(this, 37)(this);
	}

	bool IsTakingScreenshot(void)
	{
		typedef bool(__thiscall* IsInGame_t)(void*);
		return Utilities::GetVFunction<IsInGame_t>(this, 92)(this);
	}
};

class IEngineTrace
{
public:
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace)
	{
		typedef void(__thiscall* TraceRay_t)(void*, const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace);
		Utilities::GetVFunction<TraceRay_t>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

class IVModelInfo
{
public:
	int GetModelIndex(const char* name)
	{
		typedef int(__thiscall* GetModelIndex_t)(void*, const char*);
		return Utilities::GetVFunction<GetModelIndex_t>(this, 2)(this, name);
	}

	const char* GetModelName(const void* model)
	{
		typedef const char*(__thiscall* GetModelName_t)(void*, const void*);
		return Utilities::GetVFunction<GetModelName_t>(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const void* mod)
	{
		typedef studiohdr_t*(__thiscall* GetStudiomodel_t)(void*, const void*);
		return Utilities::GetVFunction<GetStudiomodel_t>(this, 30)(this, mod);
	}
};

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* mat, int pOverride = 0, int idk = 0)
	{
		typedef void(__thiscall* ForcedMaterialOverride_t)(void*, IMaterial*, int, int);
		Utilities::GetVFunction<ForcedMaterialOverride_t>(this, 1)(this, mat, pOverride, idk);
	}
};

class IVRenderView
{
public:
	void SetBlend(float blend)
	{
		typedef void(__thiscall* SetBlend_t)(void*, float);
		Utilities::GetVFunction<SetBlend_t>(this, 4)(this, blend);
	}

	void SetColorModulation(float const* blend)
	{
		typedef void(__thiscall* SetColorModulation_t)(void*, const float*);
		Utilities::GetVFunction<SetColorModulation_t>(this, 6)(this, blend);
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char*(__thiscall* GetName_t)(void*, unsigned int);
		return Utilities::GetVFunction<GetName_t>(this, 36)(this, vguiPanel);
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* DrawSetColor_t)(void*, int, int, int, int);
		Utilities::GetVFunction<DrawSetColor_t>(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* DrawFilledRect_t)(void*, int, int, int, int);
		Utilities::GetVFunction<DrawFilledRect_t>(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utilities::GetVFunction<OriginalFn>(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawSetTextFont(unsigned long HFont)
	{
		typedef void(__thiscall* DrawSetTextFont_t)(void*, unsigned long);
		Utilities::GetVFunction<DrawSetTextFont_t>(this, 23)(this, HFont);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* DrawSetTextColor_t)(void*, Color);
		Utilities::GetVFunction<DrawSetTextColor_t>(this, 24)(this, Color(r, g, b, a));
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* DrawSetTextPos_t)(void*, int, int);
		Utilities::GetVFunction<DrawSetTextPos_t>(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* wszText, int length, int drawfont = 0)
	{
		typedef void(__thiscall* DrawPrintText_t)(void*, const wchar_t*, int, int);
		Utilities::GetVFunction<DrawPrintText_t>(this, 28)(this, wszText, length, drawfont);
	}

	unsigned long CreateFont(void)
	{
		typedef unsigned long(__thiscall* CreateFont_t)(void*);
		return Utilities::GetVFunction<CreateFont_t>(this, 71)(this);
	}

	bool SetFontGlyphSet(unsigned long font, const char* szName, int tall, int weight, int blur, int scanlines, FontFlags_t flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* SetFontGlyphSet_t)(void*, unsigned long, const char*, int, int, int, int, FontFlags_t, int, int);
		return Utilities::GetVFunction<SetFontGlyphSet_t>(this, 72)(this, font, szName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	void GetTextSize(unsigned long HFont, const wchar_t* wszText, int &Width, int &Height)
	{
		typedef void(__thiscall* GetTextSize_t)(void*, unsigned long, const wchar_t*, int &, int &);
		Utilities::GetVFunction<GetTextSize_t>(this, 79)(this, HFont, wszText, Width, Height);
	}
};

class IMaterial
{
public:
	const char* GetName()
	{
		typedef const char*(__thiscall* GetName_t)(void*);
		return Utilities::GetVFunction<GetName_t>(this, 0)(this);
	}

	const char* GetTextureGroupName()
	{
		typedef const char*(__thiscall* GetTextureGroupName_t)(void*);
		return Utilities::GetVFunction<GetTextureGroupName_t>(this, 1)(this);
	}

	void IncrementReferenceCount(void)
	{
		typedef void(__thiscall* IncrementReferenceCount_t)(void*);
		Utilities::GetVFunction<IncrementReferenceCount_t>(this, 12)(this);
	}

	void DecrementReferenceCount(void)
	{
		typedef void(__thiscall* DecrementReferenceCount_t)(void*);
		Utilities::GetVFunction<DecrementReferenceCount_t>(this, 13)(this);
	}

	void ColorModulation(Color color)
	{
		typedef void(__thiscall* AlphaModulation_t)(void*, float);
		typedef void(__thiscall* ColorModulation_t)(void*, float, float, float);
		Utilities::GetVFunction<AlphaModulation_t>(this, 27)(this, color.a() / 255.f);
		Utilities::GetVFunction<ColorModulation_t>(this, 28)(this, color.r() / 255.f, color.g() / 255.f, color.b() / 255.f);
	}

	void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
	{
		typedef void(__thiscall* SetMaterialVarFlag_t)(void*, MaterialVarFlags_t, bool);
		Utilities::GetVFunction<SetMaterialVarFlag_t>(this, 29)(this, flag, on);
	}
};

class KeyValues
{
public:
	static void SetUseGrowableStringTable(bool bUseGrowableTable);

	KeyValues(const char *setName)
	{
		Init();
		SetName(setName);
	}

	class AutoDelete
	{
	public:
		explicit inline AutoDelete(KeyValues *pKeyValues) : m_pKeyValues(pKeyValues) {}
		explicit inline AutoDelete(const char *pchKVName) : m_pKeyValues(new KeyValues(pchKVName)) {}
		inline ~AutoDelete(void) { if (m_pKeyValues) delete[] m_pKeyValues; }
		inline void Assign(KeyValues *pKeyValues) { m_pKeyValues = pKeyValues; }
		KeyValues *operator->()	{ return m_pKeyValues; }
		operator KeyValues *()	{ return m_pKeyValues; }
	private:
		AutoDelete(AutoDelete const &x); // forbid
		AutoDelete & operator= (AutoDelete const &x); // forbid
		KeyValues *m_pKeyValues;
	};

	void SetName(const char *setName)
	{
		m_iKeyName = 2;
	}
public:
	KeyValues(KeyValues&);	// prevent copy constructor being used

	// prevent delete being called except through deleteThis()
	~KeyValues();

	void Init()
	{
		m_iKeyName = -1;
		m_iDataType = 0;

		m_pSub = NULL;
		m_pPeer = NULL;
		m_pChain = NULL;

		m_sValue = NULL;
		m_wsValue = NULL;
		m_pValue = NULL;

		m_bHasEscapeSequences = false;

		// for future proof
		memset(unused, 0, sizeof(unused));
	}

	int m_iKeyName;	// keyname is a symbol defined in KeyValuesSystem

	// These are needed out of the union because the API returns string pointers
	char *m_sValue;
	wchar_t *m_wsValue;

	// we don't delete these
	union
	{
		int m_iValue;
		float m_flValue;
		void *m_pValue;
		unsigned char m_Color[4];
	};

	char	   m_iDataType;
	char	   m_bHasEscapeSequences; // true, if while parsing this KeyValue, Escape Sequences are used (default false)
	char	   m_bEvaluateConditionals; // true, if while parsing this KeyValue, conditionals blocks are evaluated (default true)
	char	   unused[1];

	KeyValues *m_pPeer;	// pointer to next key in list
	KeyValues *m_pSub;	// pointer to Start of a new sub key list
	KeyValues *m_pChain;// Search here if it's not in our list

	static int(*s_pfGetSymbolForString)(const char *name, bool bCreate);
	static const char *(*s_pfGetStringForSymbol)(int symbol);

	static int GetSymbolForStringClassic(const char *name, bool bCreate = true);
	static const char *GetStringForSymbolClassic(int symbol);

	// Functions that use the growable string table
	static int GetSymbolForStringGrowable(const char *name, bool bCreate = true);
	static const char *GetStringForSymbolGrowable(int symbol);

	// Functions to get external access to whichever of the above functions we're going to call.
	static int CallGetSymbolForString(const char *name, bool bCreate = true) { return s_pfGetSymbolForString(name, bCreate); }
	static const char *CallGetStringForSymbol(int symbol) { return s_pfGetStringForSymbol(symbol); }

	bool LoadFromBuffer(const char* resourceName, const char* pBuffer, void* pFileSystem = NULL, const char* pPathID = NULL, GetSymbolProc_t pfnEvaluateSymbolProc = NULL)
	{
		static DWORD dwAddress = NULL;

		if (dwAddress == NULL)
			dwAddress = Utilities::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0x7FFFFFFF, (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89", "xxxxxxxxxxxxxx");

		typedef bool(__thiscall *LoadFromBuffer_t)(KeyValues*, const char*, const char*, void*, const char*, void*);
		return ((LoadFromBuffer_t)dwAddress)(this, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
	}
};

class IMaterialSystem
{
public:
	IMaterial* CreateMaterial(char* name, KeyValues* keyValues)
	{
		typedef IMaterial*(__thiscall* CreateMaterial_t)(void*, char*, KeyValues*);
		return Utilities::GetVFunction<CreateMaterial_t>(this, 83)(this, name, keyValues);
	}

	IMaterial* FindMaterial(char const* textureName, char const* textureGroup, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* FindMaterial_t)(void*, char const*, char const*, bool, char const*);
		return Utilities::GetVFunction<FindMaterial_t>(this, 84)(this, textureName, textureGroup, complain, pComplainPrefix);
	}
};

class CGlobalVarsBase
{
public:
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			absoluteframestarttimestddev;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	void*			pSaveData;
	bool			m_bClient;
	bool			m_bRemoteClient;
	int				nTimestampNetworkingBase;
	int				nTimestampRandomizeWindow;
};