#ifndef WEBSOCKET_LISTENER_HPP
#define WEBSOCKET_LISTENER_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "../../log/include/log.hpp" // Include the logger

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

class websocket_listener : public std::enable_shared_from_this<websocket_listener>
{
    ssl::context& ctx_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::shared_ptr<Logger> logger_;

public:
    websocket_listener(
        boost::asio::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint);

    // Start accepting incoming connections
    void run();

private:
    void do_accept();
    void on_accept(boost::system::error_code ec);
};

#endif // WEBSOCKET_LISTENER_HPP

