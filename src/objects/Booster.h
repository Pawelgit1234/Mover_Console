#pragma once

#include <vector>

#include "Bullet.h"
#include "../utils/Clock.h"
#include "../utils/Utils.h"
#include "Map.h"
#include "../settings.h"

enum class BoostType
{
	INVISIBILITY,
	PIERCING, // Бронебойность
	SHOOTER
};

class Booster
{
public:
	Booster();
	Booster& operator=(const Booster& other);

public:
	void giveBoost(std::vector<Bullet>& bullets);
	void chekActuality();

	void setY(short y) { this->y = y; }
	void setX(short x) { this->x = x; }
	void setXY(short x, short y) { this->x = x; this->y = y; }

	short getY() const { return y; }
	short getX() const { return x; }
	void getXY(short& outX, short& outY) { outX = x; outY = y; }
	
	bool is_invisible() const { return isInvisible; }
	bool is_pierce() const { return isPierce; }
	BoostType getBoostType() const { return type; }

private:
	void move();
	void changeType();
	void giveInvisibility();
	void givePiercing();
	void giveShooter(std::vector<Bullet>& bullets);

	mutable unsigned short x;
	mutable unsigned short y;
	mutable BoostType type;
	mutable bool isInvisible;
	mutable bool isPierce;
	Clock clock;
};