#ifndef WEBSOCKET_LISTENER_HPP
#define WEBSOCKET_LISTENER_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "websocket_session.hpp"
#include "../../log/include/log.hpp" // Include the logger

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

/**
 * @class websocket_listener
 * @brief Listens for incoming WebSocket connections and spawns sessions to handle them.
 *
 * This class is responsible for accepting incoming TCP connections,
 * performing SSL handshakes, and creating new WebSocket sessions to handle
 * each connection. Each session will be passed its own logger instance to
 * ensure detailed logging.
 * 
 * @note The decision to run the WebSocket server independently from the HTTP
 * server was made to simplify the compilation and execution process. Integrating
 * both servers into one was challenging due to complexities in the templates 
 * so an independent WebSocket server was chosen for its simplicity.
 */
class websocket_listener : public std::enable_shared_from_this<websocket_listener>
{
    ssl::context& ctx_;                       ///< SSL context for secure communication.
    tcp::acceptor acceptor_;                  ///< Acceptor for incoming connections.
    tcp::socket socket_;                      ///< Socket for the current connection.
    std::shared_ptr<Logger> logger_;          ///< Logger instance for the listener.
    std::shared_ptr<Logger> session_logger_;  ///< Logger instance for WebSocket sessions.

public:
    /**
     * @brief Constructor for websocket_listener.
     * @param ioc Boost.Asio IO context.
     * @param ctx SSL context.
     * @param endpoint TCP endpoint to listen on.
     * @param logger Logger instance for the listener.
     * @param session_logger Logger instance for WebSocket sessions.
     */
    websocket_listener(
        boost::asio::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint,
        std::shared_ptr<Logger> logger,
        std::shared_ptr<Logger> session_logger);

    /**
     * @brief Start accepting incoming connections.
     */
    void run();

private:
    /**
     * @brief Initiates an asynchronous accept operation.
     */
    void do_accept();

    /**
     * @brief Called when a new connection is accepted.
     * @param ec Error code from the accept operation.
     */
    void on_accept(boost::system::error_code ec);
};

#endif // WEBSOCKET_LISTENER_HPP

