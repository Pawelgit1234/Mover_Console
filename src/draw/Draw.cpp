#include "Draw.h"

bool losed = false;
unsigned short mobs_killed = 0;
unsigned short wave = 0;

void drawConsoleFrame(Mover& player, std::vector<Mob>& mobs, std::vector<Bullet>& bullets, Clock& clock, Booster& booster)
{
	std::cout << "\033[2J\033[1;1H";

	for (int i = 0; i < settings::CONSOLE_HEIGHT; i++)
	{
		for (int j = 0; j < settings::CONSOLE_WIDTH; j++)
		{
			bool isMobPosition = false;

			for (const Mob& mob : mobs)
			{
				if (i == mob.getY() && j == mob.getX())
				{
					isMobPosition = true;
					break;
				}
			}

			bool isBulletPosition = false;

			for (const Bullet& bullet : bullets)
			{
				if (i == bullet.getY() && j == bullet.getX())
				{
					isBulletPosition = true;
					break;
				}
			}

			bool isBoosterPosition = (i == booster.getY() && j == booster.getX());

			if (i == player.getY() && j == player.getX() && isMobPosition)
			{
				losed = true;
				return;
			}
			else if (i == player.getY() && j == player.getX())
			{
				if (booster.is_invisible())
					std::cout << "\033[45m \033[0m";
				else
					std::cout << "\033[44m \033[0m";
			}
			else if (isMobPosition)
				std::cout << "\033[41m \033[0m";
			else if (isBulletPosition)
			{
				if (booster.is_pierce())
					std::cout << "\033[31m*\033[0m";
				else
					std::cout << "\033[32m*\033[0m";
			}
			else if (isBoosterPosition)
			{
				switch (booster.getBoostType())
				{
				case BoostType::INVISIBILITY:
					std::cout << "\033[45m \033[0m";
					break;
				case BoostType::PIERCING:
					std::cout << "\033[47m \033[0m";
					break;
				case BoostType::SHOOTER:
					std::cout << "\033[42m \033[0m";
					break;
				}
			}
			else
			{
				switch (map[i][j])
				{
				case BLOCK:
					std::cout << "\033[43m \033[0m";
					break;
				case AIR:
					std::cout << ' ';
					break;
				}
			}
		}

		std::cout << std::endl;
	}

	if (settings::extra_info)
	{
		std::cout << "Player: " << "(X: " << player.getX() << "|Y: " << player.getY() << ")";
		std::cout << std::endl << "Time survived: " << clock.getTimeFromStartSec() << " sec | mobs killed: " << mobs_killed << " | wave: " << wave;
		std::cout << std::endl << "Boost Coordinates: " << "(X: " << booster.getX() << "|Y: " << booster.getY() << ")";
	}
}