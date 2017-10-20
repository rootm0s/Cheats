struct PredEnt
{
	int base;
	int health;
	int time;
};

class PredictionManager
{
public:
	void Think(UserCmd*);
	PredEnt list[128];
};