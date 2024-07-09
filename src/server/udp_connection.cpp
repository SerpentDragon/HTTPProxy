#include "udp_connection.h"

udp_connection::udp_connection(asio::io_service& io, const std::string& host, int port)
    : io_(io), output_dir_(host + ":" + std::to_string(port)),
    socket_(io, udp::endpoint(asio::ip::address::from_string(host), port)),
    ffmpeg_(output_dir_)     
    {
        std::filesystem::create_directory(output_dir_);
    }

udp_connection::~udp_connection()
{
    socket_.close();
}

void udp_connection::start()
{
    read_data();
}

void udp_connection::read_data()
{
    if (ffmpeg_.init_input_ctx(socket_))
    {
        ffmpeg_.write_data();
    }
}
