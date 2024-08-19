#include "log/include/log.hpp" // Include the logger
#include "http/include/utils.hpp"
#include "http/include/server_certificate.hpp"
#include "http/include/http_tools.hpp"
#include "http/include/server.hpp"
#include "http/include/client.hpp"
#include "app/include/app.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <memory>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
    auto logger = LoggerManager::getLogger("main_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    
    if (argc != 5)
    {
        logger->log(LogLevel::ERROR, "Invalid number of arguments.");
        return EXIT_FAILURE;
    }

    logger->log(LogLevel::DEBUG, "Parsing command line arguments.");
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // Initialize the io_context
    logger->log(LogLevel::DEBUG, "Initializing io_context.");
    net::io_context ioc{threads};

    // Initialize SSL context
    logger->log(LogLevel::DEBUG, "Initializing SSL context.");
    ssl::context ctx{ssl::context::tlsv12};
    load_server_certificate(ctx);
    // Initialize the Application
    auto app = std::make_shared<Application>(ioc, ctx);
    // Start the server to accept incoming connections
    logger->log(LogLevel::DEBUG, "Starting the HTTP server.");
    auto server_instance = std::make_shared<server>(
        ioc,
        ctx,
        tcp::endpoint{address, port},
        doc_root,
        app);
    server_instance->run();

    // Run the I/O context in multiple threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back([&ioc, &logger] {
            logger->log(LogLevel::DEBUG, "Running io_context in a thread.");
            ioc.run(); 
        });
    
    // Run the I/O context in the main thread
    logger->log(LogLevel::DEBUG, "Running io_context in the main thread.");
    ioc.run();

    return EXIT_SUCCESS;
}

