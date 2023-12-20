#pragma once

#include <string>
#include <chrono>

class Clock
{
public:
	Clock();
	void operator=(const Clock&) = delete;
	
public:
	void clear();
	unsigned int getTimeFromStartMilli();
	unsigned int getTimeFromStartSec();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};