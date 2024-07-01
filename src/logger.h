#pragma once

#include <string>
#include <memory>
#include <fstream>

class Logger
{
private:

    Logger();

    Logger(const Logger&) = delete;

    Logger& operator=(const Logger&) = delete;

public:

    ~Logger();

    static std::shared_ptr<Logger> getLogger();

    void log(const std::string&) const;

private:

    static std::shared_ptr<Logger> logger_;

    std::fstream log_file_;
};