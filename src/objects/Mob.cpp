#include "Mob.h"

Mob::Mob() : speed_count(1), speed_max(randint(settings::MOB_SPEED_MIN, settings::MOB_SPEED_MAX))
{
	int x;
	int y;

	while (true)
	{
		x = randint(1, settings::CONSOLE_WIDTH - 1);
		y = randint(1, settings::CONSOLE_HEIGHT - 1);

		if (map[y][x] != BLOCK)
			break;
	}

	this->x = x;
	this->y = y;
}

void Mob::calculateNextCoordinate(Mover& player)
{
    if (speed_count < speed_max)
    {
        speed_count++;
        return;
    }
    else
        speed_count = 1;

    unsigned short playerX = player.getX();
    unsigned short playerY = player.getY();

    int xDiff = playerX - x;
    int yDiff = playerY - y;

    int xDirection = (xDiff > 0) ? 1 : ((xDiff < 0) ? -1 : 0);
    int yDirection = (yDiff > 0) ? 1 : ((yDiff < 0) ? -1 : 0);

    if (x + xDirection > 0 && x + xDirection <= settings::CONSOLE_WIDTH &&
        y + yDirection > 0 && y + yDirection <= settings::CONSOLE_HEIGHT &&
        map[y + yDirection][x + xDirection] != BLOCK)
    {
        x += xDirection;
        y += yDirection;
    }
    else if (y + yDirection > 0 && y + yDirection <= settings::CONSOLE_HEIGHT &&
        map[y + yDirection][x] != BLOCK)
    {
        y += yDirection;
    }
    else if (x + xDirection > 0 && x + xDirection <= settings::CONSOLE_WIDTH &&
        map[y][x + xDirection] != BLOCK)
    {
        x += xDirection;
    }
}

Mob& Mob::operator=(const Mob& other)
{
    if (this != &other)
    {
        this->x = other.x;
        this->y = other.y;
        this->speed_count = other.speed_count;
    }

    return *this;
}
