#include "../include/application.hpp"
#include "../../log/include/log.hpp" // Include the logger

/**
 * @brief Constructs an Application object and starts the recurring GET request timer.
 * 
 * Initializes the SSL context, the client object, and starts the timer for recurring GET requests.
 */
Application::Application(boost::asio::io_context& ioc)
    : io_context_(ioc), ssl_ctx_(ssl::context::tlsv12_client), client_(io_context_, ssl_ctx_), timer_(io_context_)
{
    // Start the recurring timer when the application is constructed
    start_timer();
}

/**
 * @brief Performs an HTTP GET request to localhost:8080/.
 * 
 * This method uses the Client object to send an HTTP GET request to the specified URL.
 * 
 * @return The response body as a string.
 */
std::string Application::get() {
    return client_.get("localhost", "8080", "/");
}

/**
 * @brief Starts a recurring timer that executes a GET request every 10 seconds.
 */
void Application::start_timer() {
    auto logger = LoggerManager::getLogger("ApplicationLogger", LogLevel::INFO, LogOutput::CONSOLE);

    timer_.expires_after(std::chrono::seconds(60));
    timer_.async_wait([this, logger](const boost::system::error_code& ec) {
        if (!ec) {
            logger->log(LogLevel::INFO, "Executing GET request after 10 seconds delay.");
            std::string response = get();
            logger->log(LogLevel::INFO, "Received response: " + response);

            // Restart the timer for the next iteration
            start_timer();
        } else {
            logger->log(LogLevel::ERROR, "Timer error: " + ec.message());
        }
    });
}

