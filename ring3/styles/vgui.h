typedef void (__thiscall* PaintTraverseFn)(void*,int,bool,bool);
typedef bool (__thiscall* KeyEventFn)(void*,void*);

class BaseVGUI
{
public:
	BaseVGUI()
	{
		list = new InterfaceList();

		list->Init("vguimatsurface.dll");

		surface = list->GetInterface("VGUI_Surface");

		list->Init("vgui2.dll");

		panel = list->GetInterface("VGUI_Panel");

		list->Init("engine.dll");

		enginevgui = list->GetInterface("VEngineVGui");
	}

	// surface

	inline void SetDrawColor(int r,int g,int b,int a = 255)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,14))(surface,r,g,b,a);
	}
	inline void DrawRect(int x,int y,int w,int h)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,16))(surface,x,y,x+w,y+h);
	}
	inline void DrawOutlined(int x,int y,int w,int h)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,18))(surface,x,y,x+w,y+h);
	}
	inline void DrawLine(int x1,int y1,int x2,int y2)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,19))(surface,x1,y1,x2,y2);
	}
	inline void SetFont(int font)
	{
		typedef void (__thiscall* dfn)(void*,int);
		((dfn)Virtual(surface,23))(surface,font);
	}
	inline void SetTextColor(int r,int g,int b)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,25))(surface,r,g,b,255);
	}
	inline void SetTextPos(int x,int y)
	{
		typedef void (__thiscall* dfn)(void*,int,int);
		((dfn)Virtual(surface,26))(surface,x,y);
	}
	inline void DrawText(char* string)
	{
		wchar_t result[128];
		wsprintfW(result,L"%S",string);

		typedef void (__thiscall* dfn)(void*,wchar_t*,int,int);
		((dfn)Virtual(surface,28))(surface,result,wcslen(result),0);
	}
	inline void PlaySound(char* str)
	{
		typedef void (__thiscall* dfn)(void*,char*);
		((dfn)Virtual(surface,78))(surface,str);
	}
	inline int CreateFont(char* base,int height,int weight,int flags)
	{
		typedef int (__thiscall* dfn)(void*);
		int ret = ((dfn)Virtual(surface,71))(surface);

		typedef void (__thiscall* dfn2)(void*,int,char*,int,int,int,int,int,int,int);
		((dfn2)Virtual(surface,72))(surface,ret,base,height,weight,0,0,flags,0,0);

		return ret;
	}
	inline void DrawCircle(int x,int y,int size,int vertices)
	{
		typedef void (__thiscall* dfn)(void*,int,int,int,int);
		((dfn)Virtual(surface,99))(surface,x,y,size,vertices);
	}

	// panel

	char* GetPanelName(int index)
	{
		typedef char* (__thiscall* dfn)(void*,int);
		return ((dfn)Virtual(panel,36))(panel,index);
	}

	InterfaceList* list;
	long* surface;
	long* panel;
	long* enginevgui;

	PaintTraverseFn PaintTraversePtr;
	KeyEventFn KeyEventPtr;

	inline void PaintTraverse(int index,bool repaint,bool force) { PaintTraversePtr(panel,index,repaint,force); }
	inline bool KeyEvent(void* evt) { return KeyEventPtr(enginevgui,evt); }
};