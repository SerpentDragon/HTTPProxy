#include "http_session.h"

http_session::http_session(tcp::socket socket) : socket_(std::move(socket)) {}

void http_session::start()
{
    read_request();
}

void http_session::read_request()
{
    auto this_session = shared_from_this();

    http::async_read(socket_, buffer_, request_, 
        [this, this_session](beast::error_code ec, std::size_t bytes)
        {
            if (!ec) handle_request();
            else
            {

            }
        });
}

void http_session::handle_request()
{
    if (request_.method() != http::verb::get)
    {
        return;
    }
    
    std::string request = request_.target();

    auto connection_data = handle_request(request);

    if (connection_data.has_value())
    {
        auto [host, port] = connection_data.value();

    }
    else
    {

    }
}

std::optional<std::pair<std::string, int>> http_session::handle_request(const std::string& target) 
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
    }
    
    return std::nullopt;
}
