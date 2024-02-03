#pragma once

#include "../settings.h"

enum class BulletDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class Bullet
{
public:
	Bullet(unsigned short Bulletx, unsigned short Bullety, BulletDirection b);
	Bullet() = default;

public:
	void move();

	void setY(short y) { this->y = y; }
	void setX(short x) { this->x = x; }
	void setXY(short x, short y) { this->x = x; this->y = y; }

	short getY() const { return y; }
	short getX() const { return x; }
	void getXY(short& outX, short& outY) { outX = x; outY = y; }

private:
	mutable short x;
	mutable  short y;
	BulletDirection bullet_direction;
};