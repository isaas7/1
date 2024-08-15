#include "net/include/ollama.hpp"
#include "net/include/utils.hpp"
#include "net/include/server_certificate.hpp"
#include "net/include/http_tools.hpp"
#include "net/include/server.hpp"
#include "app/include/application.hpp"
#include "log/include/log.hpp" // Include the logger
#include "net/include/websocket_listener.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <memory>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
    auto logger = LoggerManager::getLogger("MainLogger", LogLevel::DEBUG, LogOutput::CONSOLE);
    
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
    
    /*
     * OLLAMA EXPERIMENTATION
     * The for loop currently displays the available models.
     * Generating responses does not work
     */
    try { 
        std::vector<std::string> models = ollama::list_models();
        for(int i = 0; i < models.size(); i++) {
            std::cout << models.at(i) << std::endl;
        }

        // Attempt to generate a response from the model
        //std::string response = ollama::generate("llava:latest", "Why is the sky blue?");
        //std::cout << "Generated response: " << response << std::endl;
    } 
    catch(ollama::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // END OF OLLAMA EXPERIMENTATION BLOCK

    // Initialize the io_context
    logger->log(LogLevel::DEBUG, "Initializing io_context.");
    net::io_context ioc{threads};

    // Initialize SSL context
    logger->log(LogLevel::DEBUG, "Initializing SSL context.");
    ssl::context ctx{ssl::context::tlsv12};
    load_server_certificate(ctx);

    // Initialize the Application
    auto app = std::make_shared<Application>(ioc);

    // Create loggers for WebSocket listener and session
    auto websocket_listener_logger = LoggerManager::getLogger("WebSocketListenerLogger", LogLevel::DEBUG, LogOutput::CONSOLE);
    auto websocket_session_logger = LoggerManager::getLogger("WebSocketSessionLogger", LogLevel::DEBUG, LogOutput::CONSOLE);

    // Start the WebSocket listener to accept incoming WebSocket connections
    logger->log(LogLevel::DEBUG, "Starting the WebSocket listener.");
    auto websocket_instance = std::make_shared<websocket_listener>(
        ioc,
        ctx,
        tcp::endpoint{address, static_cast<unsigned short>(port + 1)},
        websocket_listener_logger,
        websocket_session_logger // Pass the session logger to the listener, which will pass it to sessions
    );
    websocket_instance->run();

    // Start the server to accept incoming connections
    logger->log(LogLevel::DEBUG, "Starting the HTTP server.");
    auto server_instance = std::make_shared<server>(
        ioc,
        ctx,
        tcp::endpoint{address, port},
        doc_root);
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

