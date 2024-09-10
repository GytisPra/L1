#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

extern std::mutex logMutex;

template <typename... Args>
inline void logMsg(const std::string& msg, Args... args) {
    std::ostringstream stream;
    ((stream << args), ...);

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);

    std::lock_guard<std::mutex> lock(logMutex);

    std::cout << "[" << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
              << msg << stream.str() << std::endl;
}

#endif