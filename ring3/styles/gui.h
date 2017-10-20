struct MenuItem
{
	char* name;
	int max;
	int* val;
};

struct MenuSection
{
	char* name;
	MenuItem items[32];
	int count;
};

class GUIManager
{
public:
	void AddItem(char*,int,int*);
	void AddSection(char*);
	void Init();
	bool InputThink(int);
	void Think();
	MenuItem items[32];
	void DrawMenu();
	
	bool menu;
	int select;
	int sub;
	MenuSection sections[5];
	int count;
	int drawn;
};