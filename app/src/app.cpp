#include "../include/app.hpp"
#include "../../log/include/log.hpp"
#include <vector>
#include <numeric>
#include <sqlite3.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

/**
 * @brief Constructs an Application object and starts the query processing thread.
 * 
 * @param ioc The Boost.Asio I/O context that the application will use for asynchronous operations.
 * @param ssl_ctx The SSL context used for managing SSL connections.
 */
Application::Application(boost::asio::io_context& ioc, ssl::context& ssl_ctx) : io_context_(ioc), ssl_ctx_(ssl_ctx) { 
    auto logger = LoggerManager::getLogger("app_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing app.");
}

/**
 * @brief Destructor to close the database connection.
 */
Application::~Application() {}


