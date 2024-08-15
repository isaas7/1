#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/asio.hpp>
#include <string>
#include "../../net/include/ollama.hpp" // Include the Ollama class

class Application {
public:
    Application(boost::asio::io_context& ioc);

    /**
     * @brief Performs an LLM query using the Ollama class with the given prompt.
     * 
     * @param prompt The prompt to send to the LLM.
     * @return The response body as a string.
     */
    std::string query_llm(const std::string& prompt);

private:
    /**
     * @brief Starts a recurring timer that executes a GET request every 60 seconds.
     */
    void start_timer();

    boost::asio::io_context& io_context_;
    Ollama ollama_;  // Ollama instance for making LLM queries
    boost::asio::steady_timer timer_;
};

#endif // APPLICATION_HPP

