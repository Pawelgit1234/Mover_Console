#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>

#include "draw/Draw.h"
#include "objects/Mob.h"
#include "objects/Bullet.h"
#include "objects/Map.h"
#include "objects/Booster.h"
#include "objects/Mover.h"
#include "utils/Keyboard.h"
#include "draw/DrawMap.h"
#include "utils/Utils.h"
#include "draw/SelectMap.h"

class Menu
{
public:
	Menu();
	Menu(const Menu&) = delete;
	void operator=(const Menu&) = delete;
public:
	void up();
	void down();
	void back();
	void forward();

	bool isAtDetail() const { return at_btns; }
	unsigned short getPos() const { return pos; }

private:
	void draw();
	void singlePlayer();
	void multiPlayer();
	void keyBinds();
	void selectMap();
	void drawMap();

	const std::vector<std::string> buttons;
	mutable unsigned short pos;
	mutable bool at_btns;

};