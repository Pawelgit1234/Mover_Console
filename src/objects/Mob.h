#pragma once

#include <vector>
#include <iostream>

#include "Mover.h"
#include "../settings.h"
#include "../utils/Utils.h"
#include "Map.h"

class Mob
{
public:
	Mob();
	Mob& operator=(const Mob& other);

public:
	void setY(short y) { this->y = y; }
	void setX(short x) { this->x = x; }
	void setXY(short x, short y) { this->x = x; this->y = y; }

	short getY() const { return y; }
	short getX() const { return x; }
	void getXY( short& outX, short& outY) { outX = x; outY = y; }

	void calculateNextCoordinate(Mover& player);

private:
	mutable short x;
	mutable short y;
	mutable unsigned short speed_count;
	const unsigned short speed_max;
};