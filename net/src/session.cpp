#include "../include/session.hpp"
#include "../include/http_tools.hpp"
#include "../include/utils.hpp"
#include "../../log/include/log.hpp"

/**
 * @brief Constructs a session object.
 * 
 * Initializes the session with the given socket, SSL context, and document root.
 * 
 * @param socket The socket for the session.
 * @param ctx The SSL context for managing SSL connections.
 * @param doc_root The document root directory for serving files.
 */
session::session(
    tcp::socket&& socket,
    ssl::context& ctx,
    std::shared_ptr<std::string const> const& doc_root)
    : stream_(std::move(socket), ctx)
    , doc_root_(doc_root)
{
    auto logger = LoggerManager::getLogger("session_logger", LogLevel::DEBUG);
    logger->log(LogLevel::DEBUG, "Session created.");
}

/**
 * @brief Starts the session by initiating the SSL handshake.
 */
void session::run()
{
    auto logger = LoggerManager::getLogger("session_logger");
    logger->log(LogLevel::DEBUG, "Running session.");

    net::dispatch(
        stream_.get_executor(),
        beast::bind_front_handler(
            &session::on_run,
            shared_from_this()));
}

/**
 * @brief Handles the asynchronous run operation.
 * 
 * This method is called after the session is dispatched and starts the SSL handshake.
 */
void session::on_run()
{
    auto logger = LoggerManager::getLogger("session_logger");
    logger->log(LogLevel::DEBUG, "Starting SSL handshake.");

    beast::get_lowest_layer(stream_).expires_after(
        std::chrono::seconds(30));

    stream_.async_handshake(
        ssl::stream_base::server,
        beast::bind_front_handler(
            &session::on_handshake,
            shared_from_this()));
}

/**
 * @brief Handles the SSL handshake completion.
 * 
 * This method is called when the SSL handshake is complete.
 * 
 * @param ec The error code, if any, from the handshake operation.
 */
void session::on_handshake(boost::beast::error_code ec)
{
    auto logger = LoggerManager::getLogger("session_logger");

    if(ec) {
        logger->log(LogLevel::ERROR, "Handshake failed: " + ec.message());
        return fail(ec, "handshake");
    }

    logger->log(LogLevel::DEBUG, "Handshake successful.");
    do_read();
}

/**
 * @brief Reads an HTTP request from the client.
 * 
 * Initiates an asynchronous read operation to receive the client's HTTP request.
 */
void session::do_read()
{
    auto logger = LoggerManager::getLogger("session_logger");
    logger->log(LogLevel::DEBUG, "Reading request.");

    req_ = {};

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    http::async_read(stream_, buffer_, req_,
        beast::bind_front_handler(
            &session::on_read,
            shared_from_this()));
}

/**
 * @brief Handles the completion of the asynchronous read operation.
 * 
 * Processes the received HTTP request or closes the session if an error occurs.
 * 
 * @param ec The error code, if any, from the read operation.
 * @param bytes_transferred The number of bytes transferred during the read.
 */
void session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    auto logger = LoggerManager::getLogger("session_logger");

    if(ec == http::error::end_of_stream) {
        logger->log(LogLevel::DEBUG, "End of stream detected, closing session.");
        return do_close();
    }

    if(ec) {
        logger->log(LogLevel::ERROR, "Error reading request: " + ec.message());
        return fail(ec, "read");
    }

    logger->log(LogLevel::DEBUG, "Request received successfully.");
    send_response(
        handle_request(*doc_root_, std::move(req_)));
}

/**
 * @brief Sends an HTTP response to the client.
 * 
 * @param msg The HTTP response to send.
 */
void session::send_response(http::message_generator&& msg)
{
    auto logger = LoggerManager::getLogger("session_logger");
    logger->log(LogLevel::DEBUG, "Sending response.");

    bool keep_alive = msg.keep_alive();

    beast::async_write(
        stream_,
        std::move(msg),
        beast::bind_front_handler(
            &session::on_write, this->shared_from_this(), keep_alive));
}

/**
 * @brief Handles the completion of the asynchronous write operation.
 * 
 * Determines whether to keep the connection alive or close it.
 * 
 * @param keep_alive Whether to keep the connection alive.
 * @param ec The error code, if any, from the write operation.
 * @param bytes_transferred The number of bytes transferred during the write.
 */
void session::on_write(bool keep_alive, boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    auto logger = LoggerManager::getLogger("session_logger");

    if(ec) {
        logger->log(LogLevel::ERROR, "Error writing response: " + ec.message());
        return fail(ec, "write");
    }

    logger->log(LogLevel::DEBUG, "Response sent successfully.");

    if(!keep_alive)
    {
        logger->log(LogLevel::DEBUG, "Connection will be closed.");
        return do_close();
    }

    do_read();
}

/**
 * @brief Closes the session.
 * 
 * Initiates the SSL shutdown process and closes the connection.
 */
void session::do_close()
{
    auto logger = LoggerManager::getLogger("session_logger");
    logger->log(LogLevel::DEBUG, "Closing session.");

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    stream_.async_shutdown(
        beast::bind_front_handler(
            &session::on_shutdown,
            shared_from_this()));
}

/**
 * @brief Handles the completion of the asynchronous shutdown operation.
 * 
 * Finalizes the session closure after the SSL shutdown is complete.
 * 
 * @param ec The error code, if any, from the shutdown operation.
 */
void session::on_shutdown(boost::beast::error_code ec)
{
    auto logger = LoggerManager::getLogger("session_logger");

    if(ec) {
        logger->log(LogLevel::ERROR, "Error during shutdown: " + ec.message());
        return fail(ec, "shutdown");
    }

    logger->log(LogLevel::DEBUG, "Shutdown completed.");
}

