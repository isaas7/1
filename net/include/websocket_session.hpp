#ifndef WEBSOCKET_SESSION_HPP
#define WEBSOCKET_SESSION_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "../../log/include/log.hpp" // Include the logger

using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;  // from <boost/asio/ssl.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

class websocket_session : public std::enable_shared_from_this<websocket_session>
{
    websocket::stream<ssl::stream<tcp::socket>> ws_;
    boost::beast::flat_buffer buffer_;
    std::shared_ptr<Logger> logger_;

public:
    // Constructor to take ownership of the socket
    websocket_session(tcp::socket socket, ssl::context& ctx);

    // Start the asynchronous operation
    void run();

private:
    void on_handshake(boost::system::error_code ec);
    void on_accept(boost::system::error_code ec);
    void do_read();
    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);
};

#endif // WEBSOCKET_SESSION_HPP

