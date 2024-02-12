#include "Logger.h"

namespace logger
{
    void log(const std::string& text, LoggerType log_type)
    {
        std::time_t current_time;
        std::time(&current_time);
        std::tm local_time;
        localtime_s(&local_time, &current_time);

        char time_buffer[80];
        strftime(time_buffer, 80, "[%d.%m.%Y %H:%M:%S]", &local_time);

        std::string color_code;
        std::string prefix;
        switch (log_type)
        {
        case LoggerType::INFO:
            color_code = "\033[0;32m"; // green
            prefix = "[INFO] ";
            break;
        case LoggerType::WARNING:
            color_code = "\033[1;33m"; // yellow
            prefix = "[WARNING] ";
            break;
        case LoggerType::CRITICAL:
            color_code = "\033[0;31m"; // red
            prefix = "[CRITICAL] ";
            break;
        default:
            color_code = "\033[0m";
            prefix = "[UNKNOWN] ";
            break;
        }

        std::cout << color_code << time_buffer << " " << prefix << text << "\033[0m" << std::endl;
    }
}
