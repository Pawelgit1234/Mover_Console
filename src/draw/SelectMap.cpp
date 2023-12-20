#include "SelectMap.h"

std::string file;

SelectMap::SelectMap() : pos(0)
{
	namespace fs = std::filesystem;
	const std::string mapsFolderPath = "maps";

	if (fs::exists(mapsFolderPath) && fs::is_directory(mapsFolderPath))
	{
		for (const auto& entry : fs::directory_iterator(mapsFolderPath))
		{
			if (entry.is_regular_file())
			{
				std::string filename = entry.path().filename().string();
				mapList.emplace_back(filename.substr(0, filename.find_last_of('.')));
			}
		}
	}
	draw();
}

void SelectMap::up()
{
	if (pos > 0)
	{
		--pos;
		draw();
	}
}

void SelectMap::down()
{
	if (pos < mapList.size() - 1)
	{
		++pos;
		draw();
	}
}

void SelectMap::selectMap()
{
	file = mapList[pos];
	std::string path = "maps/" + file + ".mm";
	loadMap(path);
}

void SelectMap::updateMap()
{
	std::string selectedMap = mapList[pos];
	std::string path = "maps/" + selectedMap + ".mm";
	DrawMap dm(readMap(path));

	bool exitMapDrawing = false;

	while (!exitMapDrawing) {
		switch (_getch())
		{
		case 72:
			dm.up();
			break;
		case 80:
			dm.down();
			break;
		case 75:
			dm.left();
			break;
		case 77:
			dm.right();
			break;
		case ' ':
			if (dm.isSettingWall())
				dm.setWall();
			else
				dm.deleteWall();
			break;
		case 'q':
			exitMapDrawing = true;
			break;
		case 's':
			dm.saveMap();
			exitMapDrawing = true;
			break;
		}
	}
}

void SelectMap::draw()
{
	system("cls");

	for (int i = 0; i < mapList.size(); i++)
	{
		if (i == pos)
		{
			std::cout << "\033[41m\t" << mapList[i] << "\033[0m" << std::endl;
			continue;
		}
		else if (mapList[i] == file)
		{
			std::cout << "\033[44m" << mapList[i] << "\033[0m" << std::endl;
			continue;
		}
		std::cout << mapList[i] << std::endl;
	}

	std::cout << std::endl << "s to save, u to update" << std::endl;
}


