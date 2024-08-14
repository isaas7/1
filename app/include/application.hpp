#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../../net/include/client.hpp"
#include "../../log/include/log.hpp" // Include the logger

class Application {
public:
    /**
     * @brief Constructs an Application object.
     */
    Application();

    /**
     * @brief Performs an HTTP GET request to localhost:8080/.
     * 
     * @return The response body as a string.
     */
    std::string get();

private:
    boost::asio::io_context io_context_;  // The I/O context used for network operations.
    ssl::context ssl_ctx_;                // The SSL context used for managing SSL connections.
    Client client_;                       // The client object used to send HTTP requests.
    std::shared_ptr<Logger> logger_;      // Logger instance for the Application class
};

#endif // APPLICATION_HPP

