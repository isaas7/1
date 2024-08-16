#include "../include/application.hpp"
#include "../../log/include/log.hpp"
#include "../../net/include/ollama.hpp"

/**
 * @brief Constructs an Application object and starts the query processing thread.
 * 
 * @param ioc The Boost.Asio I/O context that the application will use for asynchronous operations.
 */
Application::Application(boost::asio::io_context& ioc)
    : io_context_(ioc), ollama_("http://localhost:11434"), timer_(io_context_)
{
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing app.");

    // Start a thread to process the query queue
    std::thread(&Application::process_queries, this).detach();
}

/**
 * @brief Adds a new query to the application.
 * 
 * Generates a unique query ID, stores the prompt, and places the query in the queue for processing.
 * 
 * @param prompt The prompt to be sent to the LLM.
 * @return The unique ID of the newly added query.
 */
std::string Application::add_query(const std::string& prompt) {
    // Create a new Query object and generate a unique ID based on the prompt and current time.
    auto query = std::make_shared<Query>();
    query->id = std::to_string(std::hash<std::string>{}(prompt + std::to_string(std::chrono::system_clock::now().time_since_epoch().count())));
    query->prompt = prompt;

    {
        // Lock the mutex to protect access to the queue and map.
        std::lock_guard<std::mutex> lock(queue_mutex_);
        query_queue_.push(query);  // Add the query to the queue.
        query_map_[query->id] = query;  // Store the query in the map using its ID.
    }

    // Notify the processing thread that a new query is available.
    queue_cv_.notify_one();

    return query->id;  // Return the unique ID of the new query.
}

/**
 * @brief Retrieves the status of a specific query.
 * 
 * The status includes whether the query is running, completed, canceled, and any partial responses received so far.
 * 
 * @param query_id The unique ID of the query.
 * @return A JSON string containing the status of the query.
 */
std::string Application::get_query_status(const std::string& query_id) {
    std::lock_guard<std::mutex> lock(queue_mutex_);  // Lock the mutex to protect access to the query map.
    auto it = query_map_.find(query_id);
    if (it != query_map_.end()) {
        // If the query is found, construct a JSON object with its status.
        std::shared_ptr<Query> query = it->second;
        nlohmann::json response_json;
        response_json["query_id"] = query_id;
        response_json["completed"] = static_cast<bool>(query->completed);
        response_json["running"] = static_cast<bool>(query->running);
        response_json["canceled"] = static_cast<bool>(query->canceled);
        response_json["partial_responses"] = query->partial_responses;

        return response_json.dump();  // Return the status as a JSON string.
    } else {
        return R"({"error": "Query ID not found."})";  // Return an error if the query ID is not found.
    }
}

/**
 * @brief Cancels a specific query.
 * 
 * If the query is currently in progress, it will be marked as canceled and will stop processing.
 * 
 * @param query_id The unique ID of the query to cancel.
 */
void Application::cancel_query(const std::string& query_id) {
    std::lock_guard<std::mutex> lock(queue_mutex_);  // Lock the mutex to protect access to the query map.
    if (query_map_.find(query_id) != query_map_.end()) {
        query_map_[query_id]->canceled = true;  // Mark the query as canceled.
    }
}

/**
 * @brief Continuously processes queries from the queue.
 * 
 * This function runs in a separate thread, popping queries from the queue and processing them.
 * If a query is canceled, it will be skipped.
 */
void Application::process_queries() {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    while (true) {
        std::shared_ptr<Query> query;

        {
            // Lock the mutex and wait for new queries to be added to the queue.
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this]{ return !query_queue_.empty(); });

            // Pop the next query from the queue.
            query = query_queue_.front();
            query_queue_.pop();
        }

        // If the query has not been canceled, process it.
        if (query && !query->canceled) {
            query->running = true;
            run_query(query);  // Process the query.
        }
    }
}

/**
 * @brief Processes a single query by sending it to the LLM and handling partial responses.
 * 
 * This function sends the prompt to the LLM, handles partial responses, and marks the query as 
 * completed when all responses have been received or if an error occurs.
 * 
 * @param query The query to be processed.
 */
void Application::run_query(const std::shared_ptr<Query>& query) {
    query->running = true;

    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG);

    // Lambda function to handle each partial response received from the LLM.
    auto on_receive_token = [query, logger](const ollama::response& response) {
        logger->log(LogLevel::DEBUG, "Inside on_receive_token callback.");
        
        // Check if the response contains a partial response and handle it.
        if (response.as_json().contains("response")) {
            std::string partial_response = response.as_json()["response"];
            logger->log(LogLevel::DEBUG, "Valid partial response received: " + partial_response);
            query->partial_responses.push_back(partial_response);  // Add the partial response to the query.
        } else {
            logger->log(LogLevel::ERROR, "Invalid or error response: " + response.as_json_string());
        }

        // Mark the query as completed when the "done" flag is true.
        if (response.as_json().contains("done") && response.as_json()["done"].get<bool>()) {
            logger->log(LogLevel::DEBUG, "Final response received. Marking query as completed.");
            query->completed = true;
            query->running = false;
        }

        // If the query has been canceled, mark it as completed.
        if (query->canceled) {
            logger->log(LogLevel::DEBUG, "Query was canceled.");
            query->completed = true;
            query->running = false;
        }
    };

    // Send the prompt to the LLM and handle the response streaming.
    ollama_.generate("llava:latest", query->prompt, on_receive_token);

    // Mark the query as completed after processing (even if not successful).
    query->completed = true;
    query->running = false;
}

