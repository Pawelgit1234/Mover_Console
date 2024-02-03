#include "Mob.h"

Mob::Mob() : speed_count(1), speed_max(randint(settings::MOB_SPEED_MIN, settings::MOB_SPEED_MAX))
{
    unsigned short xCoordinate;
    unsigned short yCoordinate;
    getRandomSpawnpoint(xCoordinate, yCoordinate);

    x = xCoordinate;
    y = yCoordinate;
}

void Mob::calculateNextCoordinate(std::vector<Mover>& players)
{
    if (speed_count < speed_max)
    {
        speed_count++;
        return;
    }
    else
        speed_count = 1;

    const Mover* closestPlayer = nullptr;
    unsigned int minDistance = std::numeric_limits<unsigned int>::max();

    for (const auto& player : players)
    {
        unsigned int distance = abs(player.getX() - x) + abs(player.getY() - y);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestPlayer = &player;
        }
    }

    if (closestPlayer)
    {
        int xDiff = closestPlayer->getX() - x;
        int yDiff = closestPlayer->getY() - y;

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
