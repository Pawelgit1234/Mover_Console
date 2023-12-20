#pragma once

#include <filesystem>
#include <iostream>

#include "../utils/Utils.h"
#include "DrawMap.h"

extern std::string file;

class SelectMap
{
public:
	SelectMap();
	SelectMap(const SelectMap&) = delete;
	void operator=(const SelectMap&) = delete;

public:
	void up();
	void down();
	void selectMap();
	void updateMap();

	unsigned short getPos() const { return pos; }
	std::vector<std::string> getMapList() const { return mapList; }

private:
	void draw();

	mutable unsigned short pos;
	std::vector<std::string> mapList;
};