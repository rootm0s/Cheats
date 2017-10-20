typedef void (_cdecl* RandomSeedFn)(int);
typedef int (_cdecl* RandomIntFn)(int,int);
typedef float (_cdecl* RandomFloatFn)(float,float);

class GameUtil
{
public:
	GameUtil()
	{
		HMODULE base = GetModuleHandleA("vstdlib.dll");
		RandomSeed = (RandomSeedFn)GetProcAddress(base,"RandomSeed");
		RandomInt = (RandomIntFn)GetProcAddress(base,"RandomInt");
		RandomFloat = (RandomFloatFn)GetProcAddress(base,"RandomFloat");

		speed = 0;
	}
	void Think();
	void Speedhack();
	Vector ToScreen(Vector);
	void SimulatePrediction();
	void Autohop();
	bool IsReadyToFireWeapon();
	Vector GetSpread(int);
	void SimulateFireBullets();

	// engine functions
	RandomSeedFn RandomSeed;
	RandomIntFn RandomInt;
	RandomFloatFn RandomFloat;

	// random stuff
	UserCmd* cmd;
	Vector viewangles;
	Vector vieworigin;
	Vector lastang;
	Vector velocity;
	int speed;
	char clr[3];
	int scrw,scrh;
	float curtime;
};