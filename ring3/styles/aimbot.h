class AimbotManager
{
public:
	void Think(UserCmd*);
	void GetTarget(Vector,Vector&);
	inline bool ValidTarget(Entity*);
	inline bool GetPosition(Entity*,Vector&);
	inline bool Insight(Entity*,Vector);
	Entity* target;
};