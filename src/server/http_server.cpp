#include "http_server.h"

http_server::http_server(asio::io_service& io, const tcp::endpoint& ep) : 
        io_(io), acceptor_(io, ep) {}

void http_server::run()
{
    start_accept();
}

void http_server::start_accept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!ec)
        {
            Logger::get_logger()->log("New client has connected to the server! Opening http session...\n");
            std::make_shared<http_session>(io_, std::move(socket))->start();
        }
        else
        {
            Logger::get_logger()->log("Error accepting client: " + ec.message() + "\n");
        }

        start_accept();
    });
}
