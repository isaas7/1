#include "../include/websocket_session.hpp"
#include <boost/beast/core/bind_handler.hpp>
#include "../include/utils.hpp"
websocket_session::websocket_session(tcp::socket socket, ssl::context& ctx)
    : ws_(std::move(socket), ctx), logger_(LoggerManager::getLogger("WebSocketSession"))
{
    logger_->log(LogLevel::DEBUG, "WebSocket session created.");
}

void websocket_session::run()
{
    logger_->log(LogLevel::DEBUG, "Starting WebSocket session.");
    ws_.next_layer().async_handshake(
        ssl::stream_base::server,
        boost::beast::bind_front_handler(
            &websocket_session::on_handshake,
            shared_from_this()));
}

void websocket_session::on_handshake(boost::system::error_code ec)
{
    if (ec) {
        logger_->log(LogLevel::ERROR, "SSL handshake failed: " + ec.message());
        return fail(ec, "handshake");
    }

    logger_->log(LogLevel::DEBUG, "SSL handshake successful.");
    ws_.async_accept(
        boost::beast::bind_front_handler(
            &websocket_session::on_accept,
            shared_from_this()));
}

void websocket_session::on_accept(boost::system::error_code ec)
{
    if (ec) {
        logger_->log(LogLevel::ERROR, "WebSocket accept failed: " + ec.message());
        return fail(ec, "accept");
    }

    logger_->log(LogLevel::DEBUG, "WebSocket handshake successful. Waiting for messages.");
    do_read();
}

void websocket_session::do_read()
{
    ws_.async_read(
        buffer_,
        boost::beast::bind_front_handler(
            &websocket_session::on_read,
            shared_from_this()));
}

void websocket_session::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed) {
        logger_->log(LogLevel::DEBUG, "WebSocket connection closed by client.");
        return;
    }

    if (ec) {
        logger_->log(LogLevel::ERROR, "Error reading WebSocket message: " + ec.message());
        return fail(ec, "read");
    }

    std::string received_message(boost::beast::buffers_to_string(buffer_.data()));
    logger_->log(LogLevel::DEBUG, "Received WebSocket message: " + received_message);

    ws_.text(ws_.got_text());
    ws_.async_write(
        buffer_.data(),
        boost::beast::bind_front_handler(
            &websocket_session::on_write,
            shared_from_this()));
}

void websocket_session::on_write(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        logger_->log(LogLevel::ERROR, "Error writing WebSocket message: " + ec.message());
        return fail(ec, "write");
    }

    logger_->log(LogLevel::DEBUG, "WebSocket message sent successfully. Clearing buffer and reading next message.");

    buffer_.consume(buffer_.size());
    do_read();
}

