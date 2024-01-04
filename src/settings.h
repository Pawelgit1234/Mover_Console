#pragma once

namespace settings
{
	const unsigned short CONSOLE_WIDTH = 80; // X
	const unsigned short CONSOLE_HEIGHT = 26; // Y
	constexpr unsigned short CONSOLE_MIDDLE_X = CONSOLE_WIDTH / 2;
	constexpr unsigned short CONSOLE_MIDDLE_Y = CONSOLE_HEIGHT / 2;

	const unsigned short START_POS_X = 5;
	const unsigned short START_POS_Y = 5;
	const unsigned short MOVER_SPEED = 2;

	const unsigned short MOB_SPEED_MIN = 5; // less -> more = fast -> low | optimal 2
	const unsigned short MOB_SPEED_MAX = 7;
	const unsigned short MOB_COUNT = 50;

	const unsigned short BOOSTER_TIME = 10; // In sec

	const unsigned short FPS = 50;

	extern bool extra_info;
}