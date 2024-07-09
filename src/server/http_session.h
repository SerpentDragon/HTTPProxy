#pragma once

#include <thread>
#include <optional>
#include <boost/url.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "udp_connection.h"
#include <boost/url/url_view.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace urls = boost::urls;
using tcp = boost::asio::ip::tcp;

class http_session : public std::enable_shared_from_this<http_session>
{
public:

    http_session(asio::io_service&, tcp::socket);

    ~http_session();

    void start();

private:

    void read_request();

    void handle_request(const std::string&);

    std::optional<std::pair<std::string, std::string>> parse_request(const std::string&) const;

private:

    asio::io_service& io_;
    tcp::socket socket_;
    std::vector<uint8_t> buffer_;
    http::request<http::string_body> request_;
};
