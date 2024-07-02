#include "logger.h"
#include <iostream>
#include <boost/url.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url/url_view.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace urls = boost::urls;
using tcp = asio::ip::tcp;

class http_session : public std::enable_shared_from_this<http_session>
{
public:

    http_session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start()
    {
        read();
    }

private:

    void read()
    {
        auto this_session = shared_from_this();

        http::async_read(socket_, buffer_, request_, 
            [this, this_session](beast::error_code ec, std::size_t bytes)
            {
                if (!ec) handle_request();
            });
    }

    void handle_request()
    {
        // http::field::uri
    }

    std::optional<std::pair<std::string, int>> handle_request(const std::string& target) 
    {
        urls::url_view uri(target);

        if (uri.has_query())
        {
            auto params = uri.params();
            auto it = params.find("url");

            if (it != params.end()) 
            {
                std::string udp_url = (*it).value;

                boost::system::result<urls::url_view> result = urls::parse_uri(udp_url);

                auto value = result.value();

                return { { value.host(), std::stoi(value.port()) } };
            } 
            else return std::nullopt;
        }
        else return std::nullopt;
    }

private:

    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
};

class http_server
{
public:

    http_server(asio::io_service& io, const tcp::endpoint& ep) : 
        io_(io), endpoint_(ep), acceptor_(io, endpoint_) {}

    void run()
    {
        start_accept();
    }

private:

    void start_accept()
    {
        acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket)
        {
            if (!ec) 
            {
                std::make_shared<http_session>(std::move(socket))->start();
            }
            start_accept();
        });
    }

private:

    asio::io_service& io_;
    tcp::acceptor acceptor_;
    tcp::endpoint endpoint_;
};



int main(int argc, char** argv)
{
    // std::string req = "http://127.0.0.1:8080?url=udp://127.0.0.1:15016";
    return 0;
}
