#include "http_session.h"

http_session::http_session(asio::io_service& io, tcp::socket socket) : 
    io_(io), socket_(std::move(socket)) {}

http_session::~http_session()
{
    socket_.close();
}

void http_session::start()
{
    read_request();
}

void http_session::read_request()
{
    auto this_session = shared_from_this();

    http::async_read(socket_, buffer_, request_, 
        [this, this_session](boost::system::error_code ec, std::size_t bytes)
        {
            if (!ec)
            {
                Logger::get_logger()->log("Got the request " + std::string(request_.target()) + "\n");
                handle_request();
            }
            else
            {
                Logger::get_logger()->log("Error reading address of the stream!\n");
            }
        });
}

void http_session::handle_request()
{
    if (request_.method() != http::verb::get)
    {
        Logger::get_logger()->log("Request has no GET method!\n");
        return;
    }
    
    std::string request = request_.target();

    auto connection_data = parse_request(request);

    if (connection_data.has_value())
    {
        auto [host, port] = connection_data.value();

        Logger::get_logger()->log("Connecting to the stream: udp://" + host + ":" + port + "\n");

        std::make_shared<udp_connection>(io_, host, std::stoi(port))->start();
    }
    else
    {
        Logger::get_logger()->log("Error extracting data to connect to the stream!\n");
    }
}

std::optional<std::pair<std::string, std::string>> http_session::parse_request(const std::string& target) const
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

            return { { value.host(), value.port() } };
        }
    }
    
    return std::nullopt;
}
