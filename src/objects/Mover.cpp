#include "Mover.h"

void Mover::up()
{
	if (!regulate_speed())
		return;

	if (y > 0 && map[y - 1][x] != BLOCK)
		y--;
}

void Mover::down()
{
	if (!regulate_speed())
		return;

	if (y < settings::CONSOLE_HEIGHT - 1 && map[y + 1][x] != BLOCK)
		y++;
}

void Mover::left()
{
	if (!regulate_speed())
		return;

	if (x > 0 && map[y][x - 1] != BLOCK)
		x--;
}

void Mover::right()
{
	if (!regulate_speed())
		return;

	if (x < settings::CONSOLE_WIDTH - 1 && map[y][x + 1] != BLOCK)
		x++;
}

bool Mover::regulate_speed()
{
	if (speed_count < settings::MOVER_SPEED)
	{
		speed_count++;
		return false;
	}
	else
	{
		speed_count = 1;
		return true;
	}
}

