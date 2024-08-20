#ifndef DB_HPP
#define DB_HPP

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>
#include <sstream>
#include <filesystem>

#include "../../log/include/log.hpp"

/**
 * @brief The Database class handles all interactions with the SQLite database.
 * 
 * This class is responsible for creating and managing the SQLite database connection,
 * including initializing the database file, checking for existing databases, and logging database operations.
 */
class Database {
public:
    /**
     * @brief Constructs a Database object and initializes the SQLite database.
     * 
     * The constructor creates or opens a SQLite database file in the "db/" directory with a name 
     * based on the current date (e.g., "db/database_08_19_2024.db"). If the database already exists, 
     * it is opened; otherwise, a new database file is created.
     */
    Database();

    /**
     * @brief Destructor for the Database class.
     * 
     * Ensures that the database connection is properly closed when the Database object is destroyed.
     */
    ~Database();

private:
    std::unique_ptr<SQLite::Database> db_;  ///< Unique pointer to the SQLite database connection.
};

#endif // DB_HPP

