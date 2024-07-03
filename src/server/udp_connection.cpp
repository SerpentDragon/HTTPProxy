#include "udp_connection.h"

udp_connection::udp_connection(asio::io_service& io, 
    const std::string& host, const std::string& port) : io_(io), socket_(io_)
{
    udp::resolver resolver(io_);
    udp::endpoint endpoint = *resolver.resolve(udp::v4(), host, port).begin();

    socket_.connect(endpoint);
}
