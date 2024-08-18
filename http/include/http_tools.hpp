#ifndef HTTP_TOOLS_HPP
#define HTTP_TOOLS_HPP

#include "../../app/include/application.hpp"
#include "beast.hpp"
#include <boost/config.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <string>

/**
 * @brief Determine the MIME type based on the file extension.
 * 
 * @param path The file path.
 * @return The MIME type as a string_view.
 */
beast::string_view mime_type(beast::string_view path);

/**
 * @brief Concatenate a base path with a relative path.
 * 
 * @param base The base path.
 * @param path The relative path.
 * @return The concatenated path as a string.
 */
std::string path_cat(beast::string_view base, beast::string_view path);

/**
 * @brief Handle an incoming HTTP request and generate an appropriate response.
 * 
 * This function handles different HTTP methods and serves files from the document root.
 * 
 * @param doc_root The document root directory.
 * @param req The HTTP request object.
 * @return A message generator for the HTTP response.
 */
template <class Body, class Allocator>
boost::beast::http::message_generator handle_request(
    beast::string_view doc_root,
    boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
    std::shared_ptr<Application> app);

#endif // HTTP_TOOLS_HPP

