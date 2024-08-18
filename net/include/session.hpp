#ifndef SESSION_HPP
#define SESSION_HPP

#include "../../app/include/application.hpp"
#include "http_tools.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

/**
 * @brief The session class manages an individual HTTP session.
 * 
 * This class handles the SSL handshake, reading HTTP requests, sending HTTP responses,
 * and closing the session.
 */
class session : public std::enable_shared_from_this<session>
{
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;  // SSL stream for the session
    boost::beast::flat_buffer buffer_;  // Buffer for reading requests
    std::shared_ptr<std::string const> doc_root_;  // Document root directory
    boost::beast::http::request<boost::beast::http::string_body> req_;  // HTTP request object
    std::shared_ptr<Application> app_;
public:
    /**
     * @brief Constructs a session object.
     * 
     * Initializes the session with the given socket, SSL context, and document root.
     * 
     * @param socket The socket for the session.
     * @param ctx The SSL context for managing SSL connections.
     * @param doc_root The document root directory for serving files.
     */
    session(
        boost::asio::ip::tcp::socket&& socket,
        boost::asio::ssl::context& ctx,
        std::shared_ptr<std::string const> const& doc_root,
        std::shared_ptr<Application> app);

    /**
     * @brief Starts the session by initiating the SSL handshake.
     */
    void run();

private:
    /**
     * @brief Handles the asynchronous run operation.
     * 
     * This method is called after the session is dispatched and starts the SSL handshake.
     */
    void on_run();

    /**
     * @brief Handles the SSL handshake completion.
     * 
     * This method is called when the SSL handshake is complete.
     * 
     * @param ec The error code, if any, from the handshake operation.
     */
    void on_handshake(boost::beast::error_code ec);

    /**
     * @brief Reads an HTTP request from the client.
     * 
     * Initiates an asynchronous read operation to receive the client's HTTP request.
     */
    void do_read();

    /**
     * @brief Handles the completion of the asynchronous read operation.
     * 
     * Processes the received HTTP request or closes the session if an error occurs.
     * 
     * @param ec The error code, if any, from the read operation.
     * @param bytes_transferred The number of bytes transferred during the read.
     */
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred, std::chrono::steady_clock::time_point read_start_time);

    /**
     * @brief Sends an HTTP response to the client.
     * 
     * @param msg The HTTP response to send.
     */
    void send_response(boost::beast::http::message_generator&& msg);

    /**
     * @brief Handles the completion of the asynchronous write operation.
     * 
     * Determines whether to keep the connection alive or close it.
     * 
     * @param keep_alive Whether to keep the connection alive.
     * @param ec The error code, if any, from the write operation.
     * @param bytes_transferred The number of bytes transferred during the write.
     */
    void on_write(bool keep_alive, boost::beast::error_code ec, std::size_t bytes_transferred);

    /**
     * @brief Closes the session.
     * 
     * Initiates the SSL shutdown process and closes the connection.
     */
    void do_close();

    /**
     * @brief Handles the completion of the asynchronous shutdown operation.
     * 
     * Finalizes the session closure after the SSL shutdown is complete.
     * 
     * @param ec The error code, if any, from the shutdown operation.
     */
    void on_shutdown(boost::beast::error_code ec);
};

#endif // SESSION_HPP

