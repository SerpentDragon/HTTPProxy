#include "http_server.h"

http_server::http_server(asio::io_service& io, const tcp::endpoint& ep) : 
        io_(io), endpoint_(ep), acceptor_(io, endpoint_) {}

void http_server::run()
{
    start_accept();
}

void http_server::start_accept()
{
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket)
    {
        if (!ec) 
        {
            std::make_shared<http_session>(io_, std::move(socket))->start();
        }
        else
        {

        }

        start_accept();
    });
}