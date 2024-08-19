#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../app/include/app.hpp"
#include "../../log/include/log.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

/**
 * @file server.hpp
 * @brief Defines the server class for handling incoming HTTP/HTTPS connections.
 *
 * This class sets up a server that listens for incoming TCP connections,
 * negotiates SSL/TLS using Boost.Beast and Boost.Asio, and then dispatches
 * these connections to a session for processing.
 */

/**
 * @class server
 * @brief Manages the lifecycle of incoming connections, including accepting
 * and dispatching them to sessions.
 *
 * The server class is responsible for:
 * - Setting up and managing a TCP acceptor.
 * - Handling SSL/TLS contexts.
 * - Creating sessions for each accepted connection.
 * - Running the server in a multi-threaded context.
 */
class server : public std::enable_shared_from_this<server>
{
    boost::asio::io_context& ioc_;  ///< Reference to the I/O context used for asynchronous operations.
    boost::asio::ssl::context& ctx_;  ///< Reference to the SSL context used for managing SSL/TLS settings.
    boost::asio::ip::tcp::acceptor acceptor_;  ///< Acceptor object used to listen for incoming connections.
    std::shared_ptr<std::string const> doc_root_;  ///< Shared pointer to the document root directory.
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<Application> app_;
public:
    /**
     * @brief Constructs the server object.
     * 
     * @param ioc Reference to the I/O context for handling asynchronous operations.
     * @param ctx Reference to the SSL context for managing SSL/TLS settings.
     * @param endpoint The TCP endpoint (address and port) where the server will listen for connections.
     * @param doc_root Shared pointer to the document root directory for serving static files.
     * @param app Shared pointer to the application instance.
     */
    server(
        boost::asio::io_context& ioc,
        boost::asio::ssl::context& ctx,
        boost::asio::ip::tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root,
        std::shared_ptr<Application> app);

    /**
     * @brief Starts the server to begin accepting incoming connections.
     *
     * This method triggers the asynchronous acceptance of connections
     * and continues accepting them until the server is stopped.
     */
    void run();

private:
    /**
     * @brief Asynchronously waits for a new connection to accept.
     *
     * This method is called internally by `run()` and after each accepted
     * connection to keep the server continually accepting new connections.
     */
    void do_accept();

    /**
     * @brief Handles the completion of an asynchronous accept operation.
     *
     * This method is called when a connection is accepted. It creates a
     * new session for the accepted connection and begins processing it.
     *
     * @param ec The result of the accept operation (indicates success or error).
     * @param socket The socket representing the accepted connection.
     */
    void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);
};

#endif // SERVER_HPP

