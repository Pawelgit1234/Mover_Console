#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../objects/Mover.h"
#include "../objects/Mob.h"
#include "../utils/Clock.h"
#include "../objects/Bullet.h"
#include "../settings.h"
#include "../objects/Map.h"
#include "../objects/Booster.h"

extern bool losed;
extern unsigned short mobs_killed;
extern unsigned short wave;

void drawConsoleFrame(Mover& player, std::vector<Mob>& mobs, std::vector<Bullet>& bullets, Clock& clock, Booster& booster);