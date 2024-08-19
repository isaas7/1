#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <queue>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <sqlite3.h>  // Include SQLite header
#include <SQLiteCpp/SQLiteCpp.h>

#include "../../http/include/client.hpp"
#include "../../log/include/log.hpp"

/**
 * @brief The Application class encapsulates the main logic of the application.
 * 
 * This class is responsible for managing interactions with the Large Language Model (LLM) 
 * through the Ollama API. It handles submitting prompts, receiving responses, and managing 
 * the state of each query. It also provides methods to check the status of ongoing queries 
 * and to cancel them if necessary.
 */
class Application {
public:
    /**
     * @brief Constructs an Application object.
     * 
     * @param ioc The Boost.Asio I/O context that the application will use for asynchronous operations.
     */
    Application(boost::asio::io_context& ioc, ssl::context& ssl_ctx);

    /**
     * @brief Destructor for Application class to close SQLite connection.
     */
    ~Application();


private:
    boost::asio::io_context& io_context_;  ///< Reference to the I/O context used for async operations.
    ssl::context& ssl_ctx_;

};

#endif // APPLICATION_HPP

