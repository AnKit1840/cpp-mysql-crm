#include <iostream>
#include "../include/database/ConfigLoader.hpp"
#include "../include/database/DatabaseManager.hpp"
#include "../include/ui/MenuUI.hpp"

using namespace std;

int main() {
    cout << "========================================================\n"
              << "       ENTERPRISE CRM MANAGEMENT SYSTEM (C++17)         \n"
              << "========================================================\n"
              << "Initializing database connection...\n";

    DatabaseConfig config = ConfigLoader::load("config/db_config.ini");

    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        if (db.connect(config)) {
            cout << "[SUCCESS] Connected to MySQL database '" << config.database 
                      << "' on " << config.host << ":" << config.port << " as user '" 
                      << config.user << "'.\n";
        }
    } catch (const exception& e) {
        cerr << "\n[CRITICAL ERROR] Database connection failed:\n"
                  << "  " << e.what() << "\n\n"
                  << "Troubleshooting Tips:\n"
                  << "1. Verify that the MySQL Server service is running.\n"
                  << "2. Check your credentials in 'config/db_config.ini'.\n"
                  << "3. Ensure the database 'crm_db' has been created by running:\n"
                  << "   mysql -u root -p < sql/schema.sql\n"
                  << "   mysql -u root -p < sql/seed.sql\n\n";
        return 1;
    }

    try {
        MenuUI ui;
        ui.run();
    } catch (const exception& e) {
        cerr << "\n[UNHANDLED EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
