#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

std::string stream_host;
std::string stream_port;

class http_client
{
public:

    http_client(asio::io_service& io, const std::string& host, int port)
        : io_(io), host_(host), port_(port), socket_(io) {}

    void run()
    {
        connect();
    }

private:

    void connect()
    {
        socket_.async_connect(tcp::endpoint(boost::asio::ip::make_address(host_), port_),
            [this](boost::system::error_code ec)
            {
                if (!ec) send_request();
            });
    }

    void send_request()
    {
        std::string request = "GET http://" + host_ + ":" + std::to_string(port_) + "?url=udp://" + 
            stream_host + ":" + stream_port + "";
        socket_.send(asio::buffer(request));
    }

private:

    asio::io_service& io_;
    tcp::socket socket_;

    std::string host_;
    int port_;
};

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <server host> <server port> <stream host> <stream port>\n";
        return -1;
    }

    try
    {
        asio::io_service io;

        stream_host = argv[3];
        stream_port = argv[4];

        http_client client(io, argv[1], std::stoi(argv[2]));
        client.run();

        io.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}