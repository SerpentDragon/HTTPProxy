#pragma once

#include <boost/asio.hpp>

namespace asio = boost::asio;
using udp = asio::ip::udp;

class udp_connection : public std::enable_shared_from_this<udp_connection>
{
public:

    udp_connection(asio::io_service&, const std::string&, const std::string&);

private:

    asio::io_service& io_;
    udp::socket socket_;
};
