#pragma once

#include <filesystem>
#include <boost/asio.hpp>
#include "../ffmpeg/ffmpeg_manager.h"

namespace asio = boost::asio;
using udp = asio::ip::udp;

class udp_connection : public std::enable_shared_from_this<udp_connection>
{
public:

    udp_connection(asio::io_service&, const std::string&, int);

    ~udp_connection();

    void start();

private:

    void read_data();

private:

    asio::io_service& io_;
    udp::socket socket_;
    
    std::string output_dir_;

    ffmpeg_manager ffmpeg_;
};
