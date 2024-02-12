#include "Draw.h"

bool losed = false;
unsigned short mobs_killed = 0;
unsigned short wave = 0;

void drawConsoleFrame(std::vector<Mover>& players, std::vector<Mob>& mobs, std::vector<Bullet>& bullets, Clock& clock, Booster& booster)
{
    std::string frame;

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

            bool isPlayerPosition = false;

            for (const Mover& player : players)
            {
                if (i == player.getY() && j == player.getX())
                {
                    isPlayerPosition = true;
                    break;
                }
            }

            if (isPlayerPosition && isMobPosition)
            {
                losed = true;
                return;
            }
            else if (isPlayerPosition)
            {
                if (booster.is_invisible())
                    frame += "\033[45m \033[0m";
                else
                    frame += "\033[44m \033[0m";
            }
            else if (isMobPosition)
                frame += "\033[41m \033[0m";
            else if (isBulletPosition)
            {
                if (booster.is_pierce())
                    frame += "\033[31m*\033[0m";
                else
                    frame += "\033[32m*\033[0m";
            }
            else if (isBoosterPosition)
            {
                switch (booster.getBoostType())
                {
                case BoostType::INVISIBILITY:
                    frame += "\033[45m \033[0m";
                    break;
                case BoostType::PIERCING:
                    frame += "\033[47m \033[0m";
                    break;
                case BoostType::SHOOTER:
                    frame += "\033[42m \033[0m";
                    break;
                }
            }
            else
            {
                switch (map[i][j])
                {
                case BLOCK:
                    frame += "\033[43m \033[0m";
                    break;
                case AIR:
                    frame += ' ';
                    break;
                }
            }
        }

        frame += '\n';
    }

    if (settings::extra_info)
    {
        frame += "Player: (X: " + std::to_string(players[0].getX()) + "|Y: " + std::to_string(players[0].getY()) + ")";
        frame += "\nTime survived: " + std::to_string(clock.getTimeFromStartSec()) + " sec | mobs killed: " + std::to_string(mobs_killed) + " | wave: " + std::to_string(wave);
        frame += "\nBoost Coordinates: (X: " + std::to_string(booster.getX()) + "|Y: " + std::to_string(booster.getY()) + ")";
    }

    std::cout << "\033[2J\033[1;1H" << frame << std::endl;
}
