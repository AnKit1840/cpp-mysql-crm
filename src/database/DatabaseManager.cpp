#include "../../include/database/DatabaseManager.hpp"

std::unique_ptr<DatabaseManager> DatabaseManager::instance = nullptr;
std::mutex DatabaseManager::instanceMutex;

DatabaseManager::DatabaseManager() : conn(nullptr), inTransaction(false) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

DatabaseManager& DatabaseManager::getInstance() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        instance.reset(new DatabaseManager());
    }
    return *instance;
}

bool DatabaseManager::connect(const DatabaseConfig& cfg) {
    disconnect();
    config = cfg;

    conn = mysql_init(nullptr);
    if (!conn) {
        throw DatabaseException("Failed to initialize MySQL handle (mysql_init).");
    }

    // Set connection timeout
    unsigned int timeout = 10;
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

    if (!mysql_real_connect(conn,
                            config.host.c_str(),
                            config.user.c_str(),
                            config.password.c_str(),
                            config.database.c_str(),
                            config.port,
                            nullptr,
                            CLIENT_MULTI_STATEMENTS)) {
        std::string err = mysql_error(conn);
        mysql_close(conn);
        conn = nullptr;
        throw DatabaseException("MySQL Connection Error: " + err);
    }

    // Set character set to utf8mb4
    mysql_set_character_set(conn, "utf8mb4");
    return true;
}

void DatabaseManager::disconnect() {
    if (conn) {
        if (inTransaction) {
            try { rollback(); } catch (...) {}
        }
        mysql_close(conn);
        conn = nullptr;
    }
}

bool DatabaseManager::isConnected() const {
    return conn != nullptr && mysql_ping(conn) == 0;
}

void DatabaseManager::executeNonQuery(const std::string& sql) {
    if (!conn) {
        throw DatabaseException("Database is not connected.");
    }
    if (mysql_query(conn, sql.c_str()) != 0) {
        throw DatabaseException("SQL Query Execution Error: " + std::string(mysql_error(conn)) + "\nQuery: " + sql);
    }
}

MYSQL_RES* DatabaseManager::executeQuery(const std::string& sql) {
    if (!conn) {
        throw DatabaseException("Database is not connected.");
    }
    if (mysql_query(conn, sql.c_str()) != 0) {
        throw DatabaseException("SQL Query Execution Error: " + std::string(mysql_error(conn)) + "\nQuery: " + sql);
    }
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result && mysql_field_count(conn) > 0) {
        throw DatabaseException("Failed to store MySQL result: " + std::string(mysql_error(conn)));
    }
    return result;
}

long long DatabaseManager::getLastInsertId() {
    if (!conn) return 0;
    return mysql_insert_id(conn);
}

int DatabaseManager::getAffectedRows() {
    if (!conn) return 0;
    return static_cast<int>(mysql_affected_rows(conn));
}

std::string DatabaseManager::escapeString(const std::string& input) {
    if (!conn) return input;
    std::vector<char> buffer(input.length() * 2 + 1);
    unsigned long length = mysql_real_escape_string(conn, buffer.data(), input.c_str(), input.length());
    return std::string(buffer.data(), length);
}

void DatabaseManager::beginTransaction() {
    executeNonQuery("START TRANSACTION;");
    inTransaction = true;
}

void DatabaseManager::commit() {
    executeNonQuery("COMMIT;");
    inTransaction = false;
}

void DatabaseManager::rollback() {
    executeNonQuery("ROLLBACK;");
    inTransaction = false;
}
