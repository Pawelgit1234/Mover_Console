#include "Booster.h"

Booster::Booster() : clock(), isInvisible(false), isPierce(false) { move(); changeType(); }

void Booster::move()
{
	getRandomSpawnpoint(x, y);
}

void Booster::changeType()
{
	switch (randint(1, 3))
	{
	case 1:
		type = BoostType::INVISIBILITY;
		break;
	case 2:
		type = BoostType::PIERCING;
		break;
	case 3:
		type = BoostType::SHOOTER;
		break;
	}
}

void Booster::giveBoost(std::vector<Bullet>& bullets)
{
	switch (type)
	{
	case BoostType::INVISIBILITY:
		giveInvisibility();
		break;
	case BoostType::PIERCING:
		givePiercing();
		break;
	case BoostType::SHOOTER:
		giveShooter(bullets);
		break;
	}

	changeType();
	move();
}

void Booster::chekActuality()
{
	if (clock.getTimeFromStartSec() > settings::BOOSTER_TIME)
	{
		isInvisible = false;
		isPierce = false;
	}
}

void Booster::giveInvisibility()
{
	isInvisible = true;
	isPierce = false;
	clock.clear();
}

void Booster::givePiercing()
{
	isInvisible = false;
	isPierce = true;
	clock.clear();
}

void Booster::giveShooter(std::vector<Bullet>& bullets)
{
	bullets.emplace_back(x, y, BulletDirection::UP);
	bullets.emplace_back(x, y, BulletDirection::DOWN);
	bullets.emplace_back(x, y, BulletDirection::LEFT);
	bullets.emplace_back(x, y, BulletDirection::RIGHT);
}
