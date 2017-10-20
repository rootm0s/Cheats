class CMenu
{
public:
	void Draw(int, int, int, int);
private:
	int iMouseX, iMouseY, iWidth, iHeight, iScreenWidth, iScreenHeight, iTab = 1;
	bool bClicked, bMouseD = false;
	void GetMousePos();
	void GetClicked();
	bool Holding(int, int, int, int);
	bool Hovering(int, int, int, int);
	int MouseXPos(int, int, int, int);
	bool Clicked(int, int, int, int);
	void DrawRectM(int, int, int, int, Color);
	void DrawOutlinedRectM(int, int, int, int, Color);
	void DrawStringM(HFont, int, int, Color, const char*);
	void DrawStringM2(HFont, int, int, Color, const char*);
	void DrawSlider(const char*, int, int, int, int, int, int &);
	void DrawButton(const char*, int, int, int, bool &);
	void DrawDropdown(const char*, int, int, int, int, const char**, int, bool &, bool &, int &);
	void DrawTab(const char*, int);
	void DrawBackground();
	void DrawClose();
	void DrawCursor();
	void DrawButtons();
}; CMenu *HackMenu = new CMenu;

void CMenu::Draw(int iMenuWidth, int iMenuHeight, int iSWidth, int iSHeight)
{
	iWidth = iMenuWidth;
	iHeight = iMenuHeight;
	iScreenWidth = iSWidth;
	iScreenHeight = iSHeight;

	Interfaces->Cvar->FindVar("cl_mouseenable")->SetValue(0);
	GetMousePos();
	GetClicked();
	DrawBackground();
	DrawButtons();
	DrawCursor();
}

void CMenu::GetMousePos()
{
	POINT ptMouse;

	if (GetCursorPos(&ptMouse))
		if (ScreenToClient(hwCSGO, &ptMouse))
		{
			iMouseX = clamp(ptMouse.x, 2, iScreenWidth - 2);
			iMouseY = clamp(ptMouse.y, 2, iScreenHeight - 2);
		}
}

void CMenu::GetClicked()
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		bClicked = false;
		bMouseD = true;
	}
	else
		if (bMouseD)
		{
			bClicked = true;
			bMouseD = false;
		}
		else
			bClicked = false;
}

bool CMenu::Holding(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	if (GetAsyncKeyState(VK_LBUTTON))
		if (Hovering(iXStart, iYStart, iXEnd, iYEnd))
			return true;

	return false;
}

bool CMenu::Hovering(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;
	int iRealXEnd = iRealXStart + iXEnd;
	int iRealYEnd = iRealYStart + iYEnd;

	if (iMouseX >= iRealXStart && iRealXEnd >= iMouseX)
		if (iMouseY >= iRealYStart && iRealYEnd >= iMouseY)
			return true;

	return false;
}

int CMenu::MouseXPos(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	return iMouseX - iRealXStart;
}

bool CMenu::Clicked(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	if (bClicked)
		if (Hovering(iXStart, iYStart, iXEnd, iYEnd))
			return true;

	return false;
}

void CMenu::DrawRectM(int iXStart, int iYStart, int iRWidth, int iRHeight, Color colDrawColor)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;

	Draw::DrawRect(iRealXStart, iRealYStart, iRWidth, iRHeight, colDrawColor);
}

void CMenu::DrawOutlinedRectM(int iXStart, int iYStart, int iRWidth, int iRHeight, Color colDrawColor)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;

	Draw::DrawOutlinedRect(iRealXStart, iRealYStart, iRWidth, iRHeight, colDrawColor);
}

void CMenu::DrawStringM(HFont hfFont, int iX, int iY, Color colDrawColor, const char* msg)
{
	int iRealX = ((iScreenWidth / 2) - (iWidth / 2)) + iX;
	int iRealY = ((iScreenHeight / 2) - (iHeight / 2)) + iY;

	Draw::DrawStringA(hfFont, iRealX, iRealY, colDrawColor, msg);
}

void CMenu::DrawStringM2(HFont hfFont, int iX, int iY, Color colDrawColor, const char* msg)
{
	int iRealX = ((iScreenWidth / 2) - (iWidth / 2)) + iX;
	int iRealY = ((iScreenHeight / 2) - (iHeight / 2)) + iY;

	Draw::DrawStringB(hfFont, iRealX, iRealY, colDrawColor, msg);
}

void CMenu::DrawButton(const char* strText, int iTabIndex, int iX, int iY, bool &bVar)
{
	if (iTab == iTabIndex)
	{
		if (!bVar)
			DrawOutlinedRectM(iX, iY, 20, 20, Color(25, 175, 220, 255));
		else
			DrawRectM(iX, iY, 20, 20, Color(25, 175, 220, 255));

		DrawStringM2(7, iX + 28, iY + 10, Color(255, 255, 255, 255), strText);

		if (Clicked(iX, iY, 20, 20))
		{
			bVar = !bVar;
			pConfig->Save();
		}
	}
}

void CMenu::DrawSlider(const char* strText, int iTabIndex, int iX, int iY, int iWidth, int iMax, int &iVar)
{
	if (iTab == iTabIndex)
	{
		DrawOutlinedRectM(iX, iY, iWidth, 20, Color(25, 175, 220, 255));
		DrawRectM(iX, iY, iVar * ((float)iWidth / (float)(iMax)), 20, Color(25, 175, 220, 255));

		std::string s = std::to_string(iVar);
		char const *pchar = s.c_str();

		DrawStringM2(7, iX + 5, iY + 9, Color(255, 255, 255, 255), strText);
		DrawStringM2(7, iX + ((float)iWidth / (float)2) - 3, iY + 10, Color(255, 255, 255, 255), pchar);

		if (Holding(iX, iY, iWidth, 20))
		{
			iVar = MouseXPos(iX, iY, iWidth, 20) / ((float)iWidth / (float)(iMax));
			pConfig->Save();
		}
	}
}

void CMenu::DrawDropdown(const char* strText, int iTabIndex, int iX, int iY, int iAdd, const char** chTable, int iAmount, bool &bHolding, bool &bOpen, int &iVar)
{
	if (iTab == iTabIndex)
	{
		if (Holding(iX - 50, iY, 100, iAdd) && !bHolding)
		{
			bOpen = !bOpen;
			bHolding = true;
		}
		else if (!Holding(iX - 50, iY, 100, iAdd))
			bHolding = false;

		DrawOutlinedRectM(iX - 50, iY, 100, iAdd, Color(25, 175, 220, 255));

		if (bOpen)
			DrawOutlinedRectM(iX - 50, iY, 100, iAdd + (iAmount * iAdd) + 1, Color(25, 175, 220, 255));

		DrawStringM(7, iX, iY + 10, Color(255, 255, 255, 255), strText);

		if (bOpen)
		{
			for (int iVarIndex = 1; iVarIndex < iAmount + 1; iVarIndex++)
			{
				const char* strMeme = chTable[iVarIndex - 1];

				if (Hovering(iX - 50, iY + iVarIndex * iAdd, 100, iAdd))
					DrawRectM(iX - 49, iY + iVarIndex * iAdd, 98, iAdd, Color(255, 255, 255, 125));

				if (Holding(iX - 50, iY + iVarIndex * iAdd, 100, iAdd))
				{
					iVar = iVarIndex;
					pConfig->Save();
				}

				DrawStringM(7, iX, iY + (iAdd / 2) + (iVarIndex * iAdd), iVar == iVarIndex ? Color(125, 125, 125, 255) : Color(255, 255, 255, 255), strMeme);
			}
		}
	}
}

void CMenu::DrawTab(const char* strText, int iTabIndex)
{
	int iBaseX = (iWidth / 4) * (iTabIndex - 1);
	int iBaseW = iWidth / 4;

	DrawRectM(iBaseX, 24, iBaseW, 28, iTab == iTabIndex ? Color(5, 155, 200, 255) : Color(25, 175, 220, 255));
	DrawStringM(7, iBaseX + (iBaseW / 2), 38, Color(255, 255, 255, 255), strText);

	if (Hovering(iBaseX, 24, iBaseW, 28))
		DrawRectM(iBaseX, 24, iBaseW, 28, Color(255, 255, 255, 50));

	if (Clicked(iBaseX, 24, iBaseW, 28))
		iTab = iTabIndex;
}

void CMenu::DrawBackground()
{
	DrawRectM(0, 0, iWidth, iHeight, Color(25, 25, 50, 200));
	DrawRectM(0, 0, iWidth, 24, Color(25, 175, 220, 255));
	DrawStringM(7, iWidth / 2, 12, Color(255, 255, 255, 255), "m3mehook");
	DrawOutlinedRectM(0, 0, iWidth, iHeight, Color(25, 175, 220, 255));
}

void CMenu::DrawClose()
{
	DrawRectM(iWidth - 24, 0, 24, 24, Color(255, 0, 0, 255));
	DrawStringM(7, iWidth - 12, 12, Color(255, 255, 255, 255), "X");

	if (Clicked(iWidth - 24, 0, 24, 24))
		bMenuOpen = false;
}

void CMenu::DrawCursor()
{
	Draw::DrawRect(iMouseX - 2, iMouseY - 2, 4, 4, Color(255, 255, 255, 255));
}

void CMenu::DrawButtons()
{
	int y = 58;
	int add = 24;

	DrawTab("Aimbot", 1);
	DrawTab("Visuals", 2);
	DrawTab("Misc", 3);
	DrawTab("HvH", 4);

	DrawButton("Enabled", 1, 8, y, Vars::Aimbot::Enabled);
	DrawDropdown("Aim Bone", 1, iWidth - 58, 58, add, Vars::Aimbot::BoneArray, 4, Vars::Aimbot::BoneHold, Vars::Aimbot::BoneOpen, Vars::Aimbot::Bone);
	DrawButton("FOV Check", 1, 8, y += add, Vars::Aimbot::FOV);
	DrawSlider("FOV Max", 1, 8, y += add, 180, 90, Vars::Aimbot::FOVMax);
	DrawButton("Smooth", 1, 8, y += add, Vars::Aimbot::Smooth);
	DrawSlider("Smoothness", 1, 8, y += add, 180, 30, Vars::Aimbot::Smoothness);
	DrawButton("Silent", 1, 8, y += add, Vars::Aimbot::Silent);
	DrawButton("IgnoreTeam", 1, 8, y += add, Vars::Aimbot::IgnoreTeam);
	DrawButton("RCS", 1, 8, y += add, Vars::Aimbot::RCS);
	DrawSlider("RCS Amount", 1, 8, y += add, 180, 100, Vars::Aimbot::RCSAmt);
	DrawButton("AutoWall", 1, 8, y += add, Vars::Aimbot::AutoWall);
	DrawButton("AutoShoot", 1, 8, y += add, Vars::Aimbot::AutoShoot);
	DrawButton("AutoCrouch", 1, 8, y += add, Vars::Aimbot::AutoCrouch);
	DrawButton("AutoStop", 1, 8, y += add, Vars::Aimbot::AutoStop);
	DrawButton("Aim Key [LALT]", 1, 8, y += add, Vars::Aimbot::UseAimKey);

	y = 58;
	DrawButton("Enabled", 2, 8, y, Vars::Visuals::Enabled);
	DrawButton("Enemy Only", 2, 8, y += add, Vars::Visuals::EnemyOnly);
	DrawButton("Names", 2, 8, y += add, Vars::Visuals::Name);
	DrawButton("2D Boxes", 2, 8, y += add, Vars::Visuals::Box);
	DrawDropdown("Box Type", 2, iWidth - 58, 58, add, Vars::Visuals::BoxTypeArray, 2, Vars::Visuals::BoxTypeHold, Vars::Visuals::BoxTypeOpen, Vars::Visuals::BoxType);
	DrawButton("Health Bars", 2, 8, y += add, Vars::Visuals::HealthBar);
	DrawButton("Trace Lines", 2, 8, y += add, Vars::Visuals::TraceLines);
	DrawButton("No Visual Recoil", 2, 8, y += add, Vars::Visuals::NoVisRecoil);
	DrawSlider("Added FOV", 2, 8, y += add, 180, 50, Vars::Visuals::FOVAdd);

	y = 58;
	DrawButton("AutoHop", 3, 8, y, Vars::Misc::Bhop);
	DrawButton("NoRecoil", 3, 8, y += add, Vars::Misc::RCS);
	DrawButton("NameSteal", 3, 8, y += add, Vars::Misc::NameSteal);
	DrawButton("NameSpam", 3, 8, y += add, Vars::Misc::NameStealAdv);
	DrawSlider("NS Delay", 3, 8, y += add, 180, 80, Vars::Misc::NameStealDelay);

	y = 58;
	DrawButton("Anti-Aim", 4, 8, y, Vars::HvH::AntiAim);
	DrawDropdown("Anti-Aim X Type", 4, iWidth - 160, 58, add, Vars::HvH::AntiAimTypeArray, 6, Vars::HvH::AntiAimTypeHold, Vars::HvH::AntiAimTypeOpen, Vars::HvH::AntiAimType);
	DrawDropdown("Anti-Aim Y Type", 4, iWidth - 58, 58, add, Vars::HvH::AntiAimTypeYArray, 8, Vars::HvH::AntiAimTypeYHold, Vars::HvH::AntiAimTypeYOpen, Vars::HvH::AntiAimTypeY);
	DrawButton("FakeLag", 4, 8, y += add, Vars::HvH::FakeLag);
	DrawSlider("FL Choke", 4, 8, y += add, 180, 14, Vars::HvH::FakeLagChoke);
	DrawSlider("FL Send", 4, 8, y += add, 180, 14, Vars::HvH::FakeLagSend);
	DrawButton("AirStuck [LCTRL]", 4, 8, y += add, Vars::HvH::AirStuck);
	DrawButton("NaN ForwardMove", 4, 8, y += add, Vars::HvH::ForwardNaN);
}