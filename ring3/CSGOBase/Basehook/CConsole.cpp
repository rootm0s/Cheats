#include "DllMain.h"
bool completed;

bool CConsole::IsActive()
{
	if(!bActive)
		return false;
	else
		return true;
}

char* stringToLower ( char *string ) 
{
	int i;
	int len = strlen ( string );
	for ( i = 0; i<len; i++ ) {
		if ( string [i] >= 'A' && string [i] <= 'Z' ) {
			string [i] += 32;
		}
	}
	return string;
}


bool bIsDigitInString(std::string pszString)
{
	for(int ax = 0;ax <= 9;ax++)
	{
		char buf[MAX_PATH];

		_snprintf(buf,(size_t)255,"%d",ax);

		if(strstr( pszString.c_str(), buf ))
		{
			return true;
		}
	}
	return false;
}


void CConsole::addCvar(char *szName,int iValue,bool bStyle)
{
	CRetVar buf = CRetVar(szName,iValue,bStyle);	
	vars.push_back(buf);
}

int CConsole::getValue(const char* szName)
{
	for(int ax = 0; ax < vars.size();ax++)
	{
		if(vars[ax].szName == szName)
			return vars[ax].iValue;
	}
	return 0;
}

void CConsole::setValue(const char* szName, int szValue)
{
	for (int ax = 0; ax < vars.size(); ax++)
	{
		if (vars[ax].szName == szName)
			vars[ax].iValue = szValue;
	}
}

void CConsole::HandleCommands(std::string szCommand)
{
	if(szCommand.empty())
		return;

	if ( strcmp ( szCommand.c_str ( ), "ActivateConsole" ) == 0 )
	{
		selected = 0;
		bActive = !bActive;		
		return;
	}	

	int r = getValue("printfont_r");
	int g = getValue("printfont_g");
	int b = getValue("printfont_b");

	char szCommandBuffer [512] = "";
	strcpy ( szCommandBuffer, szCommand.c_str ( ) );

	szCommand = stringToLower ( szCommandBuffer );

	if(strcmp (szCommand.c_str(), "help") == 0)
	{
		for(int ax = 0;ax < vars.size();ax++)
		{
			if(vars[ax].bStyle == true)
				continue;

			print(r,g,b,"%s %i",vars[ax].szName.c_str(),vars[ax].iValue);
		}
		return;
	}

	std::string szCmd,szValue;
	int iValue;
	size_t nPos;
	
	bool isDigitInString = bIsDigitInString(szCommand);
	bool isTextCmd = strstr(szCommand.c_str(), "echo") || strstr(szCommand.c_str(), "exec") || strstr(szCommand.c_str(), "save");
	if ( isDigitInString ||isTextCmd )
	{
		nPos = szCommand.find_first_of ( " " );
		
		if ( nPos != std::string::npos )
		{
			szCmd = szCommand.substr ( 0, nPos );		
			
			szCommand.erase ( 0, nPos + 1 );			

			if ( szCommand.length ( ) > 0 )
			{
				szValue = szCommand;
			}
		}

	}
	else 
		print(r,g,b,"%s is currently set to %i",szCommand.c_str(),getValue(szCommand.c_str()));
	
		if(strcmp(szCmd.c_str(),"echo") == 0)
		{
			print(r,g,b,szValue.c_str());
			return;
		}

		if(strcmp(szCmd.c_str(),"exec") == 0)
		{
			readConfig(szValue.c_str());
		}
		if (strcmp(szCmd.c_str(), "save") == 0)
		{
			saveConfig(szValue.c_str());
		}

	iValue = atoi ( szValue.c_str ( ) );
		
	for(int ax = 0;ax < vars.size();ax++)
	{			
		if(strcmp(vars[ax].szName.c_str(),szCmd.c_str()) == 0)
		{
			vars[ax].iValue = iValue;

			if(vars[ax].bStyle == true)
				continue;

			print(0,255,127,"%s has been set to %i",szCmd.c_str(),iValue);				
		} 
	}	
}

void CConsole::HandleKeys(int key)
{

	Key_t pKey = gKeyMap[key];	

	switch(key)
	{
	case KEY_LSHIFT:
	case KEY_RSHIFT:
	case KEY_LCONTROL:
	case KEY_RCONTROL:
	case KEY_LEFT:
	case KEY_RIGHT:		
	case KEY_CAPSLOCKTOGGLE:
	case MOUSE_WHEEL_UP:
	case MOUSE_WHEEL_DOWN:
	case KEY_ESCAPE:
	case KEY_LALT:
	case KEY_RALT:
	case MOUSE_LEFT:
	case MOUSE_RIGHT:
	case MOUSE_MIDDLE:
	case MOUSE_4:
	case MOUSE_5:
	case KEY_CAPSLOCK:
	case KEY_TAB:
	case KEY_F1:
	case KEY_F2:
	case KEY_F3:
	case KEY_F4:
	case KEY_F5:
	case KEY_F6:
	case KEY_F7:
	case KEY_F8:
	case KEY_F9:
	case KEY_F10:
	case KEY_F11:
	case KEY_F12:
	case KEY_SCROLLLOCK:
	case KEY_NUMLOCK:
	case KEY_NUMLOCKTOGGLE:
	case KEY_PAGEUP:
	case KEY_PAGEDOWN:
	case KEY_END:
	case KEY_DELETE:
	case KEY_HOME:
		return;
	
	case KEY_UP:	
		/*if(selected > 0)
			selected--;

			bufVar[selected+1].bSelected = false;

		bufVar[selected].bSelected = true;	*/

		return;

	case KEY_DOWN:			
		/*bufVar[selected].bSelected = true;

		if(selected > 0)
			bufVar[selected-1].bSelected = false;

		if(selected < bufVar.size())
		selected++;*/

		return;

	case KEY_ENTER:
		if(bComplete == true)
		{
			command = vars[selected-1].szName;
			command.append(" ");
			selected = 0;	
			bComplete = false;
			return;
		}

		HandleCommands(command.c_str());
		command.clear();
		buf.clear();
		counter = 0;
		selected = 0;
		return;

	case KEY_SPACE:
		command.append(" ");
		return;

	case KEY_BACKSPACE:
		if(command.length() > 0)
		{
			command.erase(command.length()-1);
			counter--;
		}		
		return;
	}
	
	if( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_RSHIFT ) )
	{
		command.append( pKey.m_alt );
		counter++;
	}
	else
	{
		command.append( pKey.m_def );
		counter++;
	}	
}
std::string CConsole::CheckComp(std::string tempString)
{
	int count = 0;
	int line;

	for (int ax = 0; ax < vars.size(); ax++)
	{
		buf = vars[ax].szName.substr(0, counter); //Get command name with same length as our input

		if (strcmp(command.c_str(), buf.c_str()) == 0)
		{
			count++;
			line = ax;

		}
	}
	if (count == 1)
	{
		completed = true;
		return vars[line].szName;
	}

	completed = false;
	return "";
}
void CConsole::RenderConsole()
{
	if(!bActive)
		return;

	static bool bDidOnce = false;

	int r = getValue("printfont_r");
	int g = getValue("printfont_g");
	int b = getValue("printfont_b");

	if(bDidOnce == false)
	{
		print(r, g, b, "Credits:");
		print(r,g,b,"GT3X, Kolo, rifk, Nitrix, Atex, menc9re, ReactiioN");		
		print(r, g, b, "Greetings:");
		print(r,g,b,"mekelek, LeVaCa, neko, 3arbi, sh0w");
		
		readConfig("onstartup.cfg");

		bDidOnce = true;			
		iPrintCounter = 0;
	}

	int iWidth,iHeight;
	g_pEngine->GetScreenSize(iWidth,iHeight);
		
	g_Draw.FillRGBA(600,100,400,500,getValue("mainframe_r"),getValue("mainframe_g"),getValue("mainframe_b"),getValue("mainframe_a")); //MainFrame
	g_Draw.DrawBox(600,100,400,500,3,getValue("mainbox_r"),getValue("mainbox_g"),getValue("mainbox_b"),getValue("mainbox_a")); //MainBox
	g_Draw.DrawBox(600,570,400,30,3,getValue("input_r"),getValue("input_g"),getValue("input_b"),getValue("input_a")); //InputField	
	g_Draw.FillRGBA(600,100,400,20,getValue("headerframe_r"),getValue("headerframe_g"),getValue("headerframe_b"),getValue("headerframe_a")); //HeaderFrame
	g_Draw.DrawBox(600,100,400,20,3,getValue("header_r"),getValue("header_g"),getValue("header_b"),getValue("header_a")); //Header

	g_TableFont.DrawString(true,630,107,getValue("stringcons_r"),getValue("stringcons_g"),getValue("stringcons_b"), 255, "Console");
	g_Draw.DrawStringA(g_Draw.m_Font, false, 605, 575, getValue("cmdcolor_r"), getValue("cmdcolor_g"), getValue("cmdcolor_b"), 255, command.c_str());

	buf = CheckComp(buf);
	if (completed)
	{
		g_Draw.FillRGBA(600, 600, 400, 25, 0, 0, 0, 150);
		g_Draw.DrawBox(600, 597, 400, 27, 3, 0, 0, 0, 255); //Header
		g_Draw.DrawStringA(g_Draw.m_Font, false, 605, 602, 255, 255, 255, 255, buf.c_str());
		if (GetAsyncKeyState(VK_DOWN))
		{
			command = buf;
			command + " ";

		}
	}



	//Drawconsole history
	for(int ax = 0; ax < output.size();ax++)
	{
		int iIndex = (output.size()-ax)+1;
		int iHeight = 580-(20*(iIndex));

		if(iHeight < 120 )
			continue;

		g_Draw.DrawStringA(g_Draw.m_Font, false, 605, iHeight, r, g, b, 255, (char*)output[ax].c_str());
	}

	//Autocomplete
	if(command.length() > 0)
	{			
		for(int ax = 0; ax < vars.size();ax++)
		{
			buf = vars[ax].szName.substr(0,counter); //Get command name with same length as our input

			if(strcmp(command.c_str(),buf.c_str()) == 0)
			{

			}
		}
	}

}

void CConsole::print(int r,int g,int b,const char* szInput,...)
{
	va_list		va_alist;
	char		szBuf[2048];

	va_start( va_alist, szInput );
	_vsnprintf( szBuf, sizeof( szBuf ), szInput, va_alist );
	va_end( va_alist );

	COLOR buf;

	buf.r = r;
	buf.g = g;
	buf.b = b;
	buf.a = 255;
	
	output.push_back( szBuf );
	outputColor.push_back(buf);
}

void CConsole::readConfig(const char* szName)
{
	std::string line;
	std::string crtlDir = "C:\\Configuration\\";
	std::string path = crtlDir + szName + ".cfg";

	std::fstream myfile (path);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			if(strstr(line.c_str(),"//"))
				continue;

				HandleCommands(line);
		}
		myfile.close();	

	}
	else
		print(255,0,0,"file not found");
}
void CConsole::saveConfig(const char* szName)
{
	std::string line;
	std::string crtlDir = "C:\\Configuration\\";
	std::string path = crtlDir + szName + ".cfg";

	std::fstream myfile(path, std::ios::out);

	if (myfile.is_open())
	{
		myfile.clear();
		for (int ax = 0; ax < vars.size(); ax++)
		{
			std::string cmd = vars[ax].szName + " " + std::to_string(vars[ax].iValue) + "\n";
			myfile << cmd;
		}
		myfile.close();
		print(255, 0, 0, "Config saved in C:\\Configuration");
	}
	else
		print(255, 0, 0, "file not found");
}