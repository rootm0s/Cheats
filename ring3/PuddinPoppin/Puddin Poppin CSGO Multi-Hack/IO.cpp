#include "stdafx.h"

HANDLE tIO::tIOFunctions::GetProcessHandleByName(std::wstring ProcessName)
{
    PROCESSENTRY32 ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE SnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(SnapHandle, &ProcessEntry) == TRUE)
        {
            if (!_wcsicmp(ProcessEntry.szExeFile, ProcessName.c_str()))
                {
                    HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessEntry.th32ProcessID);
                    return ProcessHandle;
                }
            else
                {
                    while (Process32Next(SnapHandle, &ProcessEntry) == TRUE)
                        {
                            if (!_wcsicmp(ProcessEntry.szExeFile, ProcessName.c_str()))
                                {
                                    HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessEntry.th32ProcessID);
                                    return ProcessHandle;
                                }
                        }
                }
        }
    CloseHandle(SnapHandle);
    return{ 0 };
}

DWORD tIO::tIOFunctions::GetModuleAddressByName(std::wstring ModuleName, HANDLE Process)
{
    MODULEENTRY32 ModuleEntry;
    ModuleEntry.dwSize = sizeof(MODULEENTRY32);
    HANDLE SnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(Process));
    if (Module32First(SnapHandle, &ModuleEntry) == TRUE)
        {
            if (!_wcsicmp(ModuleEntry.szModule, ModuleName.c_str()))
                {
                    DWORD ModuleAddress = (DWORD)ModuleEntry.modBaseAddr;
                    return ModuleAddress;
                }
            else
                {
                    while (Module32Next(SnapHandle, &ModuleEntry) == TRUE)
                        {
                            if (!_wcsicmp(ModuleEntry.szModule, ModuleName.c_str()))
                                {
                                    DWORD ModuleAddress = (DWORD)ModuleEntry.modBaseAddr;
                                    return ModuleAddress;
                                }
                        }
                }
        }
    CloseHandle(SnapHandle);
    return{ 0 };
}

bool tIO::tIOFunctions::ReadSettingsFile(std::wstring* Data)
{
    std::wifstream FileReader;
    if (std::wifstream(pGlobalVars.IOSettings.SettingsPath))
        {
            FileReader.open(pGlobalVars.IOSettings.SettingsPath);
            int currentline = 1;
            while (!FileReader.eof())
                {
                    std::getline(FileReader, Data[currentline]);
                    currentline++;
                }
            FileReader.close();
            return true;
        }
    else
        {
            return false;
        }
}

D3DCOLOR tIO::tIOFunctions::WColor(std::wstring ColorInfo)
{
    int a, r, g, b;
    a = std::stoi(ColorInfo.substr(0, 3));
    r = std::stoi(ColorInfo.substr(3, 3));
    g = std::stoi(ColorInfo.substr(6, 3));
    b = std::stoi(ColorInfo.substr(9, 3));
    return D3DCOLOR_ARGB(a, r, g, b);
}

bool tIO::tIOFunctions::ParseSettings(std::wstring* Data)
{
	try
	{
		char CharsToDelete[] = "(), ";
		for (int a = 1; a < 200; a++)
		{
			for (int b = 0; b < strlen(CharsToDelete); ++b)
			{
				Data[a].erase(std::remove(Data[a].begin(), Data[a].end(), CharsToDelete[b]), Data[a].end());
			}
			Data[a] = Data[a].substr(Data[a].find(L"=") + 1);
		}
		//offsets
		pGlobalVars.Offsets.m_iCompetitiveRanking = wcstol(Data[1].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iKills = wcstol(Data[2].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iDeaths = wcstol(Data[3].c_str(), 0, 0);
		pGlobalVars.Offsets.m_bHasDefuser = wcstol(Data[4].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iPlayerC4 = wcstol(Data[5].c_str(), 0, 0);
		pGlobalVars.Offsets.CSPlayerResource = wcstol(Data[6].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwViewMatrix = wcstol(Data[7].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwEntityList = wcstol(Data[8].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwLocalPlayer = wcstol(Data[9].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iHealth = wcstol(Data[10].c_str(), 0, 0);
		pGlobalVars.Offsets.m_vecOrigin = wcstol(Data[11].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iTeamNum = wcstol(Data[12].c_str(), 0, 0);
		pGlobalVars.Offsets.m_lifeState = wcstol(Data[13].c_str(), 0, 0);
		pGlobalVars.Offsets.m_fFlags = wcstol(Data[14].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwBoneMatrix = wcstol(Data[15].c_str(), 0, 0);
		pGlobalVars.Offsets.m_vecPunch = wcstol(Data[16].c_str(), 0, 0);
		pGlobalVars.Offsets.m_bDormant = wcstol(Data[17].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwClientState = wcstol(Data[18].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwLocalPlayerIndex = wcstol(Data[19].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iCrossHairID = wcstol(Data[20].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwViewAngle = wcstol(Data[21].c_str(), 0, 0);
		pGlobalVars.Offsets.m_iShotsFired = wcstol(Data[22].c_str(), 0, 0);
		pGlobalVars.Offsets.m_vecVelocity = wcstol(Data[23].c_str(), 0, 0);
		pGlobalVars.Offsets.m_vecViewOffset = wcstol(Data[24].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwRadarBase = wcstol(Data[25].c_str(), 0, 0);
		pGlobalVars.Offsets.m_dwRadarBasePointer = wcstol(Data[26].c_str(), 0, 0);
		pGlobalVars.Offsets.RadarName1 = wcstol(Data[27].c_str(), 0, 0);
		pGlobalVars.Offsets.RadarName2 = wcstol(Data[28].c_str(), 0, 0);
		pGlobalVars.Offsets.EntitySize = wcstol(Data[29].c_str(), 0, 0);

		//aimbot
		pGlobalVars.AimbotSettings.Enabled = std::stoi(Data[30].c_str(), 0, 0);
		pGlobalVars.AimbotSettings.ActivationKey = std::stoi(Data[31].c_str());
		pGlobalVars.AimbotSettings.EnableFire = std::stoi(Data[32].c_str());
		pGlobalVars.AimbotSettings.EnableRandomPitchRecoilReductionFactor = std::stoi(Data[33].c_str());
		pGlobalVars.AimbotSettings.EnableRandomYawRecoilReductionFactor = std::stoi(Data[34].c_str());
		pGlobalVars.AimbotSettings.EnableTimeout = std::stoi(Data[35].c_str());
		pGlobalVars.AimbotSettings.IgnoreShotCount = std::stoi(Data[36].c_str());
		pGlobalVars.AimbotSettings.MaxRandomPitch = std::stof(Data[37].c_str());
		pGlobalVars.AimbotSettings.MaxRandomYaw = std::stof(Data[38].c_str());
		pGlobalVars.AimbotSettings.MinRandomPitch = std::stof(Data[39].c_str());
		pGlobalVars.AimbotSettings.MinRandomYaw = std::stof(Data[40].c_str());
		pGlobalVars.AimbotSettings.TargetJumpingPlayers = std::stoi(Data[41].c_str());
		pGlobalVars.AimbotSettings.SmoothPercent = std::stof(Data[42].c_str());
		pGlobalVars.AimbotSettings.StaticPitch = std::stof(Data[43].c_str());
		pGlobalVars.AimbotSettings.StaticYaw = std::stof(Data[44].c_str());
		pGlobalVars.AimbotSettings.TargetBone = std::stoi(Data[45].c_str());
		pGlobalVars.AimbotSettings.Timeout = std::stoi(Data[46].c_str());

		//esp
		pGlobalVars.EspSettings.Enabled = std::stoi(Data[47].c_str());

		pGlobalVars.EspSettings.DrawEnemyBox = std::stoi(Data[48].c_str());
		pGlobalVars.EspSettings.EnemyBoxColor = WColor(Data[49]);
		pGlobalVars.EspSettings.EnemyBoxLineWidth = std::stof(Data[50].c_str());
		pGlobalVars.EspSettings.EnemyBoxOutlineColor = WColor(Data[51]);
		pGlobalVars.EspSettings.EnemyBoxOutlineWidth = std::stof(Data[52].c_str());

		pGlobalVars.EspSettings.DrawFriendlyBox = std::stoi(Data[53].c_str());
		pGlobalVars.EspSettings.FriendlyBoxColor = WColor(Data[54]);
		pGlobalVars.EspSettings.FriendlyBoxLineWidth = std::stof(Data[55].c_str());
		pGlobalVars.EspSettings.FriendlyBoxOutlineColor = WColor(Data[56]);
		pGlobalVars.EspSettings.FriendlyBoxOutlineWidth = std::stof(Data[57].c_str());

		pGlobalVars.EspSettings.DrawTargetBoxHighlight = std::stoi(Data[58].c_str());
		pGlobalVars.EspSettings.TargetEnemyBoxColor = WColor(Data[59]);
		pGlobalVars.EspSettings.TargetEnemyBoxLineWidth = std::stof(Data[60].c_str());
		pGlobalVars.EspSettings.TargetEnemyBoxOutlineColor = WColor(Data[61]);
		pGlobalVars.EspSettings.TargetEnemyBoxOutlineWidth = std::stof(Data[62].c_str());

		pGlobalVars.EspSettings.DrawEnemySnapLines = std::stoi(Data[63].c_str());
		pGlobalVars.EspSettings.EnemySnapLineColor = WColor(Data[64]);
		pGlobalVars.EspSettings.EnemySnapLineOutlineColor = WColor(Data[65]);
		pGlobalVars.EspSettings.EnemySnapLineOutlineWidth = std::stof(Data[66].c_str());
		pGlobalVars.EspSettings.EnemySnapLineWidth = std::stof(Data[67].c_str());

		pGlobalVars.EspSettings.DrawFriendlySnapLines = std::stoi(Data[68].c_str());
		pGlobalVars.EspSettings.FriendlySnapLineColor = WColor(Data[69]);
		pGlobalVars.EspSettings.FriendlySnapLineOutlineColor = WColor(Data[70]);
		pGlobalVars.EspSettings.FriendlySnapLineOutlineWidth = std::stof(Data[71].c_str());
		pGlobalVars.EspSettings.FriendlySnapLineWidth = std::stof(Data[72].c_str());

		pGlobalVars.EspSettings.DrawTargetSnapLineHighlight = std::stoi(Data[73].c_str());
		pGlobalVars.EspSettings.TargetEnemySnapLineColor = WColor(Data[74]);
		pGlobalVars.EspSettings.TargetEnemySnapLineOutlineColor = WColor(Data[75]);
		pGlobalVars.EspSettings.TargetEnemySnapLineOutlineWidth = std::stof(Data[76].c_str());
		pGlobalVars.EspSettings.TargetEnemySnapLineWidth = std::stof(Data[77].c_str());

		pGlobalVars.EspSettings.DrawEnemyHealthBar = std::stoi(Data[78].c_str());
		pGlobalVars.EspSettings.EnemyHealthBarBackColor = WColor(Data[79]);
		pGlobalVars.EspSettings.EnemyHealthBarFrontColor = WColor(Data[80]);
		pGlobalVars.EspSettings.EnemyHealthBarOutlineColor = WColor(Data[81]);
		pGlobalVars.EspSettings.EnemyHealthBarOutlineWidth = std::stof(Data[82].c_str());
		pGlobalVars.EspSettings.EnemyHealthBarWidth = std::stof(Data[83].c_str());

		pGlobalVars.EspSettings.DrawFriendlyHealthBar = std::stoi(Data[84].c_str());
		pGlobalVars.EspSettings.FriendlyHealthBarBackColor = WColor(Data[85]);
		pGlobalVars.EspSettings.FriendlyHealthBarFrontColor = WColor(Data[86]);
		pGlobalVars.EspSettings.FriendlyHealthBarOutlineColor = WColor(Data[87]);
		pGlobalVars.EspSettings.FriendlyHealthBarOutlineWidth = std::stof(Data[88].c_str());
		pGlobalVars.EspSettings.FriendlyHealthBarWidth = std::stof(Data[89].c_str());

		pGlobalVars.EspSettings.DrawEnemyNameText = std::stoi(Data[90].c_str());
		pGlobalVars.EspSettings.DrawEnemyHealthText = std::stoi(Data[91].c_str());
		pGlobalVars.EspSettings.DrawEnemyKillsText = std::stoi(Data[92].c_str());
		pGlobalVars.EspSettings.DrawEnemyDeathsText = std::stoi(Data[93].c_str());
		pGlobalVars.EspSettings.DrawEnemyKDRatioText = std::stoi(Data[94].c_str());
		pGlobalVars.EspSettings.DrawEnemyRankText = std::stoi(Data[95].c_str());
		pGlobalVars.EspSettings.DrawEnemyBombCarrierText = std::stoi(Data[96].c_str());
		pGlobalVars.EspSettings.DrawEnemyDefuseKitCarrierText = std::stoi(Data[97].c_str());
		pGlobalVars.EspSettings.EnemyInfoTextColor = WColor(Data[98]);
		pGlobalVars.EspSettings.EnemyInfoTextOutlineColor = WColor(Data[99]);

		pGlobalVars.EspSettings.DrawFriendlyNameText = std::stoi(Data[100].c_str());
		pGlobalVars.EspSettings.DrawFriendlyHealthText = std::stoi(Data[101].c_str());
		pGlobalVars.EspSettings.DrawFriendlyKillsText = std::stoi(Data[102].c_str());
		pGlobalVars.EspSettings.DrawFriendlyDeathsText = std::stoi(Data[103].c_str());
		pGlobalVars.EspSettings.DrawFriendlyKDRatioText = std::stoi(Data[104].c_str());
		pGlobalVars.EspSettings.DrawFriendlyRankText = std::stoi(Data[105].c_str());
		pGlobalVars.EspSettings.DrawFriendlyBombCarrierText = std::stoi(Data[106].c_str());
		pGlobalVars.EspSettings.DrawFriendlyDefuseKitCarrierText = std::stoi(Data[107].c_str());
		pGlobalVars.EspSettings.FriendlyInfoTextColor = WColor(Data[108]);
		pGlobalVars.EspSettings.FriendlyInfoTextOutlineColor = WColor(Data[109]);
		
		pGlobalVars.EspSettings.DrawEnemyTargetBoneMarker = std::stoi(Data[110].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerColor = WColor(Data[111].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerLineWidth = std::stof(Data[112].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerSize = std::stof(Data[113].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerType = std::stoi(Data[114].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineColor = WColor(Data[115].c_str());
		pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineWidth = std::stoi(Data[116].c_str());

		pGlobalVars.EspSettings.DrawFriendlyTargetBoneMarker = std::stoi(Data[117].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerColor = WColor(Data[118].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerLineWidth = std::stof(Data[119].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerSize = std::stof(Data[120].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerType = std::stoi(Data[121].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineColor = WColor(Data[122].c_str());
		pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineWidth = std::stoi(Data[123].c_str());

		pGlobalVars.EspSettings.DrawRecoilMarker = std::stoi(Data[124].c_str());
		pGlobalVars.EspSettings.RecoilMarkerColor = WColor(Data[125]);
		pGlobalVars.EspSettings.RecoilMarkerLineWidth = std::stof(Data[126].c_str());
		pGlobalVars.EspSettings.RecoilMarkerOutlineColor = WColor(Data[127]);
		pGlobalVars.EspSettings.RecoilMarkerOutlineWidth = std::stof(Data[128].c_str());
		pGlobalVars.EspSettings.RecoilMarkerSize = std::stof(Data[129].c_str());
		pGlobalVars.EspSettings.RecoilMarkerType = std::stoi(Data[130].c_str());

		//triggerbot
		pGlobalVars.TriggerbotSettings.Enabled = std::stoi(Data[131].c_str());
		pGlobalVars.TriggerbotSettings.ActivationKey = std::stoi(Data[132].c_str());
		pGlobalVars.TriggerbotSettings.EnableTimeout = std::stoi(Data[133].c_str());
		pGlobalVars.TriggerbotSettings.EnableRecoilLimit = std::stoi(Data[134].c_str());
		pGlobalVars.TriggerbotSettings.YawRecoilLimit = std::stof(Data[135].c_str());
		pGlobalVars.TriggerbotSettings.PitchRecoilLimit = std::stof(Data[136].c_str());
		pGlobalVars.TriggerbotSettings.FirstShotDelay = std::stoi(Data[137].c_str());
		pGlobalVars.TriggerbotSettings.FollowUpDelay = std::stoi(Data[138].c_str());
		pGlobalVars.TriggerbotSettings.Timeout = std::stoi(Data[139].c_str());

		//bhop
		pGlobalVars.BhopSettings.Enabled = std::stoi(Data[140].c_str());
		pGlobalVars.BhopSettings.ActivationKey = std::stoi(Data[141].c_str());
		return true;
	}
	catch (int err)
	{
		MessageBoxW(NULL, L"Settings file is not properly formatted!", L"Error", MB_OK);
	}
	return false;
}

bool tIO::tIOFunctions::LoadSettings()
{
    std::wstring Data[200];
    if (ReadSettingsFile(Data))
        {
			if (ParseSettings(Data))
			{
				return true;
			}
        }
    return false;
}

bool tIO::tIOFunctions::Attach()
{
    if (LoadSettings())
        {
            pIO.ProcessHandle = GetProcessHandleByName(pGlobalVars.IOSettings.TargetProcessName);
            if (pIO.ProcessHandle)
                {
                    pIO.ClientModuleBaseAddress = GetModuleAddressByName(pGlobalVars.IOSettings.ClientModuleName, pIO.ProcessHandle);
                    pIO.EngineModuleBaseAddress = GetModuleAddressByName(pGlobalVars.IOSettings.EngineModuleName, pIO.ProcessHandle);
                    if (pIO.ClientModuleBaseAddress && pIO.EngineModuleBaseAddress)
                        {
                            return true;
                        }
                }
        }
    else
        {
            MessageBox(0, L"No Settings File Found!", L"", 0);
        }
    return false;
}

bool tIO::tIOFunctions::IsProcessActive()
{
	DWORD ExitCode;
	GetExitCodeProcess(pIO.ProcessHandle, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		return true;
	}
	return false;
}

void tIO::tIOFunctions::Detach()
{
    if (pIO.ProcessHandle)
        {
            CloseHandle(pIO.ProcessHandle);
        }
}

