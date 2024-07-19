#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>

class Logger
{
private:

    Logger();

    Logger(const Logger&) = delete;

    Logger(Logger&&) noexcept = delete;

    Logger& operator=(const Logger&) = delete;

    Logger& operator=(Logger&&) = delete;

public:

    ~Logger();

    static std::shared_ptr<Logger> get_logger();

    void log(const std::string&);

private:

    static std::shared_ptr<Logger> logger_;

    std::fstream log_file_;
};
