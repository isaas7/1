#include "../include/websocket_listener.hpp"
#include "../include/websocket_session.hpp" // Include the WebSocket session header
#include "../include/utils.hpp"
#include <boost/system/error_code.hpp>

websocket_listener::websocket_listener(
    boost::asio::io_context& ioc,
    ssl::context& ctx,
    tcp::endpoint endpoint)
    : ctx_(ctx)
    , acceptor_(ioc)
    , socket_(ioc)
    , logger_(LoggerManager::getLogger("WebSocketListener"))
{
    boost::system::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        logger_->log(LogLevel::ERROR, "Failed to open acceptor: " + ec.message());
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        logger_->log(LogLevel::ERROR, "Failed to set acceptor option: " + ec.message());
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        logger_->log(LogLevel::ERROR, "Failed to bind acceptor: " + ec.message());
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        logger_->log(LogLevel::ERROR, "Failed to listen on acceptor: " + ec.message());
        fail(ec, "listen");
        return;
    }

    logger_->log(LogLevel::DEBUG, "WebSocket listener started on " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()));
}

void websocket_listener::run()
{
    if (!acceptor_.is_open())
        return;
    logger_->log(LogLevel::DEBUG, "WebSocket listener is ready to accept connections.");
    do_accept();
}

void websocket_listener::do_accept()
{
    acceptor_.async_accept(
        socket_,
        std::bind(
            &websocket_listener::on_accept,
            shared_from_this(),
            std::placeholders::_1));
}

void websocket_listener::on_accept(boost::system::error_code ec)
{
    if (ec)
    {
        logger_->log(LogLevel::ERROR, "Error accepting connection: " + ec.message());
        fail(ec, "accept");
    }
    else
    {
        logger_->log(LogLevel::DEBUG, "Connection accepted. Starting WebSocket session.");
        // Create the session and run it
        std::make_shared<websocket_session>(std::move(socket_), ctx_)->run();
    }

    // Accept another connection
    do_accept();
}

