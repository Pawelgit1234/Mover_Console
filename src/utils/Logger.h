#pragma once

#include <iostream>
#include <string>
#include <ctime>

namespace logger
{
	enum class LoggerType
	{
		INFO,
		WARNING,
		CRITICAL,
	};

	void log(const std::string& text, LoggerType log_type);
}