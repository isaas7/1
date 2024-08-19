#include "../include/db.hpp"

/**
 * @brief Constructs a Database object and initializes the SQLite database.
 * 
 * This constructor creates or opens a SQLite database file in the "db/" directory with a name 
 * based on the current date (e.g., "db/database_08_19_2024.db"). If the database already exists, 
 * it is opened; otherwise, a new database file is created. 
 */
Database::Database() {
    auto logger = LoggerManager::getLogger("database_logger", LogLevel::DEBUG, LogOutput::CONSOLE);

    // Get the current date and time for naming the database file
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%m_%d_%Y");
    
    // Create the database directory if it doesn't exist
    const std::string db_dir = "db";
    if (!std::filesystem::exists(db_dir)) {
        std::filesystem::create_directory(db_dir);
        logger->log(LogLevel::DEBUG, "Created database directory: " + db_dir);
    }

    // Create the database filename based on the current date
    std::string db_filename = db_dir + "/database_" + ss.str() + ".db";

    // Log whether the database is new or existing
    if (std::filesystem::exists(db_filename)) {
        logger->log(LogLevel::DEBUG, "Using existing database for today: " + db_filename);
    } else {
        logger->log(LogLevel::DEBUG, "Creating new database for today: " + db_filename);
    }

    // Try to open the database, logging and handling any errors
    try {
        db_ = std::make_unique<SQLite::Database>(db_filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERROR, "Cannot open database: " + std::string(e.what()));
        throw std::runtime_error("Failed to open database");
    }
}

/**
 * @brief Destructor for the Database class.
 * 
 * The destructor ensures that the database connection is properly closed when the Database object is destroyed.
 */
Database::~Database() {}

