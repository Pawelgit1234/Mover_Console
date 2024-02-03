#pragma once

namespace settings
{
	const unsigned short CONSOLE_WIDTH = 80; // X
	const unsigned short CONSOLE_HEIGHT = 26; // Y
	constexpr unsigned short CONSOLE_MIDDLE_X = CONSOLE_WIDTH / 2;
	constexpr unsigned short CONSOLE_MIDDLE_Y = CONSOLE_HEIGHT / 2;

	const unsigned short TIME_TO_WAIT_BEFORE_START = 1; // seconds before the game starts

	const unsigned short MOVER_SPEED = 2;
	const unsigned short MAX_MOVER_COUNT = 10; //Clients max count
	const unsigned short MIN_MOVER_COUNT = 2; //Clients min count

	const unsigned short MOB_SPEED_MIN = 5; // less -> more = fast -> low | optimal 2
	const unsigned short MOB_SPEED_MAX = 7;
	const unsigned short MOB_COUNT = 100;

	const unsigned short BOOSTER_TIME = 10; // In sec

	const unsigned short FPS = 50;

	extern bool extra_info;
}