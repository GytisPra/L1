#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>

#ifndef LOGGER_H
#define LOGGER_H

extern std::mutex logMutex;

template <typename... Args>

inline void logMsg(const std::string& msg, Args... args) {
    std::ostringstream stream;

    ((stream << args), ...);

    auto now = std::chrono::system_clock::now();

    // Convert to time_t (which represents time in seconds)
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to tm structure for local time
    std::tm* localTime = std::localtime(&currentTime);

    std::lock_guard<std::mutex> lock(logMutex);
    // Print the date, time, and log message
    std::cout << "[" << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
              << msg << stream.str() << std::endl;
}

#endif