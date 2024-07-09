#pragma once

#include "http_session.h"
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class http_server
{
public:

    http_server(asio::io_service&, const tcp::endpoint&);

    void run();

private:

    void start_accept();

private:

    asio::io_service& io_;
    tcp::acceptor acceptor_;
};
