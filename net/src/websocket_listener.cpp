#include "../include/websocket_listener.hpp"
#include "../include/utils.hpp"
#include <boost/beast/core.hpp>

/**
 * @brief Constructor for the websocket_listener class.
 * 
 * Initializes the WebSocket listener, sets up the SSL context, and configures 
 * the TCP acceptor for incoming connections.
 * 
 * @param ioc The I/O context used for asynchronous operations.
 * @param ctx The SSL context used for managing SSL connections.
 * @param endpoint The TCP endpoint where the listener will bind and listen for connections.
 * @param logger The logger instance used for logging events in the listener.
 * @param session_logger The logger instance used for logging events in each WebSocket session.
 */
websocket_listener::websocket_listener(
    boost::asio::io_context& ioc,
    ssl::context& ctx,
    tcp::endpoint endpoint,
    std::shared_ptr<Logger> logger,
    std::shared_ptr<Logger> session_logger)
    : ctx_(ctx) // Initialize SSL context reference
    , acceptor_(ioc) // Initialize TCP acceptor with the I/O context
    , socket_(ioc) // Initialize TCP socket with the I/O context
    , logger_(logger) // Assign the logger instance for the listener
    , session_logger_(session_logger) // Assign the logger instance for the sessions
{
    boost::system::error_code ec;

    // Open the acceptor to allow it to listen for incoming connections
    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        // Log the error and fail the operation if opening the acceptor fails
        logger_->log(LogLevel::ERROR, "Failed to open acceptor: " + ec.message());
        fail(ec, "open");
        return;
    }

    // Set the option to allow address reuse, enabling multiple connections to the same address
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        // Log the error and fail the operation if setting the option fails
        logger_->log(LogLevel::ERROR, "Failed to set acceptor option: " + ec.message());
        fail(ec, "set_option");
        return;
    }

    // Bind the acceptor to the specified endpoint (address and port)
    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        // Log the error and fail the operation if binding fails
        logger_->log(LogLevel::ERROR, "Failed to bind acceptor: " + ec.message());
        fail(ec, "bind");
        return;
    }

    // Start listening for incoming connections with a maximum backlog size
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        // Log the error and fail the operation if listening fails
        logger_->log(LogLevel::ERROR, "Failed to listen on acceptor: " + ec.message());
        fail(ec, "listen");
        return;
    }

    // Log the successful start of the WebSocket listener
    logger_->log(LogLevel::DEBUG, "WebSocket listener started on " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()));
}

/**
 * @brief Starts the WebSocket listener to accept incoming connections.
 * 
 * This method begins the asynchronous accept loop that will handle new connections 
 * and create sessions for each WebSocket client.
 */
void websocket_listener::run()
{
    if (!acceptor_.is_open())
        return;

    // Log that the listener is ready to accept connections
    logger_->log(LogLevel::DEBUG, "WebSocket listener is ready to accept connections.");
    do_accept(); // Start the asynchronous accept loop
}

/**
 * @brief Asynchronously accepts incoming connections.
 * 
 * This method is called repeatedly to accept new connections. It uses the acceptor 
 * to asynchronously accept a new connection, which will be handled by the on_accept method.
 */
void websocket_listener::do_accept()
{
    // Start an asynchronous accept operation
    acceptor_.async_accept(
        socket_,
        std::bind(
            &websocket_listener::on_accept,
            shared_from_this(),
            std::placeholders::_1));
}

/**
 * @brief Handles the completion of an asynchronous accept operation.
 * 
 * This method is called when a new connection is accepted. It logs the result 
 * of the accept operation and, if successful, creates and runs a new WebSocket session.
 * 
 * @param ec The error code from the accept operation, if any.
 */
void websocket_listener::on_accept(boost::system::error_code ec)
{
    if (ec)
    {
        // Log the error if accepting the connection failed
        logger_->log(LogLevel::ERROR, "Error accepting connection: " + ec.message());
        fail(ec, "accept");
    }
    else
    {
        // Log the successful connection acceptance
        logger_->log(LogLevel::DEBUG, "Connection accepted. Starting WebSocket session.");

        // Create and run a new WebSocket session, passing the session logger
        std::make_shared<websocket_session>(std::move(socket_), ctx_, session_logger_)->run();
    }

    // Continue accepting more connections
    do_accept();
}

