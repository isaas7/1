#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include "../../log/include/log.hpp" // Include the logger

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

/**
 * @brief A class representing an HTTP/HTTPS client.
 * 
 * This class is responsible for sending synchronous HTTP GET requests over SSL
 * and printing the response. It abstracts the functionality required to perform
 * the HTTP request.
 */
class Client {
public:
    /**
     * @brief Constructs a Client object.
     * 
     * @param io_context The I/O context used for network operations.
     * @param ssl_ctx The SSL context used for managing SSL connections.
     */
    Client(boost::asio::io_context& io_context, ssl::context& ssl_ctx);

    /**
     * @brief Performs an HTTP GET request.
     * 
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @param target The target resource to request.
     * @param version The HTTP version to use (1.0 or 1.1).
     * @return The response body as a string.
     */
    std::string get(const std::string& host, const std::string& port, const std::string& target, int version = 11);

    /**
     * @brief Performs an HTTP POST request.
     * 
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @param target The target resource to request.
     * @param body The body content to send in the POST request.
     * @param version The HTTP version to use (1.0 or 1.1).
     * @return The response body as a string.
     */
    std::string post(const std::string& host, const std::string& port, const std::string& target, const std::string& body, int version = 11);

private:
    tcp::resolver resolver_;
    ssl::context& ssl_ctx_; // Keep the SSL context for creating new streams
    boost::asio::io_context& io_context_; // Store io_context reference
    std::shared_ptr<Logger> logger_; // Logger instance for the Client class

    /**
     * @brief Helper method to handle the common logic for sending HTTP requests.
     * 
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @param req The HTTP request object.
     * @return The HTTP response as a string.
     */
    std::string send_request(const std::string& host, const std::string& port, http::request<http::string_body>& req);
};

#endif // CLIENT_HPP

