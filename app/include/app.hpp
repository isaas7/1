#ifndef APP_HPP
#define APP_HPP

#include <queue>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "../../http/include/client.hpp"
#include "../../log/include/log.hpp"
#include "../../db/include/db.hpp"

/**
 * @brief The Application class encapsulates the main logic of the application.
 * 
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
    Database db_; ///< Instance of the Database class       
};

#endif // APP_HPP

