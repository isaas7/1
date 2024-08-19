#include "../include/application.hpp"
#include "../../log/include/log.hpp"
#include <vector>
#include <numeric>
#include <sqlite3.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>  // C++17 feature for file system operations

/**
 * @brief Constructs an Application object and starts the query processing thread.
 * 
 * @param ioc The Boost.Asio I/O context that the application will use for asynchronous operations.
 */
Application::Application(boost::asio::io_context& ioc, ssl::context& ssl_ctx)
    : io_context_(ioc), ssl_ctx_(ssl_ctx), client_(std::make_shared<Client>(ioc, ssl_ctx)), ollama_("http://localhost:11434"), timer_(io_context_)
{
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger->log(LogLevel::DEBUG, "Initializing app.");

    initialize_database();  // Initialize the database connection
    check_and_create_tables();  // Check and create necessary tables

    // Start a thread to process the query queue
    std::thread(&Application::process_queries, this).detach();
}

/**
 * @brief Destructor to close the database connection.
 */
Application::~Application() {}

/**
 * @brief Initializes the SQLite database connection.
 * 
 * Opens the SQLite database for the current date. If the database file does not exist, it is created.
 */
void Application::initialize_database() {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    // Get current date and create a filename
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%m_%d_%Y");
    std::string db_filename = "database_" + ss.str() + ".db";

    // Log if using an existing or new database
    if (std::filesystem::exists(db_filename)) {
        logger->log(LogLevel::DEBUG, "Using existing database for today: " + db_filename);
    } else {
        logger->log(LogLevel::DEBUG, "Creating new database for today: " + db_filename);
    }

    try {
        db_ = std::make_unique<SQLite::Database>(db_filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Cannot open database: " + std::string(e.what()));
        throw std::runtime_error("Failed to open database");
    }
}


/**
 * @brief Checks if a table exists in the database and creates it if it doesn't.
 * 
 * Checks if the "example_table" exists in the SQLite database and creates it if it doesn't.
 */
void Application::check_and_create_tables() {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    const std::string check_table_sql = "CREATE TABLE IF NOT EXISTS example_table ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "data TEXT NOT NULL);";

    const std::string create_metrics_table_sql = "CREATE TABLE IF NOT EXISTS performance_metrics ("
                                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                 "timestamp TEXT NOT NULL,"
                                                 "metric_name TEXT NOT NULL,"
                                                 "metric_value REAL NOT NULL);";

    try {
        db_->exec(check_table_sql);
        logger->log(LogLevel::DEBUG, "Checked/created example_table successfully.");
        
        db_->exec(create_metrics_table_sql);
        logger->log(LogLevel::DEBUG, "Checked/created performance_metrics table successfully.");
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Failed to create/check tables: " + std::string(e.what()));
        throw std::runtime_error("Failed to create/check tables");
    }
}


void Application::log_performance_metric(const std::string& metric_name, double metric_value) {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    // Get the current timestamp
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    const std::string sql = "INSERT INTO performance_metrics (timestamp, metric_name, metric_value) VALUES (?, ?, ?);";

    try {
        SQLite::Statement stmt(*db_, sql);
        stmt.bind(1, ss.str());
        stmt.bind(2, metric_name);
        stmt.bind(3, metric_value);
        stmt.exec();
        logger->log(LogLevel::DEBUG, "Performance metric logged: " + metric_name + " = " + std::to_string(metric_value));
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Failed to log performance metric: " + std::string(e.what()));
    }
}



/**
 * @brief Adds a new query to the application.
 * 
 * Generates a unique query ID, stores the prompt, and places the query in the queue for processing.
 * 
 * @param prompt The prompt to be sent to the LLM.
 * @return The unique ID of the newly added query.
 */
std::string Application::add_query(const std::string& prompt, const ollama::response& context) {
    auto query = std::make_shared<Query>();
    query->id = std::to_string(std::hash<std::string>{}(prompt + std::to_string(std::chrono::system_clock::now().time_since_epoch().count())));
    query->prompt = prompt;
    
    if (context.is_valid()) {
        query->last_context = context;
    }

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        query_queue_.push(query);
        query_map_[query->id] = query;
    }

    queue_cv_.notify_one();

    return query->id;
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

        // Store the latest context for future queries
        query->last_context = response;

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

    // Send the prompt to the LLM with or without context
    if (query->last_context.is_valid()) {
        // Subsequent query with context
        ollama_.generate("llava:latest", query->prompt, query->last_context, on_receive_token);
    } else {
        // Initial query without context
        ollama_.generate("llava:latest", query->prompt, on_receive_token);
    }

    // Mark the query as completed after processing (even if not successful).
    query->completed = true;
    query->running = false;
}



void Application::fetch_and_update_json_data()
{
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG);

    try {
        // Use the Client class to perform the GET request
        std::string response_body = client_->get("localhost", "8080", "/json_data", 11);

        if (!response_body.empty()) {
            nlohmann::json json_data = nlohmann::json::parse(response_body);
            std::string prompt = "Index this data: " + json_data.dump();

            // Use the existing add_query method to submit the JSON data to the LLM for indexing
            std::string query_id = this->add_query(prompt);
            logger->log(LogLevel::DEBUG, "Submitted JSON data to LLM with query ID: " + query_id);
        } else {
            logger->log(LogLevel::ERROR, "Failed to fetch JSON data from server. Response was empty.");
        }
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Exception caught while fetching and updating JSON data: " + std::string(e.what()));
    }
}

std::vector<MetricStatistic> Application::get_performance_statistics() {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);
    std::vector<MetricStatistic> stats;

    const std::string sql = "SELECT metric_name, AVG(metric_value), MIN(metric_value), MAX(metric_value), SUM(metric_value), COUNT(*) "
                            "FROM performance_metrics "
                            "GROUP BY metric_name;";

    try {
        SQLite::Statement query(*db_, sql);
        while (query.executeStep()) {
            MetricStatistic stat;
            stat.metric_name = query.getColumn(0).getString();
            stat.average_value = query.getColumn(1).getDouble();
            stat.min_value = query.getColumn(2).getDouble();
            stat.max_value = query.getColumn(3).getDouble();
            stat.total_value = query.getColumn(4).getDouble();
            stat.count = query.getColumn(5).getInt();
            stats.push_back(stat);
        }
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Failed to retrieve performance statistics: " + std::string(e.what()));
    }

    return stats;
}


nlohmann::json Application::get_performance_statistics_json() {
    auto logger = LoggerManager::getLogger("application_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    nlohmann::json stats_json = nlohmann::json::array();
    std::vector<MetricStatistic> stats = get_performance_statistics();

    if (stats.empty()) {
        logger->log(LogLevel::INFO, "No performance metrics found.");
        return stats_json;
    }

    for (const auto& stat : stats) {
        nlohmann::json stat_json;
        stat_json["metric_name"] = stat.metric_name;
        stat_json["average_value"] = stat.average_value;
        stat_json["min_value"] = stat.min_value;
        stat_json["max_value"] = stat.max_value;
        stat_json["total_value"] = stat.total_value;
        stat_json["count"] = stat.count;
        stats_json.push_back(stat_json);
    }

    return stats_json;
}
