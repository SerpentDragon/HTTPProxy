#include <iostream>
#include "server/http_server.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return EXIT_FAILURE;
    }

    try
    {
        asio::io_service io;

        http_server server(io, tcp::endpoint(asio::ip::address::from_string(argv[1]), std::stoi(argv[2])));
        server.run();

        io.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}
