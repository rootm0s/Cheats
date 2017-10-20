#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "dt_recv2.h"

#include "vector.h"
#include "hook.h"
#include "crc.h"

//thx @ViKiNG

typedef float matrix3x4[3][4];
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

struct color24
{
	byte r, g, b;
};

typedef struct color32_s
{
	bool operator!=(const struct color32_s &other) const;

	byte r, g, b, a;
} color32;

inline bool color32::operator!=(const color32 &other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

struct colorVec
{
	unsigned r, g, b, a;
};


class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int color32)
	{
		*((int *)this) = color32;
	}
	Color(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 255);
	}
	Color(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(int _r, int _g, int _b, int _a = 255)
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

	void SetRawColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int*)this);
	}

	int GetD3DColor() const
	{
		return ((int)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
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

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

	Color &operator=(const color32 &rhs)
	{
		_color[0] = rhs.r;
		_color[1] = rhs.g;
		_color[2] = rhs.b;
		_color[3] = rhs.a;
		return *this;
	}

	color32 ToColor32() const
	{
		color32 newColor;
		newColor.r = _color[0];
		newColor.g = _color[1];
		newColor.b = _color[2];
		newColor.a = _color[3];
		return newColor;
	}

	float* Base()
	{
		float clr[3];

		clr[0] = _color[0] / 255.0f;
		clr[1] = _color[1] / 255.0f;
		clr[2] = _color[2] / 255.0f;

		return &clr[0];
	}

	float Hue() const
	{
		if (_color[0] == _color[1] && _color[1] == _color[2])
		{
			return 0.0f;
		}

		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float delta = max - min;
		float hue = 0.0f;

		if (r == max)
		{
			hue = (g - b) / delta;
		}
		else if (g == max)
		{
			hue = 2 + (b - r) / delta;
		}
		else if (b == max)
		{
			hue = 4 + (r - g) / delta;
		}
		hue *= 60;

		if (hue < 0.0f)
		{
			hue += 360.0f;
		}
		return hue;
	}

	float Saturation() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float l, s = 0;

		if (max != min)
		{
			l = (max + min) / 2;
			if (l <= 0.5f)
				s = (max - min) / (max + min);
			else
				s = (max - min) / (2 - max - min);
		}
		return s;
	}

	float Brightness() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		return (max + min) / 2;
	}

	Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
				);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
				);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
				);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
				);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
				);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
				);
		}
	}

	static Color Red()		{ return Color(255, 0, 0); }
	static Color Green()	{ return Color(0, 255, 0); }
	static Color Blue()		{ return Color(0, 0, 255); }
	static Color LightBlue(){ return Color(50, 160, 255); }
	static Color Grey()		{ return Color(128, 128, 128); }
	static Color DarkGrey()	{ return Color(45, 45, 45); }
	static Color Black()	{ return Color(0, 0, 0); }
	static Color White()	{ return Color(255, 255, 255); }
	static Color Purple()	{ return Color(220, 0, 220); }

private:
	unsigned char _color[4];
};
enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};
enum EFontFlags
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
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};
class player_info_t {
public:
	char __pad1[0x10];
	char name[32];
	char __pad2[0x64];
	char guid[32 + 1];
	char __pad3[0x17B];
	bool fakeplayer;
};
class ISurface {
public:

	void DrawSetColor(int r, int g, int b) {
		typedef void(__thiscall* oGetColor)(PVOID, int, int, int);
		return getvfunc<oGetColor>(this, 14)(this, r, g, b);
	}

	void DrawSetColor(int r, int g, int b, int a) {
		typedef void(__thiscall* oGetColor)(PVOID, int, int, int, int);
		return getvfunc<oGetColor>(this, 14)(this, r, g, b, a);
	}

	void DrawSetColor(Color col) {
		typedef void(__thiscall* oGetColor)(PVOID, Color);
		return getvfunc<oGetColor>(this, 14)(this, col);
	}

	void DrawLine(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oGetLine)(PVOID, int, int, int, int);
		return getvfunc<oGetLine>(this, 19)(this, x, y, xx, yy);
	}

	void DrawFilledRect(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oDrawRect)(PVOID, int, int, int, int);
		return getvfunc<oDrawRect>(this, 16)(this, x, y, xx, yy);
	}

	void DrawOutlinedRect(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oGetRect)(PVOID, int, int, int, int);
		return getvfunc<oGetRect>(this, 18)(this, x, y, xx, yy);
	}

	unsigned long CreateFontA() {
		typedef unsigned long(__thiscall* oCreateFont)(PVOID);
		return getvfunc<oCreateFont>(this, 71)(this);
	}

	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) {
		typedef bool(__thiscall* oCreateFont)(PVOID, unsigned long, const char *, int, int, int, int, int, int, int);
		return getvfunc<oCreateFont>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	void DrawSetTextPos(int x, int y) {
		typedef void(__thiscall* oDrawSetTextPos)(PVOID, int, int);
		return getvfunc<oDrawSetTextPos>(this, 26)(this, x, y);
	}

	void DrawSetTextFont(unsigned long font) {
		typedef void(__thiscall* oDrawSetTextFont)(PVOID, unsigned long);
		return getvfunc<oDrawSetTextFont>(this, 23)(this, font);
	}

	void DrawSetTextColor(Color col) {
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return getvfunc<oDrawSetTextColor>(this, 24)(this, col);
	}

	void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) {
		typedef void(__thiscall* oDrawPrintText)(PVOID, const wchar_t*, int, FontDrawType_t);
		return getvfunc<oDrawPrintText>(this, 28)(this, text, textLen, drawType);
	}

	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef bool(__thiscall* oGetTextSize)(PVOID, unsigned long, const wchar_t*, int&, int&);
		return getvfunc< oGetTextSize >(this, 79)(this, font, text, wide, tall);
	}

};
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
	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 8)(this);
	}
};
class IPanel {
public:

	const char* GetName(unsigned int vguiPanel) {
		typedef const char*(__thiscall* oGetName)(PVOID, unsigned int);
		return getvfunc<oGetName>(this, 36)(this, vguiPanel);
	}

};

class IBaseClientDll {
public:

};

class IVEngineClient {
public:

	int GetLocalPlayer() {
		typedef int(__thiscall* oGetLocal)(PVOID);
		return getvfunc<oGetLocal>(this, 12)(this);
	}

	void GetScreenSize(int& width, int& height) {
		typedef void(__thiscall* oScreenSize)(PVOID, int&, int&);
		return getvfunc<oScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int index, player_info_t* pInfo) {
		typedef bool(__thiscall* oPlayerInfo)(PVOID, int, player_info_t*);
		return getvfunc<oPlayerInfo>(this, 8)(this, index, pInfo);
	}

	bool IsInGame() {
		typedef bool(__thiscall* oIsInGame)(PVOID);
		return getvfunc<oIsInGame>(this, 26)(this);
	}

	bool IsConnected() {
		typedef bool(__thiscall* oIsConnected)(PVOID);
		return getvfunc<oIsConnected>(this, 27)(this);
	}

	void ExecuteClientCmd(const char* pCmd) {
		typedef void(__thiscall* oCmd)(PVOID, const char*);
		return getvfunc<oCmd>(this, 108)(this, pCmd);
	}

	bool IsTakingScreenShot() {
		typedef bool(__thiscall* oScrShot)(PVOID);
		return getvfunc<oScrShot>(this, 92)(this);
	}

	const matrix3x4& GetWorldToScreenMatrix() {
		typedef const matrix3x4&(__thiscall* oW2S)(PVOID);
		return getvfunc<oW2S>(this, 37)(this);
	}

	void SetViewAngles(Vector& ViewAngles) {
		typedef void(__thiscall* oSetViewAngle)(PVOID, Vector&);
		return getvfunc<oSetViewAngle>(this, 19)(this, ViewAngles);
	}

	void GetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oGetViewAngles)(PVOID, Vector&);
		return getvfunc< oGetViewAngles >(this, 18)(this, vAngles);
	}

	int GetPlayerForUserID(int ix) {
		typedef int(__thiscall* oIx)(PVOID, int);
		return getvfunc<oIx>(this, 9)(this, ix);
	}
};

class CUserCmd
{
public:
	virtual ~CUserCmd() { };

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;
		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &CommandNumber, sizeof(CommandNumber));
		CRC32_ProcessBuffer(&crc, &TickCount, sizeof(TickCount));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection)); //new
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &randomseed, sizeof(randomseed));
		CRC32_ProcessBuffer(&crc, &mousedX, sizeof(mousedX));
		CRC32_ProcessBuffer(&crc, &mousedY, sizeof(mousedY));
		CRC32_Final(&crc);
		return crc;
	}

	int CommandNumber;
	int TickCount;

	Vector viewangles;
	Vector aimdirection;

	float forwardmove;
	float sidemove;
	float upmove;

	int buttons;

	unsigned char impulse;

	int weaponselect;
	int weaponsubtype;
	int randomseed;

	short mousedX;
	short mousedY;

	bool HasBeenPredicted;

	char PAD1[0x18];
};



enum WeaponsUpdate { // Latest WPTable Dump by ViKiNG.
	WEAPON_DEAGLE = 1,
	WEAPON_ELITES = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_PPBIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_ZEUS = 31,
	WEAPON_P2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	KNIFE_KNIFE_CT = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	KNIFE_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75AUTO = 63,
	WEAPON_R8REVOLVER = 64,
	KNIFE_BAYONET = 500,
	KNIFE_FLIPKNIFE = 505,
	KNIFE_GUTKNIFE = 506,
	KNIFE_KARAMBIT = 507,
	KNIFE_M9BAYONET = 508,
	KNIFE_HUNTSMANKNIFE = 509,
	KNIFE_FALCHIONKNIFE = 512,
	KNIFE_BOWIEKNIFE = 514,
	KNIFE_BUTTERFLYKNIFE = 515,
	KNIFE_SHADOWDAGGERS = 516
};

class C_BaseCombatWeapon {
public:

	int WeaponID() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x2F80); // ItemDefinitionIndex
	}

	bool IsKnife() {
		int id = this->WeaponID();
		return (id == KNIFE_BAYONET || id == KNIFE_BOWIEKNIFE || id == KNIFE_BUTTERFLYKNIFE || id == KNIFE_FALCHIONKNIFE || id == KNIFE_FLIPKNIFE || id == KNIFE_GUTKNIFE || id == KNIFE_HUNTSMANKNIFE || id == KNIFE_KARAMBIT || id == KNIFE_KNIFE_CT || id == KNIFE_KNIFE_T || id == KNIFE_M9BAYONET || id == KNIFE_SHADOWDAGGERS);
	}

	bool IsBomb() {
		int id = this->WeaponID();
		return (id == WEAPON_C4);
	}

};


class C_BaseEntity {
public:

	Vector GetAbsOrigin() {
		return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x134);
	}

	Vector GetEyePos() {
		return GetAbsOrigin() + *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x104);
	}

	int Health() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xFC);
	}

	// Tog bort SetViewAngles & GetViewAngles. Dem hör till Engine, inte C_BaseEntity.

	int Team() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xF0);
	}

	int CrosshairIndex() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xA944 - 0x4);
	}

	int Flags() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x100);
	}

	int Armor() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x1337);
	}

	bool IsDormant() {
		return *reinterpret_cast<bool*>((DWORD)this + (DWORD)0xE9);
	}

	Vector GetBonePos(int i)
	{
		matrix3x4 boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, 0x100, GetTickCount64()))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}

	Vector GetBonePositon(int bone) //jag kör SetupBones på mitt egna hack, lol varför har jag inte lakt in det här? LAZY
	{
		DWORD BoneMatrix;
		BoneMatrix = *(DWORD*)(this + 0x2698);
		Vector vecBone;
		vecBone.x = *(float*)(BoneMatrix + 0x30 * bone + 0xC);
		vecBone.y = *(float*)(BoneMatrix + 0x30 * bone + 0x1C);
		vecBone.z = *(float*)(BoneMatrix + 0x30 * bone + 0x2C);
		return vecBone;
	}

	Vector AimPunch() {
		return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x3018);
	}

	Vector LocalViewAngles()
	{
		return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x4D0C);
	}
	// Behövs inte p.g.a att vi kan använda våran Engine till sådant.

	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		return getvfunc<oSetupBones>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	void Teleport(Vector const* pAngle1, QAngle const* pAngle2, Vector const* pAngle3) {
		typedef void(__thiscall* oTeleport)(PVOID, Vector const*, QAngle const*, Vector const*);
		return getvfunc<oTeleport>(this, 109)(this, pAngle1, pAngle2, pAngle3);
	}

	Vector GetVelocity() {
		return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x110);
	}

	int GlowIndex() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xA310);
	}

	bool IsValid() {
		// Check if player is valid.
		return (this->Health() > 0 && !this->IsDormant());
	}

	bool IsImmune() {
		return *reinterpret_cast<bool*>((DWORD)this + (DWORD)0x38A0);
	}

	int iClip1() {
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x31F4);
	}

	bool CanShoot() {
		return (this->iClip1() > 0);
	}

	bool GetLifeState() {
		return *reinterpret_cast<bool*>((DWORD)this + (DWORD)0x25B);
	}

	C_BaseCombatWeapon* GetActive();
};

class CGlobalVarsBase
{
public:
	CGlobalVarsBase(bool bIsClient);
	bool IsClient() const;
	int GetNetworkBase(int nTick, int nEntity);
public:
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float	absoluteframestarttimestddev;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	void*			pSaveData;
private:
	bool			m_bClient;
public:
	bool			m_bRemoteClient;
private:
	int				nTimestampNetworkingBase;
	int				nTimestampRandomizeWindow;
};

namespace Utils {
	ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask,
		ULONG uCodeBase, ULONG uSizeOfCode)
	{
		BOOL bPatternDidMatch = FALSE;
		HMODULE hModule = GetModuleHandleA(sModuleName.c_str());

		if (!hModule)
			return 0x0;

		PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
		PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

		if (uCodeBase == 0x0)
			uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

		if (uSizeOfCode == 0x0)
			uSizeOfCode = pOptionalHeader->SizeOfCode;

		ULONG uArraySize = sMask.length();

		if (!uCodeBase || !uSizeOfCode || !uArraySize)
			return 0x0;

		for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
		{
			for (size_t t = 0; t < uArraySize; t++)
			{
				if (*((PBYTE)i + t) == pbPattern[t] || sMask.c_str()[t] == '?')
					bPatternDidMatch = TRUE;

				else
				{
					bPatternDidMatch = FALSE;
					break;
				}
			}

			if (bPatternDidMatch)
				return i;
		}

		return 0x0;
	}

	HMODULE GetModuleHandleSafe(const char* pszModuleName)
	{
		HMODULE hmModuleHandle = NULL;

		do
		{
			hmModuleHandle = GetModuleHandleA(pszModuleName);
			Sleep(1);
		} while (hmModuleHandle == NULL);

		return hmModuleHandle;
	}

	bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)   return 0;
		return (*szMask) == NULL;
	}
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
	{
		for (DWORD i = 0; i<dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
		return 0;
	}
	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandleA(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}
	DWORD xFindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		return 0;
	}


}

class KeyValues
{
public:
	char _pad[0x20];//csgo, for css its a diff size
};

enum ClientFrameStage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	unsigned long	m_crc;
};

class CInput
{
public:
	CUserCmd *GetUserCmd(int sqnum)
	{
		typedef CUserCmd*(__thiscall* Fn)(void*, int, int);
		return getvfunc<Fn>(this, 8)(this, 0, sqnum);
	}
};

enum eOverrideType {
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS = 1,
	OVERRIDE_DEPTH_WRITE = 2,
	OVERRIDE_WHATEVER = 3
};

enum eMaterialVarFlags {
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
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

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};

class model_t {
public:///
};

struct vrect_t
{
	int				x, y, width, height;
	vrect_t			*pnext;
};
struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	C_BaseEntity *pRenderable;
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

struct mstudiobone_t
{
	int                                     sznameindex;
	inline char * const GetName(void) const { return ((char *)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector                          pos;
	float                           quat[4];
	Vector                          rot;
	Vector                          posscale;
	Vector                          rotscale;

	matrix3x4                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void *           GetProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
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
	inline mstudiobone_t *GetBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
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

struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	PVOID					m_pStudioHWData;
	PVOID					m_pRenderable;
	const matrix3x4			*m_pModelToWorld;
	int						m_decals;
	int						m_drawFlags;
	int						m_lod;
};

class IMaterial {
public:
	/*
	void IncrementReferenceCount(): 12
	void AlphaModulate(float alpha): 27
	void ColorModulate(float r, float g, float b): 28
	void SetVarFlag(eMaterialVarFlags flag, bool on): 29
	bool GetVarFlag(eMaterialVarFlags flag): 30
	bool IsErrorMaterial(): 42
	float GetAlphaModulation(): 44
	void GetColorModulation(float* r, float* g, float* b): 45
	*/

	void IncrementReferenceCount() {
		typedef void(__thiscall* oBlend)(PVOID);
		return getvfunc<oBlend>(this, 12)(this);
	}
	const char* GetName() {
		typedef const char* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 0)(this);
	}
	const char*     GetTextureGroupName() {
		typedef const char* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 1)(this);
	}
	void AlphaModulate(float alpha) {
		typedef void(__thiscall* OriginalFn)(PVOID, float);
		getvfunc<OriginalFn>(this, 27)(this, alpha);
	}
	void ColorModulate(float r, float g, float b) {
		typedef void(__thiscall* OriginalFn)(PVOID, float, float, float);
		getvfunc<OriginalFn>(this, 28)(this, r, g, b);
	}
	void SetVarFlag(eMaterialVarFlags flag, bool on) {
		typedef void(__thiscall* OriginalFn)(PVOID, eMaterialVarFlags, bool);
		getvfunc<OriginalFn>(this, 29)(this, flag, on);
	}
	void SetColorModulation(float r, float g, float b) {
		typedef void(__thiscall* OriginalFn)(PVOID, float, float, float);
		getvfunc<OriginalFn>(this, 28)(this, r, g, b);
	}
};

class IMaterialSystem {
public:
	/*
	void UncacheMaterials(): 65
	IMaterial* CreateMaterial(const char* pMaterialName, PVOID pVMTKeyValues): 82
	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL): 84
	bool IsMaterialLoaded(const char* pMaterialName): 72
	WORD FirstMaterial():73
	WORD NextMaterial(WORD h): 74
	WORD InvalidMaterial(): 75
	IMaterial* GetMaterial(WORD h): 76
	int GetNumMaterials(): 77
	*/

	IMaterial* CreateMaterial(const char* pMaterialName, PVOID pVMTKeyValues) {
		typedef IMaterial*(__thiscall* pBlend)(PVOID, const char*, PVOID);
		return getvfunc<pBlend>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) {
		typedef IMaterial*(__thiscall* pBlend)(PVOID, const char*, const char*, bool, const char*);
		return getvfunc<pBlend>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
};

class IVModelInfo {
public:
	/*
	void* GetModel(int Index): 1
	int GetModelIndex(const char* pModelName): 2
	const char* GetModelName(const void* model): 3
	studiohdr_t* GetStudioModel(const void* model): 30
	void GetModelMaterials(const void* model, int count, IMaterial** pMaterial): 17
	void SetVarFlag(eMaterialVarFlags flag, bool on): 29
	void SetColorModulation(const float* blend): 6*/

	void* GetModel(int Index) {
		typedef void*(__thiscall* pBlend)(PVOID, int);
		return getvfunc<pBlend>(this, 1)(this, Index);
	}

	int GetModelIndex(const char* pModelName) {
		typedef int(__thiscall* pBlend)(PVOID, const char*);
		return getvfunc<pBlend>(this, 2)(this, pModelName);
	}

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
	studiohdr_t *GetStudiomodel(const model_t *mod)
	{
		typedef studiohdr_t* (__thiscall* OriginalFn)(PVOID, const model_t*);
		return getvfunc<OriginalFn>(this, 30)(this, mod);
	}
};

class IVModelRender {
public:
	/*
	void ForcedMaterialOverride(IMaterial* mat, eOverrideType type = OVERRIDE_NORMAL, int unknown = NULL): 1
	bool IsForcedMaterialOverride(): 2
	*/

	void DrawModelExecute(const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld = NULL) {
		typedef void(__thiscall* OriginalFn)(PVOID, const DrawModelState_t, const ModelRenderInfo_t&, matrix3x4*);
		getvfunc<OriginalFn>(this, 21)(this, state, pInfo, pCustomBoneToWorld);
	}

	void ForcedMaterialOverride(IMaterial *newMaterial, eOverrideType nOverrideType = OVERRIDE_NORMAL, int unknown = NULL) {
		typedef void(__thiscall* Fn)(void*, IMaterial*, eOverrideType, int);
		getvfunc<Fn>(this, 1)(this, newMaterial, nOverrideType, unknown);
	}

	bool IsForcedMaterialOverride() {
		typedef bool(__thiscall* oForce)(PVOID);
		return getvfunc<oForce>(this, 2)(this);
	}

};

class IVRenderView {
public:
	/*
	void SetBlend(float blend): 4
	float GetBlend(): 5
	void SetColorModulation(const float* blend): 6
	void GetColorModulation(float* blend): 7*/

	float GetBlend() {
		typedef float(__thiscall* pBlend)(PVOID);
		return getvfunc<pBlend>(this, 5)(this);
	}

	void SetColorModulation(float  const* blend)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float  const*);
		getvfunc<OriginalFn>(this, 6)(this, blend);
	}
	void SetBlend(float blend)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float);
		getvfunc<OriginalFn>(this, 4)(this, blend);
	}

	void GetColorModulation(float* blend) {
		typedef void(__thiscall* pBlend)(PVOID, float*);
		return getvfunc<pBlend>(this, 7)(this, blend);
	}
};


class ConVar {
public:
	virtual void SetValue(const char *pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;
	virtual const char *GetName(void) const = 0;
	virtual bool IsFlagSet(int nFlag) const = 0;
};

class ICvar {
public:
	ConVar* FindVar(const char* var) {
		typedef ConVar*(__thiscall* pFind)(PVOID, const char*);
		return getvfunc<pFind>(this, 15)(this, var);
	}
};

class IClientEntityList {
public:
	virtual void Function0();															//GETVFUNC 0
	virtual void Function1();															//GETVFUNC 1
	virtual void Function2();															//GETVFUNC 2
	virtual C_BaseEntity *		GetClientEntity(int entnum);							//GETVFUNC 3
	virtual C_BaseEntity *		GetClientEntityFromHandle(ULONG hEnt) = 0;				//GETVFUNC 4
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;		//GETVFUNC 5
	virtual int					GetHighestEntityIndex(void);							//GETVFUNC 6
	virtual void				SetMaxEntities(int maxents);							//GETVFUNC 7
	virtual int					GetMaxEntities();										//GETVFUNC 8
};
#define CBaseEntity C_BaseEntity

typedef void(__thiscall* tPaintTraverse)(void* ecx, unsigned int, bool, bool);
typedef void*(*CreateInterface)(const char* name, int* iret);
typedef void(__stdcall* FrameStageNotify_t)(ClientFrameStage_t curStage);
bool __fastcall pCreateMove(void* self, int edx, float frametime, CUserCmd* pCmd);
typedef void(__thiscall* tDrawModelExecute)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)	// player jumping out of water
#define FL_ONTRAIN				(1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)	// Is a player
#define FL_FAKECLIENT			(1<<8)	// Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<9)	// In water
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
#define	IN_ATTACK3		(1 << 25)

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_RUNTIME_COMPOSITE				"Runtime Composite"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

#include "trace.h"