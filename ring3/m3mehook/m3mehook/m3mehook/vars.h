namespace Vars
{
	namespace Aimbot
	{
		bool Enabled = false;

		//////////////////////////////////////////////

		const char* BoneArray[4] = {
			"Head",
			"Neck",
			"Chest",
			"Pelvis",
		};

		int RealBone[4] = {
			6,
			7,
			4,
			1,
		};

		bool BoneHold = false;
		bool BoneOpen = false;
		int Bone = 1;

		//////////////////////////////////////////////

		bool FOV = false;
		int FOVMax = 90;
		bool Smooth = false;
		int Smoothness = 16;
		bool Silent = false;
		bool IgnoreTeam = false;
		bool RCS = false;
		int RCSAmt = 100;
		bool AutoWall = false;
		bool AutoShoot = false;
		bool AutoStop = false;
		bool AutoCrouch = false;
		int AimKey = VK_LMENU;
		bool UseAimKey = false;
	}

	namespace Visuals
	{
		bool Enabled = false;
		bool EnemyOnly = false;
		bool Name = false;
		bool Box = false;

		//////////////////////////////////////////////

		const char* BoxTypeArray[2] = {
			"Regular",
			"Edge",
		};

		bool BoxTypeHold = false;
		bool BoxTypeOpen = false;
		int BoxType = 1;

		//////////////////////////////////////////////

		bool HealthBar = false;
		bool TraceLines = false;
		bool NoVisRecoil = false;
		int FOVAdd = 0;
	}

	namespace Misc
	{
		bool Bhop = false;
		bool RCS = false;
		bool NameSteal = false;
		bool NameStealAdv = false;
		int NameStealDelay = 0;
	}

	namespace HvH
	{
		bool AntiAim = false;

		//////////////////////////////////////////////

		const char* AntiAimTypeArray[6] = {
			"None",
			"Static Up",
			"Static Down",
			"Jitter",
			"Safe Up",
			"Safe Down",
		};

		bool AntiAimTypeHold = false;
		bool AntiAimTypeOpen = false;
		int AntiAimType = 1;

		//////////////////////////////////////////////

		const char* AntiAimTypeYArray[8] = {
			"None",
			"Fake Sideways",
			"Fake 0",
			"Fake 180",
			"Right",
			"Left",
			"Backwards",
			"Spinbot",
		};

		bool AntiAimTypeYHold = false;
		bool AntiAimTypeYOpen = false;
		int AntiAimTypeY = 1;

		//////////////////////////////////////////////

		bool FakeLag = false;
		int FakeLagChoke = 9;
		int FakeLagSend = 4;
		bool AirStuck = false;
		bool ForwardNaN = false;
	}
}