#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include "../../net/include/ollama.hpp"
#include "../../log/include/log.hpp"

/**
 * @brief The Application class encapsulates the main logic of the application.
 * 
 * This class is responsible for managing the interaction with the LLM (Large Language Model) 
 * through the Ollama API. It handles making HTTP POST requests to the LLM server with specific 
 * prompts and retrieving the responses.
 */
class Application {
public:
    /**
     * @brief Constructs an Application object.
     * 
     * Initializes the Ollama object, which is used to interact with the LLM, 
     * and sets up the required I/O context and timer for potential future tasks.
     * 
     * @param ioc A reference to the boost::asio::io_context, which is required for asynchronous operations.
     */
    Application(boost::asio::io_context& ioc);

    /**
     * @brief Performs an LLM query using the Ollama API with the given prompt.
     * 
     * This function sends a request to the LLM server using the Ollama API and retrieves the response.
     * 
     * @param prompt The prompt to send to the LLM.
     * @return The response from the LLM as a string.
     */
    std::string query_llm(const std::string& prompt);

    /**
     * @brief Provides access to the Ollama instance.
     * 
     * This function returns a reference to the Ollama object, allowing other parts 
     * of the application to directly interact with the LLM through this instance.
     * 
     * @return A reference to the Ollama object.
     */
    Ollama& get_ollama();

private:
    /**
     * @brief (Placeholder) Starts a recurring timer for executing tasks periodically.
     * 
     * This function is intended to set up a timer that triggers certain tasks (e.g., 
     * querying the LLM) at regular intervals. It uses Boost.Asio's asynchronous timer 
     * mechanism to achieve this.
     */
    void start_timer();

    Ollama ollama_;  ///< The Ollama object responsible for interacting with the LLM.
    boost::asio::io_context& io_context_;  ///< Reference to the I/O context for asynchronous operations.
    boost::asio::steady_timer timer_;  ///< Timer for scheduling periodic tasks.
    std::shared_ptr<Logger> logger_;  ///< Logger instance for logging application events.
};

#endif // APPLICATION_HPP

