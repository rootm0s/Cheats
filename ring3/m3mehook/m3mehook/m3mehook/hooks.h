typedef void(__fastcall *PaintTraverseFn)(void*, void*, unsigned int, bool, bool);
PaintTraverseFn oPaintTraverse;
typedef bool(__stdcall *CreateMoveFn)(float, CUserCmd*);
CreateMoveFn oCreateMove;
typedef void(__stdcall *OverrideViewFn)(void*, CViewSetup*);
OverrideViewFn oOverrideView;

QAngle _angView;
QAngle angPunch;
CUserCmd* pUserCmd;
unsigned int drawPanel;
int iTest = 0;
bool bMeme = false;
bool bSilentFirst = false;
bool bAimbotting = false;
bool bAttack = false;
QAngle angOldPunch = QAngle(0, 0, 0);
QAngle angOldView;

void DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw)
{
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if (bDown && bRight)
		iWidth = iWidth + 1;

	Draw::DrawRect(iRealX, iY, iWidth, 1, colDraw);
	Draw::DrawRect(iX, iRealY, 1, iHeight, colDraw);

	Draw::DrawRect(iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color(0, 0, 0, 255));
	Draw::DrawRect(bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color(0, 0, 0, 255));
}

void __fastcall PaintTraverse(void* _this, void* _edx, unsigned int panel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(_this, _edx, panel, forceRepaint, allowForce);

	if (!drawPanel || drawPanel == NULL)
		if (strstr(Interfaces->VPanel->GetName(panel), "MatSystemTopPanel"))
			drawPanel = panel;

	if (panel != drawPanel)
		return;

	CBaseEntity* pLocalPlayer = (CBaseEntity*)Interfaces->ClientEntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
	Interfaces->Engine->GetScreenSize(iScreenWidth, iScreenHeight);

	if (Interfaces->Engine->IsConnected() && Interfaces->Engine->IsInGame() && Vars::Visuals::Enabled)
	{
		for (int iPlayerIndex = 0; iPlayerIndex < Interfaces->Engine->GetMaxClients(); iPlayerIndex++)
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces->ClientEntList->GetClientEntity(iPlayerIndex);

			if (!pEntity || pEntity == pLocalPlayer || iPlayerIndex == Interfaces->Engine->GetLocalPlayer() || 0 >= pEntity->health() || pEntity->m_bDormant)
				continue;

			if (Vars::Visuals::EnemyOnly && pEntity->team() == pLocalPlayer->team())
				continue;

			player_info_t pInfo;
			Interfaces->Engine->GetPlayerInfo(pEntity->index, &pInfo);

			Vector vecMax = pEntity->GetCollideable()->OBBMaxs();
			Vector vecHead, vecHead3D;
			Vector vecPos, vecPos3D;
			Vector vecTop, vecTop3D;
			vecHead3D = Aim->GetBonePosition(pEntity, 6);
			vecPos3D = pEntity->m_vecOrigin;
			vecTop3D = vecPos3D + Vector(0, 0, vecMax.z);
			if (!WorldToScreen(vecPos3D, vecPos, iScreenWidth, iScreenHeight) || !WorldToScreen(vecTop3D, vecTop, iScreenWidth, iScreenHeight) ||  !WorldToScreen(vecHead3D, vecHead, iScreenWidth, iScreenHeight))
				continue;

			bool bVisible = Aim->Trace(Aim->GetBonePosition(pLocalPlayer, 6), vecHead3D, pEntity);
			Color colDraw;

			if (pEntity->team() == 3)
				colDraw = bVisible ? Color(25, 200, 10) : Color(0, 75, 255, 255);
			else
				colDraw = bVisible ? Color(225, 100, 25, 255) : Color(255, 190, 70, 255);

			int iMiddle = (vecPos.y - vecTop.y) + 4;
			int iWidth = iMiddle / 3.5;

			// NAME
			if (Vars::Visuals::Name)
				Draw::DrawStringA(5, vecTop.x, vecTop.y - 8, Color(255, 255, 255, 255), pInfo.name);

			// BOUNDING BOX
			if (Vars::Visuals::Box)
			{
				if (Vars::Visuals::BoxType == 1)
				{
					Draw::DrawOutlinedRect(vecTop.x - iWidth, vecTop.y, iWidth * 2, iMiddle, colDraw);
					Draw::DrawOutlinedRect(vecTop.x - iWidth - 1, vecTop.y - 1, iWidth * 2 + 2, iMiddle + 2, Color(0, 0, 0, 255));
					Draw::DrawOutlinedRect(vecTop.x - iWidth + 1, vecTop.y + 1, iWidth * 2 - 2, iMiddle - 2, Color(0, 0, 0, 255));
				}
				else if (Vars::Visuals::BoxType == 2)
				{
					DrawCorner(vecTop.x - iWidth, vecTop.y, iWidth / 2, iWidth / 2, false, false, colDraw); // top left
					DrawCorner(vecTop.x - iWidth, vecTop.y + iMiddle, iWidth / 2, iWidth / 2, false, true, colDraw); // bottom left
					DrawCorner(vecTop.x + iWidth, vecTop.y, iWidth / 2, iWidth / 2, true, false, colDraw); // top right
					DrawCorner(vecTop.x + iWidth, vecTop.y + iMiddle, iWidth / 2, iWidth / 2, true, true, colDraw); // bottom right
				}
			}

			// HEALTH BAR
			if (Vars::Visuals::HealthBar)
			{
				int iClampedHealth = pEntity->health();
				if (iClampedHealth >= 101)
					iClampedHealth = 100;

				int iHealthG = iClampedHealth * 2.55;
				int iHealthR = 255 - iHealthG;
				int iHeight = (int)((float)iMiddle * 0.01f * (float)iClampedHealth);

				Draw::DrawRect(vecTop.x + iWidth + 3, vecTop.y, iWidth / 15, (int)((float)iMiddle * 0.01f * (float)iClampedHealth), Color(iHealthR, iHealthG, 0, 255));
				Draw::DrawOutlinedRect(vecTop.x + iWidth + 2, vecTop.y - 1, (iWidth / 15) + 2, iMiddle + 2, Color(0, 0, 0, 255));
			}

			// TRACE LINE
			if (Vars::Visuals::TraceLines)
				Draw::DrawLine(vecPos.x, vecPos.y, iScreenWidth / 2, iScreenHeight / 2, colDraw);
		}
	}

	if (bMenuOpen)
	{
		bMeme = true;
		HackMenu->Draw(600, 450, iScreenWidth, iScreenHeight);
	}
	else if (bMeme)
	{
		bMeme = false;
		Interfaces->Cvar->FindVar("cl_mouseenable")->SetValue(1);
	}

	//DrawMeme();
}

bool bMemeSwitch = false;

void DoAimbot(CUserCmd *cmd)
{
	bAimbotting = false;

	if (!Vars::Aimbot::Enabled)
		return;

	if (!GetAsyncKeyState(Vars::Aimbot::AimKey) && Vars::Aimbot::UseAimKey)
		return;

	Vector vecOrigin = pLocalPlayer->m_vecOrigin;
	Vector vecLocalPlayer = Aim->GetBonePosition(pLocalPlayer, 6);

	for (int iPlayerIndex = 0; iPlayerIndex < Interfaces->Engine->GetMaxClients(); iPlayerIndex++)
	{
		CBaseEntity* pEntity = (CBaseEntity*)Interfaces->ClientEntList->GetClientEntity(iPlayerIndex);

		if (!pEntity || pEntity == pLocalPlayer || iPlayerIndex == Interfaces->Engine->GetLocalPlayer() || 0 >= pEntity->health() || pEntity->m_bDormant)
			continue;

		if (Vars::Aimbot::IgnoreTeam && pEntity->team() == pLocalPlayer->team())
			continue;

		bool bImmune = *(bool*)((DWORD)pEntity + (DWORD)0x3890);

		if (bImmune)
			continue;

		Vector vecBone = Aim->GetBonePosition(pEntity, Vars::Aimbot::RealBone[Vars::Aimbot::Bone - 1]);
		vecBone = Aim->Predict(pEntity, vecBone);

		if (!Aim->Trace(vecLocalPlayer, vecBone, pEntity))
			continue;

		QAngle angAim;
		angAim = Aim->CalcAngle(pLocalPlayer->m_vecOrigin, vecBone);
		angAim = Aim->NormalizeAngle(angAim);

		if (Vars::Aimbot::FOV && Aim->AngleDifference(cmd->viewangles, angAim) > Vars::Aimbot::FOVMax)
			continue;

		bAimbotting = true;

		if (Vars::Aimbot::RCS)
		{
			angAim = angAim - (angPunch * (1.f + ((float)Vars::Aimbot::RCSAmt / 100.f)));
			angAim = Aim->NormalizeAngle(angAim);
		}

		if (Vars::Aimbot::Smooth)
		{
			angAim = Aim->Smooth(cmd->viewangles, angAim, (float)Vars::Aimbot::Smoothness);
			angAim = Aim->NormalizeAngle(angAim);
		}

		cmd->viewangles = Aim->NormalizeAngle(angAim);
		bAttack = !bAttack;

		if (Vars::Aimbot::AutoShoot && bAttack)
			cmd->buttons |= IN_ATTACK;

		if (Vars::Aimbot::AutoStop)
		{
			cmd->forwardmove = 0.0f;
			cmd->sidemove = 0.0f;
		}

		if (Vars::Aimbot::AutoCrouch)
			cmd->buttons |= IN_DUCK;

		break;
	}
}

void DoBhop(CUserCmd* cmd)
{
	if (!Vars::Misc::Bhop)
		return;

	if (cmd->buttons & IN_JUMP && !(pLocalPlayer->m_fFlags & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
}

void DoNoRecoil(CUserCmd* cmd)
{
	if (!Vars::Misc::RCS)
		return;

	int iShotsFired = *(int*)((DWORD)pLocalPlayer + (DWORD)0xA2A0);

	if (iShotsFired > 1)
	{
		QAngle angNoRecoil = cmd->viewangles;
		angNoRecoil += angOldPunch;
		angNoRecoil -= angPunch * 2;
		angNoRecoil = Aim->NormalizeAngle(angNoRecoil);

		cmd->viewangles = angNoRecoil;

		angOldPunch = angPunch * 2;
	}
	else
		angOldPunch = QAngle(0, 0, 0);
}

bool bNameUnlocked = false;
int iGarbage = 0;

void DoNameSteal(CUserCmd* cmd)
{
	if (!Vars::Misc::NameSteal && !Vars::Misc::NameStealAdv)
		return;

	static ConVar* pName = Interfaces->Cvar->FindVar("name");

	if (!bNameUnlocked)
	{
		pName->m_fnChangeCallbacks = NULL;
		bNameUnlocked = true;
	}

	bool bDidMeme = false;
	int iNameIndex = -1;
	iGarbage = iGarbage + 1;
	char chCommand[256];

	if (!Vars::Misc::NameStealAdv)
	{
		for (int iPlayerIndex = 0; iPlayerIndex < Interfaces->Engine->GetMaxClients(); iPlayerIndex++)
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces->ClientEntList->GetClientEntity(iPlayerIndex);

			if (!pEntity || pEntity == pLocalPlayer || iPlayerIndex == Interfaces->Engine->GetLocalPlayer() || pLocalPlayer->team() != pEntity->team())
				continue;

			if (rand() % 3 == 1)
			{
				iNameIndex = pEntity->index;
				bDidMeme = true;
			}
		}

		if (bDidMeme)
		{
			player_info_t pInfo;
			Interfaces->Engine->GetPlayerInfo(iNameIndex, &pInfo);
			sprintf(chCommand, "name \"%s  \"", pInfo.name);
		}
	}
	else
		sprintf(chCommand, "name \"m3mehook[%i]\"", rand() % 512);

	if (!bDidMeme && !Vars::Misc::NameStealAdv)
		return;

	if (iGarbage >= (Vars::Misc::NameStealDelay + (Vars::Misc::NameStealAdv ? 5 : 0)))
	{
		iGarbage = 0;
		Interfaces->Engine->ClientCmd(chCommand);
	}
}

bool __stdcall CreateMove(float flInputSampleTime, CUserCmd* cmd)
{
	oCreateMove(flInputSampleTime, cmd);

	if (cmd->command_number == 0)
		return true;

	void *vBase, *vInputBase;
	__asm mov vBase, ebp;
	vInputBase = (void*)vBase;
	bSendPacket = *(char**)vInputBase - 0x1C; // copypasted from moneybot
	*bSendPacket = TRUE;

	pLocalPlayer = (CBaseEntity*)Interfaces->ClientEntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
	pUserCmd = cmd;

	if (Vars::Aimbot::Smooth && Vars::Aimbot::Silent)
		Vars::Aimbot::Silent = false;

	angPunch = *(QAngle*)((DWORD)pLocalPlayer + (DWORD)0x3018);
	_angView = cmd->viewangles;

	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vForwardNorm, vRightNorm, vUpNorm;
	AngleVectors(cmd->viewangles, &viewforward, &viewright, &viewup);

	DoAimbot(cmd);
	DoBhop(cmd);
	DoNoRecoil(cmd);
	DoNameSteal(cmd);

	if (!bAimbotting)
	{
		HvH->AntiAim(cmd);
		HvH->AirStuck(cmd);
	}

	HvH->FakeLag(cmd);

	if (!Vars::HvH::AntiAim)
		cmd->viewangles = Aim->NormalizeAngle(cmd->viewangles);

	if (Vars::HvH::ForwardNaN)
		cmd->forwardmove = nanf("");
		
	if (Vars::Aimbot::Silent && bAimbotting)
	{
		AngleVectors(cmd->viewangles, &aimforward, &aimright, &aimup);
		HvH->Normalize(viewforward, vForwardNorm);
		HvH->Normalize(viewright, vRightNorm);
		HvH->Normalize(viewup, vUpNorm);
		cmd->forwardmove = DotProduct(forward* vForwardNorm, aimforward) + DotProduct(right* vRightNorm, aimforward) + DotProduct(up* vUpNorm, aimforward);
		cmd->sidemove = DotProduct(forward* vForwardNorm, aimright) + DotProduct(right* vRightNorm, aimright) + DotProduct(up* vUpNorm, aimright);
		cmd->upmove = DotProduct(forward* vForwardNorm, aimup) + DotProduct(right* vRightNorm, aimup) + DotProduct(up* vUpNorm, aimup);
		return false;
	}
	else if (Vars::HvH::AntiAim && !bAimbotting && !(cmd->buttons & IN_ATTACK))
		return false;

	return true;
}

void __stdcall OverrideView(CViewSetup* vsView)
{
	if (Vars::Visuals::Enabled && Interfaces->Engine->IsInGame())
	{
		if (Vars::Visuals::NoVisRecoil)
		{
			QAngle angNoRecoil = Aim->NormalizeAngle(angPunch);
			vsView->angles -= Vars::Misc::RCS ? -angNoRecoil : angNoRecoil;
		}

		vsView->fov += (float)Vars::Visuals::FOVAdd;
	}

	oOverrideView(Interfaces->ClientMode, vsView);
}