#pragma once

#include <random>
#include <string>
#include <fstream>
#include <vector>

#include "../objects/Bullet.h"
#include "../settings.h"
#include "../objects/Map.h"

int randint(int min, int max);
void loadMap(std::string& filename);
std::vector<std::vector<Map>> readMap(std::string& filename);
std::vector<std::vector<Map>> cleanMap();