#include "../include/application.hpp"
#include "../../log/include/log.hpp" // Include the logger
#include "../../net/include/ollama.hpp"

/**
 * @brief Constructs an Application object.
 * 
 * Initializes the Ollama object with the specified server URL, and sets up the 
 * I/O context and timer. This constructor also initializes the logger for the application.
 * 
 * @param ioc A reference to the boost::asio::io_context, which is required for asynchronous operations.
 */
Application::Application(boost::asio::io_context& ioc)
    : io_context_(ioc), ollama_("http://localhost:11434"), timer_(io_context_)
{
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing app.");
    // Note: The start_timer function is commented out for now.
    // Uncomment the following line if you decide to use the recurring timer.
    // start_timer();
}

/**
 * @brief Performs an LLM query using the Ollama API with the given prompt.
 * 
 * This function constructs a request with the specified prompt and sends it to the LLM 
 * server using the Ollama API. The response from the server is returned as a string.
 * 
 * @param prompt The prompt to send to the LLM.
 * @return The response from the LLM as a string.
 */
std::string Application::query_llm(const std::string& prompt) {
    try {
        // Sending a query to the LLM using the Ollama instance
        auto response = ollama_.generate("llava:latest", prompt);
        return response.as_simple_string();
    } catch (const ollama::exception& e) {
        // Logging any errors that occur during the LLM query
        auto logger = LoggerManager::getLogger("ApplicationLogger", LogLevel::ERROR, LogOutput::CONSOLE);
        logger->log(LogLevel::ERROR, "Error querying LLM: " + std::string(e.what()));
        return "Error: " + std::string(e.what());
    }
}

/**
 * @brief Placeholder function for starting a recurring timer.
 * 
 * This function sets up a timer that triggers a specific task (e.g., querying the LLM)
 * at regular intervals. The timer is asynchronous, and the task is repeated indefinitely 
 * unless an error occurs.
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

/**
 * @brief Provides access to the Ollama instance.
 * 
 * This function returns a reference to the Ollama object, allowing other parts 
 * of the application to directly interact with the LLM through this instance.
 * 
 * @return A reference to the Ollama object.
 */
Ollama& Application::get_ollama() { 
    return ollama_; 
}

