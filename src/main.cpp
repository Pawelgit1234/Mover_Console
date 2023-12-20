#include <conio.h>
#include <filesystem>
#include <string>

#include "Menu.h"

int main(int argc, char* argv[])
{
	namespace fs = std::filesystem;
	const std::string mapsFolderPath = "maps";

	if (!fs::exists(mapsFolderPath))
		if (!fs::create_directory(mapsFolderPath))
			return 1;

	map = cleanMap();

	Menu menu;
	
	while (true){
		switch (_getch())
		{
		case 72:
			if (menu.isAtDetail())
				menu.up();
			break;
		case 80:
			if (menu.isAtDetail())
				menu.down();
			break;
		case 75:
			if (!menu.isAtDetail())
				menu.back();
			break;
		case 77:
			if (menu.isAtDetail())
				menu.forward();
			break;
		}
	}

	return 0;
}