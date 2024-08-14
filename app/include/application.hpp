#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../../net/include/client.hpp"
#include <boost/asio/steady_timer.hpp>

class Application {
public:
    /**
     * @brief Constructs an Application object and starts the recurring GET request timer.
     */
    Application(boost::asio::io_context& ioc);

    /**
     * @brief Performs an HTTP GET request to localhost:8080/.
     * 
     * @return The response body as a string.
     */
    std::string get();

private:
    /**
     * @brief Starts a recurring timer that executes a GET request every 10 seconds.
     */
    void start_timer();

    boost::asio::io_context& io_context_;  // The I/O context used for network operations.
    ssl::context ssl_ctx_;                // The SSL context used for managing SSL connections.
    Client client_;                       // The client object used to send HTTP requests.
    boost::asio::steady_timer timer_;     // Timer to schedule recurring tasks.
};

#endif // APPLICATION_HPP

