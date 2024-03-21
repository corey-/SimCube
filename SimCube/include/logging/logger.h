#pragma once

#include <array>
#include <format>
#include <iostream>
#include <string>

struct LoggerInfo
{
    std::string SourceName;
};

enum class LogLevel : uint8_t
{
    INFO  = 0,
    WARN  = 1,
    ERROR = 2
};

template <typename... Args>
void Log(const LoggerInfo& info, LogLevel level, std::string_view message, Args&&... args)
{
    static constexpr std::array LOG_LEVEL_NAMES{ "INFO", "WARN", "ERROR" };

    std::cout << std::format("[{}] ", info.SourceName);
    std::cout << std::format("{}: ", LOG_LEVEL_NAMES.at(static_cast<std::underlying_type_t<LogLevel>>(level)));
    std::cout << std::vformat(message, std::make_format_args(args...)) << "\n";
}

template <typename... Args>
void DevLog(const LoggerInfo& info, LogLevel level, std::string_view message, Args&&... args)
{
#ifdef DEV_LOGGING
    Log(info, level, message, std::forward<Args>(args)...);
#endif
}
