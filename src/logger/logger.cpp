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

std::shared_ptr<Logger> Logger::get_logger()
{
    if (logger_ == nullptr) logger_ = std::shared_ptr<Logger>(new Logger());

    return logger_;
}

void Logger::log(const std::string& msg)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "[%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count() << "] ";

    log_file_ << ss.str() << msg << std::endl;
}
