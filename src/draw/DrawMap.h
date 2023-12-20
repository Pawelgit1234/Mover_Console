#pragma once

#include <vector>
#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>

#include "../objects/Map.h"
#include "../settings.h"

class DrawMap
{
public:
	DrawMap(std::vector<std::vector<Map>> map);
	DrawMap(const DrawMap&) = delete;
	void operator=(const DrawMap&) = delete;

public:
	bool isSettingWall() const { return map[y][x] == AIR; }

	void up();
	void down();
	void left();
	void right();

	void setWall();
	void deleteWall();
	void saveMap();

private:
	void drawMap();

	mutable unsigned short x;
	mutable unsigned short y;
	mutable std::vector<std::vector<Map>> map;
};