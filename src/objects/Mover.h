#pragma once

#include <iostream>

#include "../settings.h"
#include "Map.h"
#include "../utils/Utils.h"

class Mover final
{
public:
	Mover();

public:
	void up();
	void down();
	void left();
	void right();

	void setY(unsigned short y) { this->y = y; }
	void setX(unsigned short x) { this->x = x; }
	void setXY(unsigned short x, unsigned short y) { this->x = x; this->y = y; }

	unsigned short getY() const { return y; }
	unsigned short getX() const { return x; }
	void getXY(unsigned short& outX, unsigned short& outY) { outX = x; outY = y; }

private:
	bool regulate_speed();

	mutable unsigned short x;
	mutable unsigned short y;
	mutable unsigned short speed_count = 1;
};