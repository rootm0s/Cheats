#include <iostream>

#include "Engine.h"
#include "Settings.h"
#include "Triggerbot.h"
#include "ESP.h"
#include "Noflash.h"
#include "Bunnyhop.h"
#include "Status.h"

int main() {
	system("Title remware, the premium external polyhack");
	
	Status()->credits();
    
	Settings()->writeConfig();
	
	std::cout << "\nwaiting for csgo.exe . . . ";
	
	Engine()->start();
	std::thread engineThread(&CEngine::update, Engine());

	std::cout << "found that shit b\n\n";

	Sleep(1000);
	
	std::thread settingsThread(&CSettings::run, Settings());
	
	Sleep(500);
	
    std::thread triggerThread(&Triggerbot::run, Triggerbot());
	std::thread espThread(&ESP::run, ESP());
    std::thread noflashThread(&Noflash::run, Noflash());
    std::thread bunnyhopThread(&Bunnyhop::run, Bunnyhop());
    
    Sleep(500);
    
    Status()->refresh();

	while (KeyUp(Settings()->exit_key)) {
		Sleep(100);
	}

	exit(0);

	return 0;
}