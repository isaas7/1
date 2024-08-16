#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include "../../net/include/ollama.hpp"
#include "../../log/include/log.hpp"
class Application {
public:
    Application(boost::asio::io_context& ioc);

    /**
     * @brief Performs an HTTP POST request to the LLM server with the given prompt.
     * 
     * @param prompt The prompt to send to the LLM.
     * @return The response body as a string.
     */
    std::string query_llm(const std::string& prompt);
    Ollama& get_ollama();
private:
    /**
     * @brief Starts a recurring timer that executes a GET request every 60 seconds.
     */
    void start_timer();
    Ollama ollama_;
    boost::asio::io_context& io_context_;
    boost::asio::steady_timer timer_;
    std::shared_ptr<Logger> logger_; 
};

#endif // APPLICATION_HPP

