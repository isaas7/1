#include "../include/application.hpp"

/**
 * @brief Constructs an Application object.
 * 
 * Initializes the SSL context and the client object.
 */
Application::Application()
    : ssl_ctx_(ssl::context::tlsv12_client), client_(io_context_, ssl_ctx_), logger_(LoggerManager::getLogger("ApplicationLogger", LogLevel::DEBUG))
{
    logger_->log(LogLevel::INFO, "Application initialized.");
}

/**
 * @brief Performs an HTTP GET request to localhost:8080/.
 * 
 * This method uses the Client object to send an HTTP GET request to the specified URL.
 * 
 * @return The response body as a string.
 */
std::string Application::get() {
    logger_->log(LogLevel::INFO, "Sending GET request to localhost:8080/");
    std::string response = client_.get("localhost", "8080", "/");
    logger_->log(LogLevel::INFO, "Received response: " + response);
    return response;
}

