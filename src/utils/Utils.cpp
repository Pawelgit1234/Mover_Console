#include "Utils.h"

int randint(int min, int max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void loadMap(std::string& filename)
{
    std::ifstream file(filename);

    if (file.is_open())
    {
        map.clear();

        std::string line;
        while (std::getline(file, line))
        {
            std::vector<Map> row;
            for (char c : line)
            {
                if (c == ' ')
                    row.push_back(AIR);
                else if (c == '#')
                    row.push_back(BLOCK);
            }
            map.push_back(row);
        }

        file.close();
    }
}

std::vector<std::vector<Map>> readMap(std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::vector<Map>> newMap;

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<Map> row;
            for (char c : line)
            {
                if (c == ' ')
                    row.push_back(AIR);
                else if (c == '#')
                    row.push_back(BLOCK);
            }
            newMap.push_back(row);
        }

        file.close();
    }

    return newMap;
}

std::vector<std::vector<Map>> cleanMap()
{
    std::vector<std::vector<Map>> newMap;
    for (unsigned short i = 0; i < settings::CONSOLE_HEIGHT; ++i)
    {
        std::vector<Map> row;

        for (unsigned short j = 0; j < settings::CONSOLE_WIDTH; ++j)
        {
            if (i == 0 || i == settings::CONSOLE_HEIGHT - 1 || j == 0 || j == settings::CONSOLE_WIDTH - 1)
                row.push_back(BLOCK);
            else
                row.push_back(AIR);
        }
        newMap.push_back(row);
    }

    return newMap;
}

void getRandomSpawnpoint(unsigned short& x, unsigned short& y)
{
    while (true)
    {
        x = randint(0, settings::CONSOLE_WIDTH - 1);
        y = randint(0, settings::CONSOLE_HEIGHT - 1);

        if (map[y][x] == Map::AIR)
            break;
    }
}
