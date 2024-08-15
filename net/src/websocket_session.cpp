#include "../include/websocket_session.hpp"
#include "../include/utils.hpp"
#include <boost/beast/core/bind_handler.hpp>

/**
 * @brief Constructor for the websocket_session class.
 * 
 * Initializes a WebSocket session by taking ownership of the socket and setting up
 * the SSL context and logger.
 * 
 * @param socket The TCP socket associated with the session.
 * @param ctx The SSL context for handling SSL connections.
 * @param logger The logger instance for logging events in this session.
 */
websocket_session::websocket_session(tcp::socket socket, ssl::context& ctx, std::shared_ptr<Logger> logger)
    : ws_(std::move(socket), ctx), logger_(logger)
{
    // Log the creation of the WebSocket session
    logger_->log(LogLevel::DEBUG, "WebSocket session created.");
}

/**
 * @brief Starts the WebSocket session.
 * 
 * This method initiates the SSL handshake, which is the first step in establishing a secure WebSocket connection.
 */
void websocket_session::run()
{
    logger_->log(LogLevel::DEBUG, "Starting WebSocket session.");

    // Perform the SSL handshake asynchronously
    ws_.next_layer().async_handshake(
        ssl::stream_base::server,
        boost::beast::bind_front_handler(
            &websocket_session::on_handshake,
            shared_from_this()));
}

/**
 * @brief Handles the completion of the SSL handshake.
 * 
 * This method is called when the SSL handshake is complete. It checks for errors and, if successful,
 * proceeds to accept the WebSocket handshake.
 * 
 * @param ec The error code from the handshake operation, if any.
 */
void websocket_session::on_handshake(boost::system::error_code ec)
{
    if (ec) {
        // Log the error if the SSL handshake fails
        logger_->log(LogLevel::ERROR, "SSL handshake failed: " + ec.message());
        return fail(ec, "handshake");
    }

    // Log the success of the SSL handshake
    logger_->log(LogLevel::DEBUG, "SSL handshake successful.");

    // Accept the WebSocket handshake asynchronously
    ws_.async_accept(
        boost::beast::bind_front_handler(
            &websocket_session::on_accept,
            shared_from_this()));
}

/**
 * @brief Handles the completion of the WebSocket handshake.
 * 
 * This method is called when the WebSocket handshake is complete. It checks for errors and, if successful,
 * begins reading messages from the WebSocket.
 * 
 * @param ec The error code from the WebSocket accept operation, if any.
 */
void websocket_session::on_accept(boost::system::error_code ec)
{
    if (ec) {
        // Log the error if the WebSocket accept fails
        logger_->log(LogLevel::ERROR, "WebSocket accept failed: " + ec.message());
        return fail(ec, "accept");
    }

    // Log the success of the WebSocket handshake
    logger_->log(LogLevel::DEBUG, "WebSocket handshake successful. Waiting for messages.");

    // Start reading messages from the WebSocket
    do_read();
}

/**
 * @brief Initiates an asynchronous read operation from the WebSocket.
 * 
 * This method begins the process of reading a message from the WebSocket connection.
 */
void websocket_session::do_read()
{
    ws_.async_read(
        buffer_,
        boost::beast::bind_front_handler(
            &websocket_session::on_read,
            shared_from_this()));
}

/**
 * @brief Handles the completion of a WebSocket read operation.
 * 
 * This method processes the data read from the WebSocket. If the connection is still open,
 * it logs the received message and sends it back to the client.
 * 
 * @param ec The error code from the read operation, if any.
 * @param bytes_transferred The number of bytes transferred during the read.
 */
void websocket_session::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed) {
        // Log if the WebSocket connection was closed by the client
        logger_->log(LogLevel::DEBUG, "WebSocket connection closed by client.");
        return;
    }

    if (ec) {
        // Log any other error that occurred during reading
        logger_->log(LogLevel::ERROR, "Error reading WebSocket message: " + ec.message());
        return fail(ec, "read");
    }

    // Convert the received buffer to a string for logging
    std::string received_message(boost::beast::buffers_to_string(buffer_.data()));
    logger_->log(LogLevel::DEBUG, "Received WebSocket message: " + received_message);

    // Echo the message back to the client
    ws_.text(ws_.got_text());
    ws_.async_write(
        buffer_.data(),
        boost::beast::bind_front_handler(
            &websocket_session::on_write,
            shared_from_this()));
}

/**
 * @brief Handles the completion of a WebSocket write operation.
 * 
 * This method processes the result of writing data to the WebSocket. If successful, 
 * it clears the buffer and initiates another read operation.
 * 
 * @param ec The error code from the write operation, if any.
 * @param bytes_transferred The number of bytes transferred during the write.
 */
void websocket_session::on_write(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        // Log any error that occurred during writing
        logger_->log(LogLevel::ERROR, "Error writing WebSocket message: " + ec.message());
        return fail(ec, "write");
    }

    // Log the success of the write operation
    logger_->log(LogLevel::DEBUG, "WebSocket message sent successfully. Clearing buffer and reading next message.");

    // Clear the buffer to prepare for the next message
    buffer_.consume(buffer_.size());

    // Start reading the next message
    do_read();
}

