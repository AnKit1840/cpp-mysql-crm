#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <mutex>
#include <iostream>
#include <mysql.h>
#include "ConfigLoader.hpp"

// NOTE: 'using namespace std' is intentionally omitted in this header.
// Including <mysql.h> on Windows transitively includes <windows.h> -> <rpcndr.h>
// which typedefs 'byte' as 'unsigned char'. C++17 std::byte conflicts with this.
// Using 'using namespace std' here would cause an "ambiguous reference to byte"
// compilation error. All .cpp files that include this header may still use
// 'using namespace std' because they include this header first (before <iostream>
// re-opens the std namespace issue in this header's scope).

/**
 * @brief Thread-safe Singleton Database Manager wrapping MySQL C API with modern C++ RAII.
 * Demonstrates:
 * - Singleton Design Pattern (centralized connection access)
 * - RAII (Resource Acquisition Is Initialization for MYSQL handles and transactions)
 * - Exception Safety (SQLException thrown on database failures)
 * - ACID Transactions (explicit beginTransaction, commit, rollback)
 */
class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

class DatabaseManager {
private:
    static std::unique_ptr<DatabaseManager> instance;
    static std::mutex instanceMutex;

    MYSQL* conn;
    bool inTransaction;
    DatabaseConfig config;

    DatabaseManager();

public:
    ~DatabaseManager();

    // Delete copy and move semantics for Singleton
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& getInstance();

    bool connect(const DatabaseConfig& cfg);
    void disconnect();
    bool isConnected() const;

    MYSQL* getConnection() { return conn; }

    // Query execution
    void executeNonQuery(const std::string& sql);
    MYSQL_RES* executeQuery(const std::string& sql);
    long long getLastInsertId();
    int getAffectedRows();

    // SQL Injection Prevention: Escapes user inputs
    std::string escapeString(const std::string& input);

    // ACID Transaction Control
    void beginTransaction();
    void commit();
    void rollback();
    bool isInTransaction() const { return inTransaction; }
};

/**
 * @brief RAII Transaction Guard.
 * Automatically issues a ROLLBACK in the destructor if commit() was not called,
 * guaranteeing zero partial database states upon exceptions or early returns.
 */
class TransactionGuard {
private:
    DatabaseManager& db;
    bool committed;

public:
    explicit TransactionGuard(DatabaseManager& dbManager = DatabaseManager::getInstance())
        : db(dbManager), committed(false) {
        db.beginTransaction();
    }

    ~TransactionGuard() {
        if (!committed && db.isInTransaction()) {
            try {
                db.rollback();
                std::cerr << "[RAII] Transaction rolled back automatically due to uncommitted state.\n";
            } catch (...) {
                // Destructors should never throw
            }
        }
    }

    void commit() {
        db.commit();
        committed = true;
    }

    // Non-copyable
    TransactionGuard(const TransactionGuard&) = delete;
    TransactionGuard& operator=(const TransactionGuard&) = delete;
};

#endif // DATABASE_MANAGER_HPP
