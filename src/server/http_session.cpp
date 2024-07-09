#include "http_session.h"

http_session::http_session(asio::io_service& io, tcp::socket socket) : 
    io_(io), socket_(std::move(socket)), buffer_(128) {}

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

    socket_.async_receive(asio::buffer(buffer_), 
        [this, this_session](boost::system::error_code ec, std::size_t len)
        {
            if (!ec)
            {
                std::string request_data(buffer_.begin(), buffer_.begin() + len);
                request_data = request_data.substr(request_data.find("GET") + 5, request_data.size() - 5);
                Logger::get_logger()->log("Got the request " + request_data + "\n");
                handle_request(request_data);
            }
            else
            {
                Logger::get_logger()->log("Error reading address of the stream from client!\n");
            }
        });
}

void http_session::handle_request(const std::string& request)
{
    auto connection_data = parse_request(request);

    if (connection_data.has_value())
    {
        auto [host, port] = connection_data.value();

        Logger::get_logger()->log("Connecting to the stream: udp://" + host + ":" + port + "\n");

        auto udp_connection_ptr = std::make_shared<udp_connection>(io_, host, std::stoi(port));
        
        std::thread th([udp_connection_ptr](){ udp_connection_ptr->start(); });
        th.detach();
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
