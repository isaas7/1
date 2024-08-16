#include "../include/server.hpp"
#include "../include/session.hpp"

/**
 * @brief Constructs a server object.
 * 
 * Initializes the server with the provided io_context, SSL context, endpoint, and document root.
 * Configures the acceptor for incoming connections.
 * 
 * @param ioc The io_context to be used for asynchronous operations.
 * @param ctx The SSL context used for managing SSL connections.
 * @param endpoint The endpoint on which the server will accept connections.
 * @param doc_root The document root directory for serving files.
 */
server::server(
    boost::asio::io_context& ioc,
    boost::asio::ssl::context& ctx,
    boost::asio::ip::tcp::endpoint endpoint,
    std::shared_ptr<std::string const> const& doc_root,
    std::shared_ptr<Application> app)
    : ioc_(ioc)
    , ctx_(ctx)
    , acceptor_(ioc)
    , doc_root_(doc_root)
    , app_(app)
{
    auto logger = LoggerManager::getLogger("server_logger", LogLevel::INFO, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing server.");

    boost::beast::error_code ec;

    // Open the acceptor to allow incoming connections.
    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        logger->log(LogLevel::ERROR, "Error opening acceptor: " + ec.message());
        return;
    }

    logger->log(LogLevel::DEBUG, "Acceptor opened successfully.");

    // Allow the socket to be reused after the server is closed.
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        logger->log(LogLevel::ERROR, "Error setting socket option: " + ec.message());
        return;
    }

    logger->log(LogLevel::DEBUG, "Socket option set for address reuse.");

    // Bind the acceptor to the specified endpoint.
    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        logger->log(LogLevel::ERROR, "Error binding acceptor: " + ec.message());
        return;
    }

    logger->log(LogLevel::DEBUG, "Acceptor bound to endpoint.");

    // Start listening for incoming connections.
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        logger->log(LogLevel::ERROR, "Error starting listener: " + ec.message());
        return;
    }

    logger->log(LogLevel::DEBUG, "Server listening for connections.");
}

/**
 * @brief Starts the server's asynchronous operations.
 * 
 * Initiates the process of accepting incoming connections.
 */
void server::run()
{
    auto logger = LoggerManager::getLogger("server_logger");
    logger->log(LogLevel::DEBUG, "Running server.");
    do_accept();
}

/**
 * @brief Asynchronously accepts incoming connections.
 * 
 * This method initiates an asynchronous accept operation to wait for new client connections.
 * When a connection is accepted, the on_accept handler is invoked.
 */
void server::do_accept()
{
    auto logger = LoggerManager::getLogger("server_logger");
    logger->log(LogLevel::DEBUG, "Waiting for connections...");

    acceptor_.async_accept(
        boost::asio::make_strand(ioc_),
        boost::beast::bind_front_handler(
            &server::on_accept,
            shared_from_this()));
}

/**
 * @brief Handles the completion of an asynchronous accept operation.
 * 
 * If a connection is successfully accepted, this method creates a new session to handle the connection.
 * If there is an error, it is logged.
 * 
 * @param ec The error code, if any, resulting from the accept operation.
 * @param socket The socket associated with the accepted connection.
 */
void server::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
{
    auto logger = LoggerManager::getLogger("server_logger");

    if (!ec)
    {
        logger->log(LogLevel::DEBUG, "Connection accepted.");
        
        // Create a new session and start it
        std::make_shared<session>(std::move(socket), ctx_, doc_root_, app_)->run();
    }
    else
    {
        logger->log(LogLevel::ERROR, "Error accepting connection: " + ec.message());
    }

    // Accept another connection
    do_accept();
}

