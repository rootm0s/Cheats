#include "main.h"
#include "menu.h"
#include "ESP.h"
#include "misc.h"
#include "hvh.h"
#include "aimbot.h"
#include "materials.h"

// Update: 1.0.8
// Changelog:
// Eftersom jag har arbetat med det här så jävla mycket själv
// så vill jag att du ska fixa:
// 1, Silent AntiAim
// 2, Aimbot Silent (Med correction)
// 3, Fakelag
// 4, Fixa crashes.

Vector AnglesToSet;
Vector firstAngles;
bool bOnce = false;

// Credits: Kiro, Kolo, ViKiNG, Liquid and Steady.
#define MAT( _TYPE_ ) "\"" + _TYPE_ + "\"\n{\n\t\"$basetexture\" \"VGUI/white_additive\"\n\t\"$envmap\" \"\"\n\t\"$model\" \"1\"\n\t\"$ignorez\" \"1\"\n\t\"$nofog\" \"1\"\n\t\"$halflambert\" \"1\"\n}"
#define MAT_IGNOREZ( _TYPE_ ) "\"" + _TYPE_ + "\"\n{\n\t\"$basetexture\" \"VGUI/white_additive\"\n\t\"$envmap\" \"\"\n\t\"$model\" \"1\"\n\t\"$ignorez\" \"0\"\n\t\"$nofog\" \"1\"\n\t\"$halflambert\" \"1\"\n}"

void GenerateRandomString(char *s, const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; i++)
	{
		s[i] = alphanum[rand() % (sizeof(alphanum)-1)];
	}

	s[len] = 0;
}

namespace Functions
{
	DWORD KeyValues_KeyValues;
	DWORD KeyValues_LoadFromBuffer;
	DWORD dwCalcPlayerView;
};
void InitValues() {
Functions::KeyValues_KeyValues = Utils::xFindPattern("client.dll", (PBYTE)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx");
Functions::KeyValues_KeyValues += 7;
Functions::KeyValues_KeyValues = Functions::KeyValues_KeyValues + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_KeyValues + 1) + 5;

Functions::KeyValues_LoadFromBuffer = Utils::xFindPattern("client.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x8A\xD8\xFF\x15\x00\x00\x00\x00\x84\xDB", "x????xxxx????xx");
Functions::KeyValues_LoadFromBuffer = Functions::KeyValues_LoadFromBuffer + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_LoadFromBuffer + 1) + 5;
}

void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Functions::KeyValues_KeyValues;
	__asm
	{
		push name
			mov ecx, keyValues
			call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer)
{
	DWORD dwFunction = (DWORD)Functions::KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
			push 0
			push 0
			push pBuffer
			push resourceName
			mov ecx, keyValues
			call dwFunction
	}
}

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe) //credits to ph0ne
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
				\n{\
						\n\t\"$basetexture\" \"vgui/white_additive\"\
								\n\t\"$envmap\" \"\"\
										\n\t\"$model\" \"1\"\
												\n\t\"$flat\" \"1\"\
														\n\t\"$nocull\" \"0\"\
																\n\t\"$selfillum\" \"1\"\
																		\n\t\"$halflambert\" \"1\"\
																				\n\t\"$nofog\" \"0\"\
																						\n\t\"$ignorez\" \"%i\"\
																								\n\t\"$znearer\" \"0\"\
																										\n\t\"$wireframe\" \"%i\"\
																										        \n}\n"
	};

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#ayy_meme_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = pMatSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};

		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;


		float alpha = color.a();

		pRenderView->SetBlend(1.0f);
		pRenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		pModelRender->ForcedMaterialOverride(material);
	else
		pModelRender->ForcedMaterialOverride(NULL);

}

C_BaseCombatWeapon* C_BaseEntity::GetActive() {
	ULONG WeaponUlong = *(PULONG)((DWORD)this + 0x2EE8); // hActiveWeapon
	return (C_BaseCombatWeapon*)(pEntList->GetClientEntityFromHandle(WeaponUlong));
}
static bool lOnce = false;
bool cOnce = false, twice = false, vcheck1 = false, vcheck2 = false;
void __fastcall hkPaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
	PanelHook->Function<tPaintTraverse>(41)(ecx, vguiPanel, forceRepaint, allowForce);

	const char* pszPanelName = pPanel->GetName(vguiPanel);
	bool isValidPanel = false;
	static bool xOnce = false;

	if (pszPanelName && pszPanelName[0] == 'M' && pszPanelName[3] == 'S' &&
		pszPanelName[9] == 'T' && pszPanelName[12] == 'P')
		isValidPanel = true;

	if (isValidPanel)
	{
		if (!cOnce) {
			pEngine->GetScreenSize(width, height); // Lägg till denna.
			pFont = pSurface->CreateFontA();
			pSurface->SetFontGlyphSet(pFont, "Tahoma", 12, 800, 0, 0, FONTFLAG_OUTLINE); //MENU font, FONTFLAG_OUTLINE = outlined text :3
			aim_fov.setValue(90);
			aim_bone.setValue(6);
			misc_viewfov.setValue(90);
			initMenu();
			cOnce = true;

		}	
		if (vis_active.getValve() == true) {
			RenderESP(vis_active, vis_esp_box, vis_esp_name, vis_esp_healthbar, vis_activeweapon, vis_skeleton);
		}
		//char szMaterialName[128];
		//GenerateRandomString(szMaterialName, 17);
		//strcat(szMaterialName, ".vmt");
		//DrawString(10, 10, Color(255, 255, 255, 255), pFont, szMaterialName);
		// Det där ^ var ett test och see vad som sparas kek.

		DrawMenu();
		UpdateMenu();
		GetCursorPos(&Cursor);
		if (MenuOpend) {
			if (pEngine->IsConnected() && pEngine->IsInGame()) {
				DrawpCursor(Cursor.x, Cursor.y);
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 1) {
			onMouseClick();
		}
		if (GetAsyncKeyState(VK_LBUTTON)) {
			onMouseHold();
		}

	}
}

// Crc

#define CRC32_INIT_VALUE 0xFFFFFFFFUL
#define CRC32_XOR_VALUE  0xFFFFFFFFUL
#define LittleLong( val )			( val )
#define NUM_BYTES 256
static const CRC32_t pulCRCTable[NUM_BYTES] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
	0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
	0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
	0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
	0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
	0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
	0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
	0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
	0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
	0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
	0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
	0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
	0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
	0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
	0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

void CRC32_Init(CRC32_t *pulCRC)
{
	*pulCRC = CRC32_INIT_VALUE;
}

void CRC32_Final(CRC32_t *pulCRC)
{
	*pulCRC ^= CRC32_XOR_VALUE;
}

CRC32_t	CRC32_GetTableEntry(unsigned int slot)
{
	return pulCRCTable[(unsigned char)slot];
}

void CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *pBuffer, int nBuffer)
{
	CRC32_t ulCrc = *pulCRC;
	unsigned char *pb = (unsigned char *)pBuffer;
	unsigned int nFront;
	int nMain;

JustAfew:

	switch (nBuffer)
	{
	case 7:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 6:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 5:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 4:
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		*pulCRC = ulCrc;
		return;

	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 1:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 0:
		*pulCRC = ulCrc;
		return;
	}

	// We may need to do some alignment work up front, and at the end, so that
	// the main loop is aligned and only has to worry about 8 byte at a time.
	//
	// The low-order two bits of pb and nBuffer in total control the
	// upfront work.
	//
	nFront = ((unsigned int)pb) & 3;
	nBuffer -= nFront;
	switch (nFront)
	{
	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 1:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	}

	nMain = nBuffer >> 3;
	while (nMain--)
	{
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc ^= LittleLong(*(CRC32_t *)(pb + 4));
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		pb += 8;
	}

	nBuffer &= 7;
	goto JustAfew;
}

/*void __stdcall HookedCMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket) {
	//oCreateMove(sequence_number, input_sample_frametime, active);
	ClientHooked->Function<tCreateMove>(21)(sequence_number, input_sample_frametime, active);
	CUserCmd* Cmdlist = *(CUserCmd**)((DWORD)pInput + 0xEC);
	CUserCmd* cmd = &Cmdlist[sequence_number % 150];

	C_BaseEntity* Local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = Local->GetActive();
	if (!Local)
		return;

	Vector oldView = cmd->viewangles;
	float oldForwardmove = cmd->forwardmove;
	float oldSidemove = cmd->sidemove;

	if (pEngine->IsInGame() && pEngine->IsConnected()) //IsConnected & IsInGame Kollar om du är in-game. 
	{
		if (Local->GetLifeState() == 0) {
			//BYTE* bSendIt = (BYTE*)((DWORD)pEngine + 0xF6887);
			if ((hvh_x.getValue() > 0 || hvh_y.getValue() > 0)) {
			firstAngles = cmd->viewangles;
			AnglesToSet = firstAngles;

			//fix movement
			Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
			float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
			Vector vMove2;
			Vector vRealView(cmd->viewangles);
			VectorAngles3D(vMove, vMove2);



			float flServerTime = (float)*(int*)((DWORD)Local + 0x3410) * pGlobals->interval_per_tick; // Enligt mina beräkningar, så ska det här
			float flNextPrimaryAttack = *(float*)((DWORD)pWeapon + 0x31C8); // tydligen göra alla våra antiaims till silent antiaims.

			bool bBulletTime = true;
			if (flNextPrimaryAttack > flServerTime)
				bBulletTime = false;

			if (cmd->buttons & IN_ATTACK && bBulletTime) {
				bSendPacket = false;
			}
			else {
				bSendPacket = true;
				// Do anti-aim here.
				DoAntiAim(cmd, hvh_x, hvh_y); // do the fucking anti aim bitch
			}


			flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
			cmd->forwardmove = cos(flYaw) * flSpeed;
			cmd->sidemove = sin(flYaw) * flSpeed;
			ClampAngles(cmd->viewangles);
			NormalizeVector(cmd->viewangles);
			}
			doMisc(cmd, Local, AnglesToSet, misc_active.getValve(), misc_bunnyhop.getValve(), misc_autostrafe.getValve(), misc_fakelag.getValve(), misc_fakelag_factor.getValue(), misc_norecoil.getValve(), misc_nospread.getValve(), misc_force_nospread.getValve(), misc_teleport.getValve(), aim_active.getValve(), bSendPacket);
			}
	}

	CVerifiedUserCmd* verifiedlist = *(CVerifiedUserCmd**)((DWORD)pInput + 0xF0);
	CVerifiedUserCmd* verified = &verifiedlist[sequence_number % 150];
	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();

}*/

void SinCos(float a, float* s, float*c) {
	*s = sin(a);
	*c = cos(a);
}

void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0){
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

/* cmove hook function */
bool __stdcall HookedCreateMove(float SampleTime, CUserCmd* pCmd)
{
	if (!pCmd->CommandNumber)
		return true;

	C_BaseEntity* pLocal = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal)
		return false;

	if (pLocal->Health() < 1)
		return false;

	if (!pLocal->GetLifeState() == 0)
		return false;

	// Backup for safety
	Vector origView = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSidemove = pCmd->sidemove;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	Vector qAimAngles;
	qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

	/* code goes here */
	if (pEngine->IsInGame() && pEngine->IsConnected()) {
		//Movement Fix
		
		//get old views sidemovs etc.
		Vector vMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		Vector vRealView(pCmd->viewangles);
		VectorAngles3D(vMove, vMove2);
	
		DoAntiAim(pCmd, hvh_x, hvh_y);
		NormalizeVector(pCmd->viewangles);
		ClampAngles(pCmd->viewangles);

		doAim(pCmd, aim_active.getValve(), aim_silent.getValve(), aim_autoshoot.getValve(), aim_psilent.getValve());
		doMisc(pCmd, pLocal, AnglesToSet, misc_active.getValve(), misc_bunnyhop.getValve(), misc_autostrafe.getValve(), misc_fakelag.getValve(), misc_fakelag_factor.getValue(), misc_norecoil.getValve(), misc_nospread.getValve(), misc_force_nospread.getValve(), misc_teleport.getValve(), aim_active.getValve());
		flYaw = DEG2RAD(pCmd->viewangles.y - vRealView.y + vMove2.y);
		
		//fix movement
		pCmd->forwardmove = cos(flYaw) * flSpeed;
		pCmd->sidemove = sin(flYaw) * flSpeed;
		ClampAngles(pCmd->viewangles);
		NormalizeVector(pCmd->viewangles);
	}

	return false;
}

void __stdcall FrameStageHook(ClientFrameStage_t curStage) {

	C_BaseEntity* gLocal = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
	//C_BaseCombatWeapon* gWeapon = gLocal->GetActive();
	if (pEngine->IsInGame() && pEngine->IsConnected() && gLocal)
	{

		Vector *PunchAngle, OldPunchAngle;
		Vector *PunchVisualAngle, OldVisualAngle;

		PunchAngle = (Vector*)((DWORD)gLocal + 0x3018);
		PunchVisualAngle = (Vector*)((DWORD)gLocal + 0x300C);
		if (misc_active.getValve()) {
			if (misc_novisualrecoil.getValve()) {
				if (curStage == FRAME_RENDER_START)
				{
					if (gLocal) {
						if (gLocal->Health() > 0) {
							if (gLocal->GetLifeState() == 0) {
								//	if (!gWeapon->IsKnife() || gWeapon->IsBomb()) {
								if (PunchAngle->x != 0 || PunchAngle->y != 0 || PunchAngle->z != 0 || PunchVisualAngle->x != 0 || PunchVisualAngle->y != 0 || PunchVisualAngle->z != 0)
								{

									OldPunchAngle = *PunchAngle;
									OldVisualAngle = *PunchVisualAngle;
									*PunchAngle = Vector(0, 0, 0);
									*PunchVisualAngle = Vector(0, 0, 0);
								}
								//}
							}
						}
					}
				}
			}

		}
		ClientHooked->Function<FrameStageNotify_t>(36)(curStage);
		if (misc_active.getValve()) {
			if (misc_novisualrecoil.getValve()) {
				if (curStage == FRAME_RENDER_START)
				{

					if (gLocal) {
						if (gLocal->Health() > 0) {
							if (gLocal->GetLifeState() == 0) {
								//	if (!gWeapon->IsKnife() || gWeapon->IsBomb()) {
								if (PunchAngle->x == 0 || PunchAngle->y == 0 || PunchAngle->z == 0 || PunchVisualAngle->x == 0 || PunchVisualAngle->y == 0 || PunchVisualAngle->z == 0)
								{

									*PunchAngle = OldPunchAngle;
									*PunchVisualAngle = OldVisualAngle;
								}
								//	}
							}
						}
					}
				}
			}
		}
	}
}

void Converter(float r, float g, float b, float a) {
	r = (1 / 255.f)*r;
	g = (1 / 255.f)*g;
	b = (1 / 255.f)*b;
	a = (1 / 255.f)*a;

}

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true, true, false);
	static IMaterial* OpenLit = CreateMaterial(false, true, false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false, false);
	bool DontDraw = false;

	const char* ModelName = pModelInfo->GetModelName((model_t*)pInfo.pModel);
	C_BaseEntity* pModelEntity = (C_BaseEntity*)pEntList->GetClientEntity(pInfo.entity_index);
	C_BaseEntity* pLocal = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());

	if (vis_active.getValve())
	{
		// Player Chams
		int ChamsStyle = 1;
		if (vis_chams.getValve() && strstr(ModelName, "models/player"))
		{
			if (pLocal && (!pModelEntity->Team() != pLocal->Team()))
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				C_BaseEntity* pModelEntity = (C_BaseEntity*)pEntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					C_BaseEntity *local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->Health() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
						{
							float alpha = 1.f;

							if (pModelEntity->Team() == 2)
							{
								flColor[0] = 240.f / 255.f;
								flColor[1] = 30.f / 255.f;
								flColor[2] = 35.f / 255.f;
							}
							else
							{
								flColor[0] = 63.f / 255.f;
								flColor[1] = 72.f / 255.f;
								flColor[2] = 205.f / 255.f;
							}

							pRenderView->SetColorModulation(flColor);
							pRenderView->SetBlend(alpha);
							pModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pModelEntity->Team() == 2)
							{
								flColor[0] = 247.f / 255.f;
								flColor[1] = 180.f / 255.f;
								flColor[2] = 20.f / 255.f;
							}
							else
							{
								flColor[0] = 32.f / 255.f;
								flColor[1] = 180.f / 255.f;
								flColor[2] = 57.f / 255.f;
							}

							pRenderView->SetColorModulation(flColor);
							pRenderView->SetBlend(alpha);
							pModelRender->ForcedMaterialOverride(open);
						}
						else
						{
							color.SetColor(255, 255, 255, 255);
							ForceMaterial(color, open, true, true);
						}
					}
				}
			}
		}
	}
	oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	pModelRender->ForcedMaterialOverride(NULL);
}

void* GetInterfacePtr(const char* interfaceName, const char* ptrName, CreateInterface pInterface)
{
	char szDebugString[1024];
	std::string sinterface = "";
	std::string interfaceVersion = "0";

	for (int i = 0; i <= 99; i++)
	{
		sinterface = interfaceName;
		sinterface += interfaceVersion;
		sinterface += std::to_string(i);

		void* funcPtr = pInterface(sinterface.c_str(), NULL);

		if ((DWORD)funcPtr != 0x0)
		{
			sprintf(szDebugString, "%s: 0x%x (%s%s%i)", ptrName, (DWORD)funcPtr, interfaceName, interfaceVersion.c_str(), i);
			return funcPtr;
		}
		if (i >= 99 && interfaceVersion == "0")
		{
			interfaceVersion = "00";
			i = 0;
		}
		else if (i >= 99 && interfaceVersion == "00")
		{
			sprintf(szDebugString, "%s: 0x%x (error)", ptrName, (DWORD)funcPtr);
		}
	}
	return 0;
}


DWORD WINAPI Start(LPVOID lpArguments) {
	CreateInterface engine = (CreateInterface)GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface");
	CreateInterface client = (CreateInterface)GetProcAddress(GetModuleHandleA("client.dll"), "CreateInterface");
	CreateInterface vgui = (CreateInterface)GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface");
	CreateInterface surface = (CreateInterface)GetProcAddress(GetModuleHandleA("vguimatsurface.dll"), "CreateInterface");
	CreateInterface matsystem = (CreateInterface)GetProcAddress(GetModuleHandleA("materialsystem.dll"), "CreateInterface");
	CreateInterface vstbase = (CreateInterface)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "CreateInterface");

	// Interfaces.
	pPanel = (IPanel*)vgui("VGUI_Panel009", NULL);
	pSurface = (ISurface*)surface("VGUI_Surface031", NULL);
	pEngine = (IVEngineClient*)engine("VEngineClient013", NULL);
	pClient = (IBaseClientDll*)client("VClient017", NULL);
	pEntList = (IClientEntityList*)client("VClientEntityList003", NULL);
	pEngineTrace = (IEngineTrace*)engine("EngineTraceClient004", NULL);
	pModelRender = (IVModelRender*)GetInterfacePtr("VEngineModel", "pModelRender", engine);
	pModelInfo = (IVModelInfo*)GetInterfacePtr("VModelInfoClient", "pModelInfo", engine);
	pRenderView = (IVRenderView*)GetInterfacePtr("VEngineRenderView", "pRenderView", engine);
	pMatSystem = (IMaterialSystem*)GetInterfacePtr("VMaterialSystem", "pMatSystem", matsystem);
	pCvar = (ICvar*)GetInterfacePtr("VEngineCvar", "pCvar", vstbase);
	InitValues();
	PanelHook = new CHook((DWORD**)pPanel);
	PanelHook->dwHookMethod((DWORD)hkPaintTraverse, 41);

	pModelHook = new CHook((DWORD**)pModelRender);
	oDrawModelExecute = (tDrawModelExecute)pModelHook->dwHookMethod((DWORD)hkDrawModelExecute, 21);

	ClientHooked = new CHook((DWORD**)pClient);
	ClientHooked->dwHookMethod((DWORD)FrameStageHook, 36);

	void** BaseClientDllVMT = *(void***)pClient;
	ClientMode = *(IBaseClientDll***)((DWORD)BaseClientDllVMT[10] + 5);
	CreateMoveHook = new CHook(*(DWORD***)ClientMode);
	_CreateMove = (CreateMove)CreateMoveHook->dwHookMethod((DWORD)HookedCreateMove, 24);

	PDWORD pdwClient = (PDWORD)*(PDWORD)pClient;
	DWORD dwInitAddr = (DWORD)(pdwClient[0]);
	for (DWORD dwIter = 0; dwIter <= 0xFF; dwIter++) {
		if (*(PBYTE)(dwInitAddr + dwIter) == 0xA3) {
			pGlobals = (CGlobalVarsBase*)*(PDWORD)*(PDWORD)(dwInitAddr + dwIter + 1);

			break;
		}
	}
	return false;
}



bool DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL);
	}
	return true;
}