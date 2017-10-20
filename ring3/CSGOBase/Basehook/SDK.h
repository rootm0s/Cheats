//================================================================================
//VALVE SOURCE SDK REBUILD
//CREDITS: KOLO,RIFK,BADST3R
//================================================================================
#pragma once
#include "netvars.h"

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];
#include "dt_recv2.h"
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
typedef void(__cdecl* pRandomSeed)(int iSeed);
typedef ULONG EHANDLE;
typedef __m128 fltx4;

class CBaseEntity;
class CBaseCombatWeapon;
class model_t;

#define M_PI			3.14159265358979323846
#define MakePtr( Type, dwBase, dwOffset ) ( ( Type )( DWORD( dwBase ) + (DWORD)( dwOffset ) ) )

#ifdef HACK_CSGO
#define VTABLE_CREATEFONT 70
#define VTABLE_SETFONTGLYPHSET 71
#else HACK_CSS
#define VTABLE_CREATEFONT 65
#define VTABLE_SETFONTGLYPHSET 66
#endif


//USERCMD OFFSETS
#define USERCMDOFFSET 0xEC
#define VERIFIEDCMDOFFSET 0xF0
#define MULTIPLAYER_BACKUP 150

//LIFESTATE
#define	LIFE_ALIVE				0 
#define	LIFE_DYING				1 
#define	LIFE_DEAD				2 
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//BIG TRIGGER CODENZ
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

//USERCMD BUTTONS
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
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

#define CHAR_TEX_ANTLION 'A'
#define CHAR_TEX_BLOODYFLESH 'B'
#define CHAR_TEX_CONCRETE 'C'
#define CHAR_TEX_DIRT 'D'
#define CHAR_TEX_EGGSHELL 'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH 'F'
#define CHAR_TEX_GRATE 'G'
#define CHAR_TEX_ALIENFLESH 'H'
#define CHAR_TEX_CLIP 'I'
//#define CHAR_TEX_UNUSED 'J'
#define CHAR_TEX_SNOW 'K'
#define CHAR_TEX_PLASTIC 'L'
#define CHAR_TEX_METAL 'M'
#define CHAR_TEX_SAND 'N'
#define CHAR_TEX_FOLIAGE 'O'
#define CHAR_TEX_COMPUTER 'P'
//#define CHAR_TEX_UNUSED 'Q'
#define CHAR_TEX_REFLECTIVE 'R'
#define CHAR_TEX_SLOSH 'S'
#define CHAR_TEX_TILE 'T'
//#define CHAR_TEX_UNUSED 'U'
#define CHAR_TEX_VENT 'V'
#define CHAR_TEX_WOOD 'W'
//#define CHAR_TEX_UNUSED 'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS 'Y'
#define CHAR_TEX_WARPSHIELD 'Z' ///< wierd-looking jello effect for advisor shield.

struct WeaponInfo_s
{
	int iWeaponID;
	int iPenetration;
	int fCurrentDamage;
	float flDistance;
	float flRangeModifier;
	int iBulletsPerShot;
	int iAmmoType;
	float flPenetrationDistance;
	float flPenetrationPower;
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
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};
enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};
enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};
struct GlowObject_t {
	CBaseEntity*	m_pEntity;	// 0
	Vector		m_vGlowColor;	// 4
	float		m_flGlowAlpha;	// 16
	unsigned char   junk[16];	// 20
	bool		m_bShouldGlow;	// 36
	bool		m_bUnknown;	// 37
	bool		m_bFullBloomRender; // 38
	unsigned char   junk2[13];	// 39
}; // total size should be 52
class CViewSetup
{
public:
	CViewSetup()
	{
		m_flAspectRatio = 0.0f;
		m_bRenderToSubrectOfLargerScreen = false;
		m_bDoBloomAndToneMapping = true;
		m_bOrtho = false;
		m_bOffCenter = false;
		m_bCacheFullSceneState = false;
		//		m_bUseExplicitViewVector = false;
		m_bViewToProjectionOverride = false;
		//m_eStereoEye = STEREO_EYE_MONO;
	}

	// shared by 2D & 3D views

	// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	// which eye are we rendering?
	//StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

	// the rest are only used by 3D views

	// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vector		origin;

	// heading of camera (pitch, yaw, roll)
	QAngle		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_bViewToProjectionOverride;
	//VMatrix     m_ViewToProjection;
};
//WEAPONLIST 
enum WeaponId
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_P228 = 5,
	WEAPON_USP = 6,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 12,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M3 = 15,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_MP5NAVY = 18,
	WEAPON_P90 = 19,
	WEAPON_SCOUT = 20,
	WEAPON_SG550 = 21,
	WEAPON_SG552 = 22,
	WEAPON_TMP = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR17 = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	ITEM_KEVLAR = 50,
	ITEM_ASSAULTSUIT = 51,
	ITEM_NVG = 52,
	ITEM_DEFUSER = 53,
	ITEM_CUTTERS = 54,

};
struct vrect_t
{
	int				x, y, width, height;
	vrect_t			*pnext;
};
struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	CBaseEntity *pRenderable;
	const model_t *pModel;
	const matrix3x4 *pModelToWorld;
	const matrix3x4 *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;
	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};
struct mstudiobbox_t
{
	int                                     bone;
	int                                     group;
	Vector                          bbmin;
	Vector                          bbmax;
	int                                     szhitboxnameindex;
	int                                     unused[8];



	const char* pszHitboxName() const
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              pszName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   pHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};


struct studiohdr_t
{
	int					id;
	int					version;

	int					checksum;		// this has to be the same in the phy and vtx files to load!

	char				name[64];
	int					length;


	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center

	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		Assert(i >= 0 && i < numhitboxsets);
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

	// file local animations? and sequences
	//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions

	int					numlocalseq;				// sequences
	int					localseqindex;

	//public:
	bool				SequencesAvailable() const;
	int					GetNumSeq() const;
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index

	//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	// raw textures
	int					numtextures;
	int					textureindex;


	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char			*pCdtexture(int i) const { return (((char *)this) + *((int *)(((BYTE *)this) + cdtextureindex) + i)); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short		*pSkinref(int i) const { return (short *)(((BYTE *)this) + skinindex) + i; };

	int					numbodyparts;
	int					bodypartindex;

	// queryable attachable points
	//private:
	int					numlocalattachments;
	int					localattachmentindex;
	//public:
	int					GetNumAttachments(void) const;
	int					GetAttachmentBone(int i);
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone(int iAttachment, int iBone);

	// animation node to animation node transition graph
	//private:
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char			*pszLocalNodeName(int iNode) const { Assert(iNode >= 0 && iNode < numlocalnodes); return (((char *)this) + *((int *)(((BYTE *)this) + localnodenameindex) + iNode)); }
	inline BYTE			*pLocalTransition(int i) const { Assert(i >= 0 && i < (numlocalnodes * numlocalnodes)); return (BYTE *)(((BYTE *)this) + localnodeindex) + i; };

	//public:
	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char				*pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					numflexdesc;
	int					flexdescindex;

	int					numflexcontrollers;
	int					flexcontrollerindex;

	int					numflexrules;
	int					flexruleindex;

	int					numikchains;
	int					ikchainindex;

	int					nummouths;
	int					mouthindex;

	//private:
	int					numlocalposeparameters;
	int					localposeparamindex;
	//public:
	int					GetNumPoseParameters(void) const;
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					surfacepropindex;
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropindex; }

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;

	int					GetNumIKAutoplayLocks(void) const;
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences(unsigned short *pOut, int outCount) const;
	int					GetAutoplayList(unsigned short **pOut) const;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;
	// implementation specific call to get a named model
	const studiohdr_t	*FindModel(void **cache, char const *modelname) const;

	// implementation specific back pointer to virtual data
	mutable void		*virtualModel;
	//virtualmodel_t		GetVirtualModel(void) const;

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char * const pszAnimBlockName(void) const { return ((char *)this) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	mutable void		*animblockModel;
	BYTE *				GetAnimBlock(int i) const;

	int					bonetablebynameindex;
	inline const BYTE	*GetBoneTableSortedByName() const { return (BYTE *)this + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void				*pVertexBase;
	void				*pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting 
	// on static props
	BYTE				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	BYTE				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	BYTE				numAllowedRootLODs;

	BYTE				unused[1];

	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	int					unused3[2];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	int					studiohdr2index;


	// NOTE: No room to add stuff? Up the .mdl file format version 
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	int					unused2[1];

	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};

//selfexplain?
enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

//SRC SDK VECTOR SUBTRAC
//inline void VectorSubtract( const Vector& a, const Vector& b, Vector& c )
//{
//	CHECK_VALID(a);
//	CHECK_VALID(b);
//	c.x = a.x - b.x;
//	c.y = a.y - b.y;
//	c.z = a.z - b.z;
//}
//
////SRC SDK VECTORADD
//inline void VectorAdd( const Vector& a, const Vector& b, Vector& c )
//{
//	CHECK_VALID(a);
//	CHECK_VALID(b);
//	c.x = a.x + b.x;
//	c.y = a.y + b.y;
//	c.z = a.z + b.z;
//}

struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	unsigned long			m_pStudioHWData;
	CBaseEntity*		m_pRenderable;
	const matrix3x4		*m_pModelToWorld;
	unsigned long		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

//Credits: Casualhacker
inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

//======================================================================
//VALVE SDK REBUILD
//LAST UPDATE 27.12.2013
//======================================================================

//CBASENTITY REBUILD
class CBaseEntity
{
public:

	virtual void init(){};
	typedef bool(__thiscall* IsA_t)(CBaseEntity*);
#define CALLFUNC(index) return ((IsA_t)((*(DWORD**)this)[index]))(this);

	inline bool IsPlayer()
	{
		CALLFUNC(152);
	}
	inline bool IsBaseCombatWeapon()
	{
		CALLFUNC(153);
	}

	Vector GetAbsOrigin()
	{
		__asm
		{
			MOV ECX, this
				MOV EAX, DWORD PTR DS : [ECX]
				CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	QAngle GetAbsAngles()
	{
		__asm
		{
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}


	int GetIndex()
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, DWORD PTR DS : [EDI + 0x8]	//pointer to iclientrenderable (cbaseentity + 0x8)
				MOV EDX, DWORD PTR DS : [ECX]
				CALL DWORD PTR DS : [EDX + 0x28]	//pointer to getindex()
		}
	}

	bool IsDormant()
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, [EDI + 0x8]
				MOV EDX, DWORD PTR DS : [ecx]
				CALL[EDX + 0x24]
		}
	}

	model_t* GetModel()
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, [EDI + 0x4]
				MOV EDX, DWORD PTR DS : [ECX]
				CALL[EDX + 0x20]
		}
	}

	bool SetupBones(matrix3x4* matrix, int maxbones, int mask, float time)
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, DWORD PTR DS : [EDI + 0x4]
				MOV EDX, DWORD PTR DS : [ECX]
				PUSH time
				PUSH mask
				PUSH maxbones
				PUSH matrix
				CALL DWORD PTR DS : [EDX + 0x34]
		}
	}

	//for example
	int GetShotsFired()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_iShotsFired);
	}
	//u can do something cleaner
	int GetHealth()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_iHealth); //m_iHealth
	}

	int GetTeamNumber()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_iTeamNum);
	}
	int GetArmorValue()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_ArmorValue); //m_iHealth
	}
	bool IsProtected()
	{
		return *(bool*)((DWORD)this + NetVars::Player::m_bGunGameImmunity);
	}
	int GetFlags()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_fFlags); //m_iHealth
	}
	char GetLifeState()
	{
		return *(char*)((DWORD)this + NetVars::Player::m_lifeState); //m_iHealth
	}
	int m_Local()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_Local); //m_iHealth
	}
	QAngle GetPunchAngle()
	{
		return *(QAngle*)((DWORD)this + NetVars::Player::m_Local + NetVars::Player::m_aimPunchAngle); //wrong
	}
	Vector viewGetPunchAngle()
	{
		return *(Vector*)((DWORD)this + NetVars::Player::m_Local + NetVars::Player::m_viewPunchAngle); //wrong
	}
	Vector vecGetPunchAngle()
	{
		return *(Vector*)((DWORD)this + NetVars::Player::m_Local + NetVars::Player::m_aimPunchAngle); //wrong
	}
	Vector GetvecOrigin()
	{
		return *(Vector*)((DWORD)this + NetVars::Player::m_vecOrigin);
	}
	Vector GetEyePosition()
	{
		return (*(Vector*)((DWORD)this + NetVars::Player::m_vecOrigin) + *(Vector*)((DWORD)this + NetVars::Player::m_vecViewOffset));
	}
	int GetTickBase()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_nTickBase);
	}
	int GetFlashDuration()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_flFlashDuration);
	}
	int GetFlashAlpha()
	{
		return *(int*)((DWORD)this + NetVars::Player::m_flFlashMaxAlpha);
	}
	bool HasHelmet()
	{
		return *(bool*)((DWORD)this + NetVars::Player::m_bHasHelmet);
	}
	Vector GetAbsVelocity()
	{
		return *(Vector*)((DWORD)this + NetVars::Player::m_vecVelocity); //m_iHealth
	}
	QAngle GetEyeAngles() //now u have getabsangle it's better than netvar ok
	{
		QAngle EyeAngle = *(QAngle*)((DWORD)this + NetVars::Player::m_angEyeAngles);
		return EyeAngle;
	}
	Vector vecGetEyeAngles() //now u have getabsangle it's better than netvar ok
	{
		return *(Vector*)((DWORD)this + NetVars::Player::m_angEyeAngles);
	}

};
//=======================================================================

class CBaseCombatWeapon : public CBaseEntity
{
public:

	int GetClip()
	{
		return *(int*)((DWORD)this + NetVars::Weapon::m_iClip1); //m_iHealth
	}
	float GetNextPrimaryAttack()
	{
		return *(float*)((DWORD)this + NetVars::Weapon::m_flNextPrimaryAttack);
	}
	int GetState()
	{
		return *(int*)((DWORD)this + NetVars::Weapon::m_iState);
	}

};

//======================================================================
//!BEGIN VALVE SDK REBUILD
namespace ValveSDK
{
	//======================================================================
	//CLIENTCLASS
	class ClientClass
	{
	public:

		const char* GetName(void)
		{
			return *(char**)(this + 0x8);
		}
		RecvTable* GetTable()
		{
			return *(RecvTable**)(this + 0xC);
		}
		ClientClass* NextClass()
		{
			return *(ClientClass**)(this + 0x10);
		}
		int GetClassID(void)
		{
			return *(int*)(this + 0x14);
		}
	};
	//======================================================================
	//HLCLIENT
	class HLCLient
	{
	public:
		ClientClass* GetAllClasses(VOID)
		{
			typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 8)(this);
			//don't forget the return here for all type except void!
			// oki <3
		}
		void GetPlayerView(CViewSetup &playerview)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CViewSetup);
			getvfunc<OriginalFn>(this, 68)(this, playerview);
		}
		void RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const CViewSetup, int, int);
			getvfunc<OriginalFn>(this, 27)(this, view, nClearFlags, whatToDraw);
		}
		int IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, int, ButtonCode_t, const char*);
			return getvfunc<OriginalFn>(this, 20)(this, eventcode, keynum, pszCurrentBinding);
		}
	};
	class IMaterial
	{
	public:
		const char*     GetName()
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 0)(this);
		}
		const char*     GetTextureGroupName()
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 1)(this);
		}
		void AlphaModulate(float alpha)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, float);
			getvfunc<OriginalFn>(this, 27)(this, alpha);
		}
		void ColorModulate(float r, float g, float b)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, float, float, float);
			getvfunc<OriginalFn>(this, 28)(this, r, g, b);
		}
		void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, MaterialVarFlags_t, bool);
			getvfunc<OriginalFn>(this, 29)(this, flag, on);
		}
	};
	//======================================================================
	//CModelInfo
	class CModelInfo
	{
	public:
		const char* GetModelName(const model_t* pModel)
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc<OriginalFn>(this, 3)(this, pModel);
		}
		void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const model_t*, int, IMaterial**);
			getvfunc<OriginalFn>(this, 17)(this, model, count, ppMaterial);
		}
		studiohdr_t     *GetStudiomodel(const model_t *mod)
		{
			typedef studiohdr_t* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc<OriginalFn>(this, 30)(this, mod);
		}
	};
	//======================================================================
	//ENGINECLIENT

	class CEngineClient
	{
	public:

		/*
		struct player_infos
		{
		//DECLARE_BYTESWAP_DATADESC();
		uint64unknown;
		uint64xuid;
		// scoreboard information
		charname[MAX_PLAYER_NAME_LENGTH];

		char unknown01[96];

		// local server user ID, unique while server is running
		intuserID;
		charguid[SIGNED_GUID_LEN + 1];
		intfriendsID;
		};

		*/
		typedef struct player_info_s
		{
			unsigned long long      unknown;
			unsigned long long      xuid;
			char                    name[128];
			int                     userID;
			char                    guid[32 + 1];
			unsigned int            friendsID;
			char                    friendsName[128];
			bool                    fakeplayer;
			bool                    ishltv;
			unsigned long           customFiles[4];
			unsigned char           filesDownloaded;
		} player_info_t;


		void GetScreenSize(int& width, int& height)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int&, int&);
			return getvfunc<OriginalFn>(this, 5)(this, width, height);
		}
		void ClientCmd(const char *szCmdString)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const char*);
			getvfunc<OriginalFn>(this, 7)(this, szCmdString);
		}
		bool isinGame(void)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 26)(this);
		}
		bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, int, player_info_t*);
			return getvfunc<OriginalFn>(this, 8)(this, iIndex, pInfo);
		}
		int GetMaxClients(void)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 20)(this);
		}
		int GetLocalPlayer(VOID)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 12)(this);
		}
		float GetLastTimeStamp(VOID)
		{
			typedef float(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 14)(this);
		}
		const matrix3x4& WorldToScreenMatrix(void)
		{
			typedef const matrix3x4& (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 37)(this);
		}
		void ExecuteClientCmd(char const* szCommand)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, char const* szCommand);
			getvfunc<OriginalFn>(this, 108)(this, szCommand);
		}
		void SetViewAngles(Vector& vAngles)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, Vector&);
			getvfunc<OriginalFn>(this, 19)(this, vAngles);
		}
		void GetViewAngles(Vector& vAngles)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, Vector&);
			getvfunc<OriginalFn>(this, 18)(this, vAngles);
		}

	};
	//======================================================================
	//SURFACE
	class ISurface
	{
	public:
		void DrawSetColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			getvfunc<OriginalFn>(this, 14)(this, r, g, b, a);
		}
		void DrawFilledRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			getvfunc<OriginalFn>(this, 15)(this, x0, y0, x1, y1);
		}
		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			getvfunc<OriginalFn>(this, 18)(this, x0, y0, x1, y1);
		}
		void DrawSetTextFont(unsigned long font)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
			getvfunc<OriginalFn>(this, 22)(this, font);
		}
		void DrawSetTextColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			getvfunc<OriginalFn>(this, 24)(this, r, g, b, a);
		}
		void SurfaceGetCursorPos(int &x, int &y)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int &, int &);
			getvfunc<OriginalFn>(this, 99)(this, x, y);
		}
		void SurfaceSetCursorPos(int x, int y)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int);
			getvfunc<OriginalFn>(this, 100)(this, x, y);
		}
		bool AddCustomFontFile(const char* fontName)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, const char*);
			return getvfunc<OriginalFn>(this, 72)(this, fontName);
		}
		void DrawSetTextPos(int x, int y)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int);
			getvfunc<OriginalFn>(this, 25)(this, x, y);
		}
		void DrawPrintText(const wchar_t *text, int textLen)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t *, int, int);
			return getvfunc<OriginalFn>(this, 27)(this, text, textLen, 0);
		}
		unsigned long CreateFont()
		{
			typedef unsigned int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, VTABLE_CREATEFONT)(this);
		}
		void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
			getvfunc<OriginalFn>(this, VTABLE_SETFONTGLYPHSET)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
		}
		void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
		{
			typedef void(__thiscall* OriginalFn)(void*, unsigned long font, const wchar_t *text, int &wide, int &tall);
			getvfunc<OriginalFn>(this, 78)(this, font, text, wide, tall);
		}
		void DrawOutlinedRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(void*, int x0, int y0, int x1, int y1);
			getvfunc<OriginalFn>(this, 17)(this, x0, y0, x1, y1);
		}
		void DrawOutlinedCircle(int x, int y, int radius, int segments)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			getvfunc<OriginalFn>(this, 102)(this, x, y, radius, segments);
		}
	};
	//======================================================================
	//PANEL
	class IPanel
	{
	public:
		unsigned int GetPanel(int type)
		{
			typedef unsigned int(__thiscall *tGetPanel)(void*, int type);
			return getvfunc<tGetPanel>(this, 1)(this, type);
		}
		const char *GetName(unsigned int vguiPanel)
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
			return getvfunc<OriginalFn>(this, 36)(this, vguiPanel);
		}
	};
	//======================================================================
	//ENTITYLIS
	class CEntityList
	{
	public:
		CBaseEntity* GetClientEntity(int entnum)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 3)(this, entnum);
		}
		CBaseEntity* GetClientEntityFromHandle(int hEnt)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 4)(this, hEnt);
		}
		int GetHighestEntityIndex(void)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 6)(this);
		}
	};
	//======================================================================
	//INPUTw
	class CInput
	{
	public:
		class CUserCmd
		{
		public:
			CRC32_t GetChecksum(void) const
			{
				CRC32_t crc;
				CRC32_Init(&crc);
				CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
				CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
				CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
				CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection)); //new
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
			BYTE u1[4];
			int command_number;
			int tick_count;
			Vector viewangles;
			Vector aimdirection;
			float forwardmove;
			float sidemove;
			float upmove;
			int buttons;
			BYTE impulse;
			int weaponselect;
			int weaponsubtype;
			int random_seed;
			short mousedx;
			short mousedy;
			bool hasbeenpredicted;
			Vector headangles;
			Vector headoffset;
		};
		class CVerifiedUserCmd
		{
		public:
			CUserCmd			m_cmd;
			unsigned long		m_crc;
		};
		CUserCmd* GetUserCmd(int slot, int seq)
		{
			typedef CUserCmd* (__thiscall* OriginalFn)(PVOID, int, int);
			return getvfunc<OriginalFn>(this, 8)(this, slot, seq);
		}
	};

	class ImoveHelper
	{
	public:
		void SetHost(CBaseEntity *pPlayer)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer);
		}
	};
	class IVModelRender
	{
	public:
		void DrawModelExecute(void* context, DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, void* context, DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
			getvfunc<OriginalFn>(this, 21)(this, context, state, pInfo, pCustomBoneToWorld);
		}
		void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, IMaterial *newMaterial, OverrideType_t nOverrideType);
			getvfunc<OriginalFn>(this, 1)(this, newMaterial, nOverrideType);
		}
	};
	class IVRenderView
	{
	public:
		void SetColorModulation(float  const* blend)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, float  const* blend);
			getvfunc<OriginalFn>(this, 6)(this, blend);
		}
		void SetBlend(float blend)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, float blend);
			getvfunc<OriginalFn>(this, 4)(this, blend);
		}
	};
	//======================================================================
	//PREDICTION
	class CPrediction
	{
	public:
		void SetupMove(CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID empty, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID empty, PVOID moveData);
			getvfunc<OriginalFn>(this, 20)(this, player, ucmd, NULL, moveData);
		}
		void FinishMove(CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID moveData);
			getvfunc<OriginalFn>(this, 21)(this, player, ucmd, moveData);
		}
	};
	//======================================================================
	//GAMEMOVEMENT
	class CGameMovement
	{
	public:
		void ProcessMovement(CBaseEntity *pPlayer, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer, PVOID moveData);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer, moveData);
		}
	};
	//======================================================================
	//DEBUGOVERLAY
	class CDebugOverlay
	{
	public:
		bool ScreenPosition(const Vector& vIn, Vector& vOut)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, const Vector&, Vector&);
			return getvfunc<OriginalFn>(this, 12)(this, vIn, vOut);
		}
	};
	//======================================================================
	//GLOBALVARS
	class CGlobalVars
	{
	public:
		float realtime;
		int framecount;
		float absoluteframetime;
		float unknown;
		float curtime;
		float frametime;
		int maxclients;
		int tickcount;
		float interval_per_tick;
		float interpolation_amount;
	};
	//======================================================================
	//======================================================================
	//TRACE
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
			float normal[3];
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


		class ITraceFilter
		{
		public:
			virtual bool ShouldHitEntity(CBaseEntity *pEntity, int contentsMask) = 0;
			virtual TraceType_t	GetTraceType() const = 0;
		};

		class CTraceFilter : public ITraceFilter
		{
		public:
			bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
			{
				return !(pEntityHandle == pSkip);
			}

			virtual TraceType_t GetTraceType() const
			{
				return TRACE_EVERYTHING;
			}

			void* pSkip;
		};


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
	//======================================================================
	//C_BASECOMBATWEAPON
	class CBaseCombatWeapon
	{
	public:

	};
	//======================================================================
	//GAMEEVENTMANAGER
	class IGameEvent
	{
	public:

		const char* GetName()
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 1)(this);
		}

		int GetInt(const char *keyName, int defaultValue)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, const char *, int);
			return getvfunc<OriginalFn>(this, 6)(this, keyName, defaultValue);
		}
		bool GetBool(const char *keyName, bool value)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, const char *, bool);
			return getvfunc<OriginalFn>(this, 5)(this, keyName, value );
		}
		char GetString(const char *keyName, const char *value)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, const char *, const char *);
			return getvfunc<OriginalFn>(this, 9)(this, keyName, value);
		}
		/*bool GetBool(const char *keyName, bool value)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, const char *, bool);
			return getvfunc<OriginalFn>(this, 5)(this, keyName, value);
		}*/
	};

	class IGameEventListener2
	{
	public:
		virtual	~IGameEventListener2(void) {};

		// FireEvent is called by EventManager if event just occured
		// KeyValue memory will be freed by manager if not needed anymore
		virtual void FireGameEvent(IGameEvent *event) = 0;
	};

	class IGameEventManager
	{
	public:
		bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, IGameEventListener2*, const char*, bool);
			return getvfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
		}

		bool FireEventClientSide(IGameEvent *event)
		{
			typedef bool(__thiscall* OriginalFn)(PVOID, IGameEvent*);
			return getvfunc<OriginalFn>(this, 8)(this, event);
		}
	};

	class CGlowObjectManager
	{
	public:
		struct GlowObjectDefinition_t  {
			CBaseEntity*	m_pEntity;	// 0
			Vector		m_vGlowColor;	// 4
			float		m_flGlowAlpha;	// 16
			unsigned char   junk[16];	// 20
			bool		m_bShouldGlow;	// 36
			bool		m_bUnknown;	// 37
			bool		m_bFullBloomRender; // 38
			unsigned char   junk2[13];	// 39
		};
		std::vector < GlowObjectDefinition_t > m_GlowObjectDefinitions;
		int m_nFirstFreeSlot;
		/*int RegisterGlowObject(CBaseEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, CBaseEntity*, const Vector&, float, bool, bool, int);
			return getvfunc<OriginalFn>(this, 6)(this, pEntity, vGlowColor, flGlowAlpha, bRenderWhenOccluded, bRenderWhenUnoccluded, nSplitScreenSlot);
		}*/
	};
	class Color
	{
	public:
		// constructors
		Color()
		{
			*((int *)this) = 0;
		}
		Color(int r, int g, int b)
		{
			SetColor(r, g, b, 0);
		}
		Color(int r, int g, int b, int a)
		{
			SetColor(r, g, b, a);
		}

		// set the color
		// r - red component (0-255)
		// g - green component (0-255)
		// b - blue component (0-255)
		// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
		void SetColor(int r, int g, int b, int a = 0)
		{
			_color[0] = (unsigned char)r;
			_color[1] = (unsigned char)g;
			_color[2] = (unsigned char)b;
			_color[3] = (unsigned char)a;
		}

		void GetColor(int &r, int &g, int &b, int &a) const
		{
			r = _color[0];
			g = _color[1];
			b = _color[2];
			a = _color[3];
		}

		void SetRawColor(int color32)
		{
			*((int *)this) = color32;
		}

		int GetRawColor() const
		{
			return *((int *)this);
		}

		inline int r() const	{ return _color[0]; }
		inline int g() const	{ return _color[1]; }
		inline int b() const	{ return _color[2]; }
		inline int a() const	{ return _color[3]; }

		unsigned char &operator[](int index)
		{
			return _color[index];
		}

		const unsigned char &operator[](int index) const
		{
			return _color[index];
		}

		bool operator == (const Color &rhs) const
		{
			return (*((int *)this) == *((int *)&rhs));
		}

		bool operator != (const Color &rhs) const
		{
			return !(operator==(rhs));
		}

	private:
		unsigned char _color[4];
	};
}
//!END VALVE SDK REBUILD
//=======================================================================

//=======================================================================


