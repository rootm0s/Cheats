class ESPManager
{
public:
	ESPManager()
	{
		index = -1;
	}
	void Think();
	void EntThink();
	void GetHealthColor(Entity*,float*);
	void DrawName(Entity*);
	void DrawWeapon(Entity*);
	void DrawHealth(Entity*);
	void DrawBox(Vector,Vector);
	int index;
	int todraw[256];
};