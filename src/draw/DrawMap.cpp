#include "DrawMap.h"

DrawMap::DrawMap(std::vector<std::vector<Map>> map) : x(1), y(1), map(map) { drawMap(); }

void DrawMap::up()
{
    if (y > 1)
    {
        --y;
        drawMap();
    }
}

void DrawMap::down()
{
    if (y < settings::CONSOLE_HEIGHT - 2)
    {
        ++y;
        drawMap();
    }
}

void DrawMap::left()
{
    if (x > 1)
    {
        --x;
        drawMap();
    }
}

void DrawMap::right()
{
    if (x < settings::CONSOLE_WIDTH - 2)
    {
        ++x;
        drawMap();
    }
}

void DrawMap::setWall()
{
    map[y][x] = BLOCK;
    drawMap();
}

void DrawMap::deleteWall()
{
    map[y][x] = AIR;
    drawMap();
}

void DrawMap::saveMap()
{
    system("cls");
    std::string mapName;
    std::cout << "How you want to call the map: ";
    std::cin >> mapName;

    std::ofstream file("maps\\" + mapName + ".mm");

    if (file.is_open())
    {
        for (const auto& row : map)
        {
            for (const auto& cell : row)
                file << (cell == BLOCK ? '#' : ' ');

            file << std::endl;
        }
        file.close();
    }
}

void DrawMap::drawMap()
{
    system("cls");
    for (int i = 0; i < settings::CONSOLE_HEIGHT; ++i)
    {
        for (int j = 0; j < settings::CONSOLE_WIDTH; ++j)
        {
            if ((i == y && j == x) && (map[i][j] == BLOCK))
                std::cout << "\033[43m*\033[0m";
            else if (i == y && j == x)
                std::cout << "*";
            else if (map[i][j] == BLOCK)
                std::cout << "\033[43m \033[0m";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Controls: Arrows to move, Space to set/delete wall, q to exit, s to save" << std::endl;
    std::cout << "Cursor position: (" << y << ", " << x << ")" << std::endl;

}
