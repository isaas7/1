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

/**
 * @class websocket_session
 * @brief Handles an individual WebSocket connection.
 *
 * This class is responsible for managing a single WebSocket connection, including
 * performing the SSL handshake, reading and writing WebSocket messages, and
 * logging all relevant events.
 */
class websocket_session : public std::enable_shared_from_this<websocket_session>
{
    websocket::stream<ssl::stream<tcp::socket>> ws_;  ///< WebSocket stream with SSL.
    boost::beast::flat_buffer buffer_;                ///< Buffer for incoming messages.
    std::shared_ptr<Logger> logger_;                  ///< Logger instance for the session.

public:
    /**
     * @brief Constructor to take ownership of the socket.
     * @param socket TCP socket.
     * @param ctx SSL context.
     * @param logger Logger instance for the session.
     */
    websocket_session(tcp::socket socket, ssl::context& ctx, std::shared_ptr<Logger> logger);

    /**
     * @brief Start the asynchronous WebSocket session.
     */
    void run();

private:
    void on_handshake(boost::system::error_code ec);
    void on_accept(boost::system::error_code ec);
    void do_read();
    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);
};

#endif // WEBSOCKET_SESSION_HPP

