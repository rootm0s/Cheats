using namespace std;

// stolen from liqob :^)
class GConfig
{
public:
	void Save();
	void Load();
private:
	void Read(char*, char*, char*&);
	void ReadInt(char*, char*, int&);
	void ReadBool(char*, char*, bool&);
	void ReadFloat(char*, char*, float&);
	void Write(char*, char*, char*);
	void WriteInt(char*, char*, int);
	void WriteFloat(char*, char*, float);
	void WriteMode(int);
	char szFile[MAX_PATH];
}; GConfig* pConfig = new GConfig;

void GConfig::Save()
{
	TCHAR szPath[MAX_PATH];
	char szFolder[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		sprintf(szFolder, "%s\\m3mehook\\", szPath);
		sprintf(szFile, "%s\\m3mehook\\m3mehook.ini", szPath);
	}

	CreateDirectory(szFolder, NULL);

	WriteInt("Aimbot", "Enabled", Vars::Aimbot::Enabled);
	WriteInt("Aimbot", "Bone", Vars::Aimbot::Bone);
	WriteInt("Aimbot", "FOV", Vars::Aimbot::FOV);
	WriteInt("Aimbot", "FOVMax", Vars::Aimbot::FOVMax);
	WriteInt("Aimbot", "Smooth", Vars::Aimbot::Smooth);
	WriteInt("Aimbot", "Smoothness", Vars::Aimbot::Smoothness);
	WriteInt("Aimbot", "Silent", Vars::Aimbot::Silent);
	WriteInt("Aimbot", "IgnoreTeam", Vars::Aimbot::IgnoreTeam);
	WriteInt("Aimbot", "RCS", Vars::Aimbot::RCS);
	WriteInt("Aimbot", "RCSAmt", Vars::Aimbot::RCSAmt);
	WriteInt("Aimbot", "AutoWall", Vars::Aimbot::AutoWall);
	WriteInt("Aimbot", "AutoShoot", Vars::Aimbot::AutoShoot);
	WriteInt("Aimbot", "AutoStop", Vars::Aimbot::AutoStop);
	WriteInt("Aimbot", "AutoCrouch", Vars::Aimbot::AutoCrouch);
	WriteInt("Aimbot", "AimKey", Vars::Aimbot::AimKey);
	WriteInt("Aimbot", "UseAimKey", Vars::Aimbot::UseAimKey);

	WriteInt("Visuals", "Enabled", Vars::Visuals::Enabled);
	WriteInt("Visuals", "EnemyOnly", Vars::Visuals::EnemyOnly);
	WriteInt("Visuals", "Name", Vars::Visuals::Name);
	WriteInt("Visuals", "Box", Vars::Visuals::Box);
	WriteInt("Visuals", "BoxType", Vars::Visuals::BoxType);
	WriteInt("Visuals", "HealthBar", Vars::Visuals::HealthBar);
	WriteInt("Visuals", "TraceLines", Vars::Visuals::TraceLines);
	WriteInt("Visuals", "NoVisRecoil", Vars::Visuals::NoVisRecoil);
	WriteInt("Visuals", "FOVAdd", Vars::Visuals::FOVAdd);

	WriteInt("Misc", "Bhop", Vars::Misc::Bhop);
	WriteInt("Misc", "RCS", Vars::Misc::RCS);
	WriteInt("Misc", "NameSteal", Vars::Misc::NameSteal);
	WriteInt("Misc", "NameStealAdv", Vars::Misc::NameStealAdv);
	WriteInt("Misc", "NameStealDelay", Vars::Misc::NameStealDelay);

	WriteInt("HvH", "AntiAim", Vars::HvH::AntiAim);
	WriteInt("HvH", "AntiAimType", Vars::HvH::AntiAimType);
	WriteInt("HvH", "AntiAimTypeY", Vars::HvH::AntiAimTypeY);
	WriteInt("HvH", "FakeLag", Vars::HvH::FakeLag);
	WriteInt("HvH", "FakeLagChoke", Vars::HvH::FakeLagChoke);
	WriteInt("HvH", "FakeLagSend", Vars::HvH::FakeLagSend);
	WriteInt("HvH", "AirStuck", Vars::HvH::AirStuck);
}

void GConfig::Load()
{
	TCHAR szPath[MAX_PATH];
	char szFolder[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		sprintf(szFolder, "%s\\m3mehook\\", szPath);
		sprintf(szFile, "%s\\m3mehook\\m3mehook.ini", szPath);
	}

	CreateDirectory(szFolder, NULL);

	ReadBool("Aimbot", "Enabled", Vars::Aimbot::Enabled);
	ReadInt("Aimbot", "Bone", Vars::Aimbot::Bone);
	ReadBool("Aimbot", "FOV", Vars::Aimbot::FOV);
	ReadInt("Aimbot", "FOVMax", Vars::Aimbot::FOVMax);
	ReadBool("Aimbot", "Smooth", Vars::Aimbot::Smooth);
	ReadInt("Aimbot", "Smoothness", Vars::Aimbot::Smoothness);
	ReadBool("Aimbot", "Silent", Vars::Aimbot::Silent);
	ReadBool("Aimbot", "IgnoreTeam", Vars::Aimbot::IgnoreTeam);
	ReadBool("Aimbot", "RCS", Vars::Aimbot::RCS);
	ReadInt("Aimbot", "RCSAmt", Vars::Aimbot::RCSAmt);
	ReadBool("Aimbot", "AutoWall", Vars::Aimbot::AutoWall);
	ReadBool("Aimbot", "AutoShoot", Vars::Aimbot::AutoShoot);
	ReadBool("Aimbot", "AutoStop", Vars::Aimbot::AutoStop);
	ReadBool("Aimbot", "AutoCrouch", Vars::Aimbot::AutoCrouch);
	ReadInt("Aimbot", "AimKey", Vars::Aimbot::AimKey);
	ReadBool("Aimbot", "UseAimKey", Vars::Aimbot::UseAimKey);

	ReadBool("Visuals", "Enabled", Vars::Visuals::Enabled);
	ReadBool("Visuals", "EnemyOnly", Vars::Visuals::EnemyOnly);
	ReadBool("Visuals", "Name", Vars::Visuals::Name);
	ReadBool("Visuals", "Box", Vars::Visuals::Box);
	ReadInt("Visuals", "BoxType", Vars::Visuals::BoxType);
	ReadBool("Visuals", "HealthBar", Vars::Visuals::HealthBar);
	ReadBool("Visuals", "TraceLines", Vars::Visuals::TraceLines);
	ReadBool("Visuals", "NoVisRecoil", Vars::Visuals::NoVisRecoil);
	ReadInt("Visuals", "FOVAdd", Vars::Visuals::FOVAdd);

	ReadBool("Misc", "Bhop", Vars::Misc::Bhop);
	ReadBool("Misc", "RCS", Vars::Misc::RCS);
	ReadBool("Misc", "NameSteal", Vars::Misc::NameSteal);
	ReadBool("Misc", "NameStealAdv", Vars::Misc::NameStealAdv);
	ReadInt("Misc", "NameStealDelay", Vars::Misc::NameStealDelay);

	ReadBool("HvH", "AntiAim", Vars::HvH::AntiAim);
	ReadInt("HvH", "AntiAimType", Vars::HvH::AntiAimType);
	ReadInt("HvH", "AntiAimTypeY", Vars::HvH::AntiAimTypeY);
	ReadBool("HvH", "FakeLag", Vars::HvH::FakeLag);
	ReadInt("HvH", "FakeLagChoke", Vars::HvH::FakeLagChoke);
	ReadInt("HvH", "FakeLagSend", Vars::HvH::FakeLagSend);
	ReadBool("HvH", "AirStuck", Vars::HvH::AirStuck);
}

void GConfig::Read(char* szSection, char* szKey, char* &szData)
{
	char szTemp[128];
	if (GetPrivateProfileString(szSection, szKey, "", szTemp, 128, szFile))
	{
		char szString[128];
		sprintf(szString, szTemp);

		szData = szString;
	}
}

void GConfig::ReadInt(char* szSection, char* szKey, int &iData)
{
	char szTemp[128];
	if (GetPrivateProfileString(szSection, szKey, "", szTemp, 128, szFile))
	{
		char szString[128];
		sprintf(szString, szTemp);

		iData = stoi(szString);
	}
}

void GConfig::ReadBool(char* szSection, char* szKey, bool &bData)
{
	char szTemp[128];
	if (GetPrivateProfileString(szSection, szKey, "", szTemp, 128, szFile))
	{
		char szString[128];
		sprintf(szString, szTemp);

		bData = !!stoi(szString);
	}
}

void GConfig::ReadFloat(char* szSection, char* szKey, float &fData)
{
	char szTemp[128];
	if (GetPrivateProfileString(szSection, szKey, "", szTemp, 128, szFile))
	{
		char szString[128];
		sprintf(szString, szTemp);

		fData = stof(szString);
	}
}

void GConfig::Write(char* szSection, char* szKey, char* szValue)
{
	WritePrivateProfileString(szSection, szKey, szValue, szFile);
}

void GConfig::WriteInt(char* szSection, char* szKey, int szValue)
{
	WritePrivateProfileString(szSection, szKey, to_string(szValue).c_str(), szFile);
}

void GConfig::WriteFloat(char* szSection, char* szKey, float szValue)
{
	WritePrivateProfileString(szSection, szKey, to_string(szValue).c_str(), szFile);
}