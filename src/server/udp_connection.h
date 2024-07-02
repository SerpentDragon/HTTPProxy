#pragma once

#include <boost/asio.hpp>

namespace asio = boost::asio;
using udp = asio::ip::udp;

class udp_connection : public std::enable_shared_from_this<udp_connection>
{
public:

private:

    udp::socket socket_;
};
