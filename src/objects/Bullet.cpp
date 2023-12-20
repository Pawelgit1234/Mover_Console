#include "Bullet.h"

Bullet::Bullet(unsigned short Bulletx, unsigned short Bullety, BulletDirection b) : x(Bulletx), y(Bullety), bullet_direction(b){}

void Bullet::move()
{
	switch (bullet_direction)
	{
	case BulletDirection::UP:
		y--;
		break;
	case BulletDirection::DOWN:
		y++;
		break;
	case BulletDirection::LEFT:
		x--;
		break;
	case BulletDirection::RIGHT:
		x++;
		break;
	}
}
