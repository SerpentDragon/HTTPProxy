#include "logger.h"

std::shared_ptr<Logger> Logger::logger_ = nullptr;

Logger::Logger()
{
    log_file_.open("log.txt", std::ios_base::app);
}

Logger::~Logger()
{
    log_file_.close();
}

std::shared_ptr<Logger> Logger::getLogger()
{
    if (logger_ == nullptr) logger_ = std::shared_ptr<Logger>(new Logger());

    return logger_;
}

void Logger::log(const std::string& msg) const
{
    
}