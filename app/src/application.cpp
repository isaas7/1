#include "../include/application.hpp"
#include "../../log/include/log.hpp" // Include the logger
#include "../../net/include/ollama.hpp"
/**
 * @brief Constructs an Application object and starts the recurring GET request timer.
 * 
 * Initializes the Ollama object and starts the timer for recurring LLM queries.
 */
Application::Application(boost::asio::io_context& ioc)
    : io_context_(ioc), ollama_("http://localhost:11434"), timer_(io_context_)
{
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing app.");
    // Start the recurring timer when the application is constructed
    //start_timer();
}

/**
 * @brief Performs an LLM query using the Ollama class with the given prompt.
 * 
 * @param prompt The prompt to send to the LLM.
 * @return The response body as a string.
 */
std::string Application::query_llm(const std::string& prompt) {
    try {
        // Sending a query to the LLM using the Ollama instance
        auto response = ollama_.generate("llava:latest", prompt);
        return response.as_simple_string();
    } catch (const ollama::exception& e) {
        auto logger = LoggerManager::getLogger("ApplicationLogger", LogLevel::ERROR, LogOutput::CONSOLE);
        logger->log(LogLevel::ERROR, "Error querying LLM: " + std::string(e.what()));
        return "Error: " + std::string(e.what());
    }
}

/**
 * @brief Starts a recurring timer that executes an LLM query every 60 seconds.
 */
void Application::start_timer() {
    auto logger = LoggerManager::getLogger("ApplicationLogger", LogLevel::INFO, LogOutput::CONSOLE);

    timer_.expires_after(std::chrono::seconds(10));
    timer_.async_wait([this, logger](const boost::system::error_code& ec) {
        if (!ec) {
            logger->log(LogLevel::INFO, "Executing LLM query after 10 seconds delay.");
            std::string response = query_llm("Why is the sky blue?");
            logger->log(LogLevel::INFO, "Received response from LLM: " + response);

            // Restart the timer for the next iteration
            start_timer();
        } else {
            logger->log(LogLevel::ERROR, "Timer error: " + ec.message());
        }
    });
}

Ollama& Application::get_ollama() { return ollama_; }
