#include "Menu.h"

Menu::Menu() : pos(0), at_btns(true), buttons{"Single player", "Multi player", "Key binds", "Select map", "Draw Map"} { draw(); }

void Menu::up()
{
	if (pos == 0)
		pos = buttons.size() - 1;
	else
		pos--;

	draw();
}

void Menu::down()
{
	if (pos == buttons.size() - 1)
		pos = 0;
	else
		pos++;

	draw();
}

void Menu::back()
{
	at_btns = true;
	draw();
}

void Menu::forward()
{
	at_btns = false;
	switch (pos)
	{
	case 0:
		singlePlayer();
		break;
	case 1:
		multiPlayer();
		break;
	case 2:
		keyBinds();
		break;
	case 3:
		selectMap();
		break;
	case 4:
		drawMap();
		break;
	}
}

void Menu::draw()
{
	system("cls");

	for (int i = 0; i < buttons.size(); i++)
	{
		if (i == pos)
		{
			std::cout << "\033[41m\t" << buttons[i] << "\033[0m" << std::endl;
			continue;
		}
		std::cout << buttons[i] << std::endl;
	}
}

void Menu::singlePlayer()
{
	Mover mover;
	std::vector<Mob> mobs;
	std::vector<Bullet> bullets;
	Booster booster;
	Clock clock;

	bool can_start = false;

	while (true)
	{
		switch (pressed())
		{
		case Keyboard::W:
			mover.up();
			break;
		case Keyboard::A:
			mover.left();
			break;
		case Keyboard::S:
			mover.down();
			break;
		case Keyboard::D:
			mover.right();
			break;
		case Keyboard::M:
			mover.setXY(settings::CONSOLE_MIDDLE_X, settings::CONSOLE_MIDDLE_Y);
			break;
		case Keyboard::E:
			settings::extra_info = !settings::extra_info;
			break;
		case Keyboard::ARROW_UP:
			bullets.emplace_back(mover.getX(), mover.getY(), BulletDirection::UP);
			break;
		case Keyboard::ARROW_DOWN:
			bullets.emplace_back(mover.getX(), mover.getY(), BulletDirection::DOWN);
			break;
		case Keyboard::ARROW_LEFT:
			bullets.emplace_back(mover.getX(), mover.getY(), BulletDirection::LEFT);
			break;
		case Keyboard::ARROW_RIGHT:
			bullets.emplace_back(mover.getX(), mover.getY(), BulletDirection::RIGHT);
			break;
		case Keyboard::NOTHING:
			break;
		}

		if (!losed)
		{
			if (mobs.empty())
			{
				wave++;
				for (int i = 0; i < settings::MOB_COUNT; i++)
				{
					Mob mob;
					mobs.emplace_back(mob);
				}
			}

			std::vector<Mover> moverVector;
			moverVector.push_back(mover);

			if (!can_start)
			{
				drawConsoleFrame(moverVector, mobs, bullets, clock, booster);
				std::this_thread::sleep_for(std::chrono::seconds(settings::TIME_TO_WAIT_BEFORE_START));
				clock.clear();
				can_start = true;
			}

			booster.chekActuality();

			if (booster.getX() == mover.getX() && booster.getY() == mover.getY())
				booster.giveBoost(bullets);

			if (!booster.is_invisible())
				for (Mob& mob : mobs)
					mob.calculateNextCoordinate(moverVector);

			for (Bullet& b : bullets)
			{
				b.move();
				if (map[b.getY()][b.getX()] == BLOCK)
					b.setXY(-1, -1);
				else
				{
					auto mobIt = std::remove_if(mobs.begin(), mobs.end(), [&b](const Mob& mob) {
						return b.getX() == mob.getX() && b.getY() == mob.getY();
						});

					if (mobIt != mobs.end())
					{
						mobs.erase(mobIt, mobs.end());
						if (!booster.is_pierce())
							b.setXY(-1, -1);
						mobs_killed++;
					}
				}
			}

			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
				return b.getX() == -1 && b.getY() == -1;
				}), bullets.end());

			std::this_thread::sleep_for(std::chrono::milliseconds((int)1000 / settings::FPS));
			drawConsoleFrame(moverVector, mobs, bullets, clock, booster);
		}
		else
			break;
	}

	bullets.clear();
	mobs.clear();
	system("cls");
	std::cout << "Game over!" << std::endl << "You survived " << clock.getTimeFromStartSec() << " sec and killed " << mobs_killed << " mobs!" << std::endl;
	std::cout << "Wave " << wave << std::endl;
	losed = false;
	wave = 0;
	mobs_killed = 0;
}

void Menu::multiPlayer()
{
	system("cls");
	int choice;
	std::cout << "Server[0] or Client[1]: ";
	std::cin >> choice;
	if (choice == 0)
	{
		std::cout << "Clients count [2 - 10]: ";
		std::cin >> choice;
		if (choice > settings::MAX_MOVER_COUNT || choice < settings::MIN_MOVER_COUNT)
		{
			std::cerr << "Error: Wrong count of Players" << std::endl;
			return;
		}

		Server server(choice);
		server.start(8080);
	}
	else if (choice == 1)
	{
		Client client;
		client.connect("127.0.0.1", 8080);
	}
}

void Menu::keyBinds()
{
	system("cls");

	std::cout << "WASD - Player Contral" << std::endl;
	std::cout << "Arrows - Player Shoot" << std::endl << std::endl;

	std::cout << "You - \033[44m \033[0m" << std::endl;
	std::cout << "Mob - \033[41m \033[0m" << std::endl;
	std::cout << "Bullet - \033[32m*\033[0m" << std::endl;
	std::cout << "Walls - \033[43m \033[0m" << std::endl;
	std::cout << "Shoot boost - \033[42m \033[0m" << std::endl;
	std::cout << "Invisibility boost - \033[45m \033[0m" << std::endl;
	std::cout << "Piercing boost - \033[47m \033[0m" << std::endl << std::endl;

	std::cout << "Invisible you - \033[45m \033[0m" << std::endl;
	std::cout << "Pierce bullet - \033[31m*\033[0m" << std::endl;
}

void Menu::selectMap()
{
	SelectMap sm;

	bool exitMapSelecting = false;

	while (!exitMapSelecting) {
		switch (_getch())
		{
		case 72:
			sm.up();
			break;
		case 80:
			sm.down();
			break;
		case 75:
			exitMapSelecting = true;
			back();
			break;
		case 77:
			sm.selectMap();
			exitMapSelecting = true;
			break;
		case 'u':
			sm.updateMap();
			exitMapSelecting = true;
			break;
		}
	}
	at_btns = true;
	draw();
}

void Menu::drawMap()
{
	DrawMap dm(cleanMap());

	bool exitMapDrawing = false;

	while (!exitMapDrawing) {
		switch (_getch())
		{
		case 72:
			dm.up();
			break;
		case 80:
			dm.down();
			break;
		case 75:
			dm.left();
			break;
		case 77:
			dm.right();
			break;
		case ' ':
			if (dm.isSettingWall())
				dm.setWall();
			else
				dm.deleteWall();
			break;
		case 'q':
			exitMapDrawing = true;
			break;
		case 's':
			dm.saveMap();
			exitMapDrawing = true;
			break;
		}
	}
	at_btns = true;
	draw();
}
