#include "Clock.h"

Clock::Clock() : start_time(std::chrono::high_resolution_clock::now()) {}

void Clock::clear()
{
	this->start_time = std::chrono::high_resolution_clock::now();
}

unsigned int Clock::getTimeFromStartMilli()
{
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
	return elapsed_time;
}

unsigned int Clock::getTimeFromStartSec()
{
	auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count();
	return elapsed_time;
}
