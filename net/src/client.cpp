#include "../include/client.hpp"
#include "../include/root_certificate.hpp"
#include <iostream>

/**
 * @brief Constructs a Client object.
 * 
 * Initializes the resolver with the I/O context and stores references to the I/O context and SSL context.
 * 
 * @param io_context The I/O context used for network operations.
 * @param ssl_ctx The SSL context used for managing SSL connections.
 */
Client::Client(boost::asio::io_context& io_context, ssl::context& ssl_ctx)
    : resolver_(io_context), ssl_ctx_(ssl_ctx), io_context_(io_context), logger_(LoggerManager::getLogger("ClientLogger", LogLevel::DEBUG))
{
    // Load the root certificates into the SSL context
    load_root_certificates(ssl_ctx_);

    // Set SSL context to verify the server's certificate
    ssl_ctx_.set_verify_mode(ssl::verify_none);

    logger_->log(LogLevel::INFO, "Client initialized.");
}

/**
 * @brief Performs an HTTP GET request.
 * 
 * Constructs and sends an HTTP GET request to the specified host and target.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param target The target resource to request.
 * @param version The HTTP version to use (1.0 or 1.1).
 * @return The response body as a string.
 */
std::string Client::get(const std::string& host, const std::string& port, const std::string& target, int version) {
    logger_->log(LogLevel::DEBUG, "Performing GET request to " + host + ":" + port + target);
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    return send_request(host, port, req);
}

/**
 * @brief Performs an HTTP POST request.
 * 
 * Constructs and sends an HTTP POST request to the specified host and target with the provided body.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param target The target resource to request.
 * @param body The body content to send in the POST request.
 * @param version The HTTP version to use (1.0 or 1.1).
 * @return The response body as a string.
 */
std::string Client::post(const std::string& host, const std::string& port, const std::string& target, const std::string& body, int version) {
    logger_->log(LogLevel::DEBUG, "Performing POST request to " + host + ":" + port + target);
    http::request<http::string_body> req{http::verb::post, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.body() = body;
    req.prepare_payload();
    return send_request(host, port, req);
}

/**
 * @brief Helper method to handle the common logic for sending HTTP requests.
 * 
 * Resolves the host, establishes an SSL connection, sends the request, receives the response,
 * and returns the response body as a string.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param req The HTTP request object.
 * @return The HTTP response as a string.
 */
std::string Client::send_request(const std::string& host, const std::string& port, http::request<http::string_body>& req) {
    try {
        ssl::stream<beast::tcp_stream> stream(io_context_, ssl_ctx_);

        // Set SNI hostname for SSL handshake
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        logger_->log(LogLevel::DEBUG, "Resolving " + host + ":" + port);
        // Resolve the host and port
        auto const results = resolver_.resolve(host, port);

        logger_->log(LogLevel::DEBUG, "Connecting to resolved address.");
        // Connect to the resolved IP address
        beast::get_lowest_layer(stream).connect(results);

        logger_->log(LogLevel::DEBUG, "Performing SSL handshake.");
        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        logger_->log(LogLevel::DEBUG, "Sending HTTP request.");
        // Send the HTTP request
        http::write(stream, req);

        // Buffer for reading the response
        beast::flat_buffer buffer;

        // Container for the response
        http::response<http::dynamic_body> res;

        logger_->log(LogLevel::DEBUG, "Receiving HTTP response.");
        // Receive the HTTP response
        http::read(stream, buffer, res);

        logger_->log(LogLevel::INFO, "Received response: " + beast::buffers_to_string(res.body().data()));
        // Return the response body as a string
        return beast::buffers_to_string(res.body().data());
    }
    catch (const std::exception& e) {
        logger_->log(LogLevel::ERROR, "Error occurred: " + std::string(e.what()));
        return "";
    }
}

