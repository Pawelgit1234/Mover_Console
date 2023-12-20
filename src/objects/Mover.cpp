#include "Mover.h"

void Mover::up()
{
	if (y > 0 && map[y - 1][x] != BLOCK)
		y--;
}

void Mover::down()
{
	if (y < settings::CONSOLE_HEIGHT - 1 && map[y + 1][x] != BLOCK)
		y++;
}

void Mover::left()
{
	if (x > 0 && map[y][x - 1] != BLOCK)
		x--;
}

void Mover::right()
{
	if (x < settings::CONSOLE_WIDTH - 1 && map[y][x + 1] != BLOCK)
		x++;
}

