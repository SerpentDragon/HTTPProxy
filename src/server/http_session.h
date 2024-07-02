#pragma once

#include <optional>
#include <boost/url.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url/url_view.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace urls = boost::urls;
using tcp = boost::asio::ip::tcp;

class http_session : public std::enable_shared_from_this<http_session>
{
public:

    http_session(tcp::socket);

    void start();

private:

    void read_request();

    void handle_request();

    std::optional<std::pair<std::string, int>> handle_request(const std::string&);

private:

    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
};