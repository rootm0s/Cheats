#include "main.h"

Base::Utils::CVMTHookManager g_pFrameStageNotify;

void Hooked_RecvProxy_Viewmodel(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	if ((*(int*)((DWORD)pData + 0x8) == g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_default_t.mdl")) || *(int*)((DWORD)pData + 0x8) == g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_default_ct.mdl"))
	{
		if (g_pConsole->getValue("misc_knifeskinchanger") == 1)
		{
			if (g_pConsole->getValue("misc_knifemodel") == 0)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_default_t.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 1)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 2)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 3)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_flip.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 4)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_gut.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 5)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_karam.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 6)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 7)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_tactical.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 8)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
			if (g_pConsole->getValue("misc_knifemodel") == 9)
				*(int*)((DWORD)pData + 0x8) = g_Valve.g_pModel->GetModelIndex("models/weapons/v_knife_gg.mdl");
		}
	}
	*(int*)((DWORD)pOut) = *(int*)((DWORD)pData + 0x8);
}

void __stdcall new_FrameStageNotify(ClientFrameStage_t curStage)
{
	static DWORD dwFrameStage = g_pFrameStageNotify.dwGetMethodAddress(36);

	_asm
	{
		PUSH curStage;
		CALL dwFrameStage;
	}

	//Vector vAimPunch, vViewPunch;
	//static bool recoilcheck = false;

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START && g_Valve.pEngine->GetLocalPlayer() > 0)
	{
		if (g_Valve.pEngine->IsInGame())
		{
			CBaseEntity* pLocal = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());
			if (pLocal)
			{
				ValveSDK::CBaseCombatWeapon* pWeapon = pLocal->GetActiveBaseCombatWeapon();
				if (pWeapon)
				{
					if ((GetAsyncKeyState(VK_END) & 1))
					{
						static PVOID pClientState = **(PVOID**)(Base::Utils::PatternSearch("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx", NULL, 0x438000) + 2);

						static DWORD dwAddr1 = Base::Utils::PatternSearch("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????", NULL, 0x438000);
						static DWORD dwRelAddr = *(DWORD*)(dwAddr1 + 1);
						static DWORD sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

						__asm
						{
							pushad
								mov edi, pClientState
								lea ecx, dword ptr[edi + 0x8]
								call sub_B5E60
								mov dword ptr[edi + 0x154], 0xFFFFFFFF //m_nDeltaTick
								popad

						}
					}

					if (g_pConsole->getValue("misc_fov") != 90)
					{
						pLocal->SetFov(g_pConsole->getValue("misc_fov"));
					}
					/*
					if (g_pConsole->getValue("novisrecoil") == 1)
					{
						static int iOffset2 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_viewPunchAngle");
						static int iOffset3 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_aimPunchAngle");
						vViewPunch = *(Vector*)((DWORD)pLocal + iOffset2);
						vAimPunch = *(Vector*)((DWORD)pLocal + iOffset3);
						*(Vector*)((DWORD)pLocal + iOffset2) = Vector(0.0f, 0.0f, 0.0f);
						*(Vector*)((DWORD)pLocal + iOffset3) = Vector(0.0f, 0.0f, 0.0f);
						recoilcheck = true;
					}
					*/
					if (g_pConsole->getValue("misc_skinchanger") == 1)
					{
						*(PINT)((DWORD)pWeapon + 0x14F0) = 0;
						*(PINT)((DWORD)pWeapon + 0x14F4) = 0; //owner
						*(PINT)((DWORD)pWeapon + 0x14FC) = 1;

						if (pWeapon->GetWeaponID() == 1) //DEAGLE
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_deagle");
						else if (pWeapon->GetWeaponID() == 2) //DUALS
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_duals");
						else if (pWeapon->GetWeaponID() == 3) //FIVESEVEN
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_fiveseven");
						else if (pWeapon->GetWeaponID() == 4) //GLOCK
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_glock");
						else if (pWeapon->GetWeaponID() == 61) //USP
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_usp");
						else if (pWeapon->GetWeaponID() == 63) //CZ
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_cz");
						else if (pWeapon->GetWeaponID() == 30) //TEC9
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_tec9");
						else if (pWeapon->GetWeaponID() == 32) //P2000
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_p2000");
						else if (pWeapon->GetWeaponID() == 36) //p250
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_p250");
						/*
						else if (g_pMisc.bIsUsingShotgun()) //SHOTGUN
							*(PINT)((DWORD)pWeapon + 0x14F8) = gCvars::shotgun.Get();

						else if (g_pMisc.bIsUsingMP()) //MP
							*(PINT)((DWORD)pWeapon + 0x14F8) = gCvars::mp.Get();
						*/
						else if (pWeapon->GetWeaponID() == 13) //GALIL
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_galil");
						else if (pWeapon->GetWeaponID() == 10) //FAMAS
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_famas");
						else if (pWeapon->GetWeaponID() == 7) //AK47
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_ak47");
						else if (pWeapon->GetWeaponID() == 16) //M4A1
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_m4a1");
						else if (pWeapon->GetWeaponID() == 60) //M4A1S
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_m4a1s");
						else if (pWeapon->GetWeaponID() == 8) //AUG
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_aug");
						else if (pWeapon->GetWeaponID() == 39) //KRIEG
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_krieg");

						else if (pWeapon->GetWeaponID() == 40) //SCOUT
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_scout");
						else if (pWeapon->GetWeaponID() == 9) //AWP
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_awp");
						else if (pWeapon->GetWeaponID() == 11) //G3SG1
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_g3sg1");
						else if (pWeapon->GetWeaponID() == 38) //SCAR20
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_scar20");

						else if (pWeapon->GetWeaponID() == 14) //M249
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_m249");
						else if (pWeapon->GetWeaponID() == 28) //NEGEV
							*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_negev");

						*(PFLOAT)((DWORD)pWeapon + 0x1500) = 0.99f - (98 / 100.0f);
						*(PINT)((DWORD)pWeapon + 0x1504) = (false ? 1337 : -1);
						*(PINT)((DWORD)pWeapon + 0x1330) = 1; //account id
						*(PINT)((DWORD)pWeapon + 0x1320) = 0; //geniue
					}

					if (pLocal->GetActiveBaseCombatWeapon()->IsKnife() && g_pConsole->getValue("misc_knifeskinchanger") == 1)
					{
						*(PINT)((DWORD)pWeapon + 0x14F8) = g_pConsole->getValue("skin_knife");
						*(PFLOAT)((DWORD)pWeapon + 0x1500) = 0.99f - (98 / 100.0f);
						*(PINT)((DWORD)pWeapon + 0x1504) = (true ? 1337 : -1);
						*(PINT)((DWORD)pWeapon + 0x1330) = 1; //account id
						*(PINT)((DWORD)pWeapon + 0x1320) = 0; //geniue
					}
				}
			}
		}

		CBaseEntity* pLocal = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());
		if (pLocal)
		{
			if (pLocal->GetLifeState() == LIFE_ALIVE)
			{
				ValveSDK::CBaseCombatWeapon* pWeapon = pWeapon = pLocal->GetActiveBaseCombatWeapon();
				if (pWeapon)
				{
					ValveSDK::ClientClass *pClass = g_Valve.pClient->GetAllClasses();
					while (pClass)
					{
						const char *pszName = pClass->GetTable()->GetName();
						if (!strcmp(pszName, "DT_BaseViewModel"))
						{
							for (int i = 0; i < pClass->GetTable()->GetNumProps(); i++)
							{
								RecvProp *pProp = pClass->GetTable()->GetPropA(i);

								if (!strcmp(pProp->GetName(), "m_nModelIndex"))
								{
									if (pLocal->GetActiveBaseCombatWeapon()->IsKnife() && g_pConsole->getValue("misc_knifeskinchanger") == 1)
									{
										pProp->SetProxyFn(Hooked_RecvProxy_Viewmodel);
										if (g_pConsole->getValue("misc_knifemodel") == 2)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 500;
										if (g_pConsole->getValue("misc_knifemodel") == 3)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 505;
										if (g_pConsole->getValue("misc_knifemodel") == 4)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 506;
										if (g_pConsole->getValue("misc_knifemodel") == 5)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 507;
										if (g_pConsole->getValue("misc_knifemodel") == 6)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 508;
										if (g_pConsole->getValue("misc_knifemodel") == 7)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 509;
										if (g_pConsole->getValue("misc_knifemodel") == 8)
											*(int*)((DWORD)pWeapon + 0x1148 + 0x40 + 0x194) = 515;
									}
								}
							}
						}
						pClass = pClass->NextClass();
					}
				}
			}
		}
	}
	/*
	if (recoilcheck)
	{
		static int iOffset2 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_viewPunchAngle");
		static int iOffset3 = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_aimPunchAngle");
		*(Vector*)((DWORD)g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer()) + iOffset2) = vViewPunch;
		*(Vector*)((DWORD)g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer()) + iOffset3) = vAimPunch;
	}
	recoilcheck = false;
	*/
}