#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>

// NOTE: 'using namespace std' is intentionally omitted in this header.
// This header is included by DatabaseManager.hpp which also includes <mysql.h>.
// MySQL on Windows pulls in <windows.h> -> <winscard.h> -> <rpcndr.h> which
// typedefs 'byte' as 'unsigned char'. C++17 also defines 'std::byte'.
// A 'using namespace std' here would make 'byte' ambiguous and break compilation.
// Use explicit std:: prefixes in this file only.

struct DatabaseConfig {
    std::string host     = "127.0.0.1";
    unsigned int port    = 3306;
    std::string database = "crm_db";
    std::string user     = "root";
    std::string password = "";
};

class ConfigLoader {
public:
    static DatabaseConfig load(const std::string& configFilePath = "config/db_config.ini") {
        DatabaseConfig config;

        // 1. Try reading from config file if it exists
        std::ifstream file(configFilePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                // Trim leading whitespace
                line.erase(0, line.find_first_not_of(" \t\r\n"));
                if (line.empty() || line[0] == '#' || line[0] == ';' || line[0] == '[') {
                    continue;
                }

                auto delimiterPos = line.find('=');
                if (delimiterPos != std::string::npos) {
                    std::string key   = line.substr(0, delimiterPos);
                    std::string value = line.substr(delimiterPos + 1);

                    // Trim key and value
                    key.erase(key.find_last_not_of(" \t\r\n") + 1);
                    value.erase(0, value.find_first_not_of(" \t\r\n"));
                    value.erase(value.find_last_not_of(" \t\r\n") + 1);

                    if (key == "host")          config.host     = value;
                    else if (key == "port")     config.port     = static_cast<unsigned int>(std::stoul(value));
                    else if (key == "database") config.database = value;
                    else if (key == "user")     config.user     = value;
                    else if (key == "password") config.password = value;
                }
            }
            file.close();
        }

        // 2. Override with environment variables if set (highest precedence)
        if (const char* envHost = getenv("DB_HOST"))     config.host     = envHost;
        if (const char* envPort = getenv("DB_PORT"))     config.port     = static_cast<unsigned int>(std::stoul(envPort));
        if (const char* envDb   = getenv("DB_NAME"))     config.database = envDb;
        if (const char* envUser = getenv("DB_USER"))     config.user     = envUser;
        if (const char* envPass = getenv("DB_PASSWORD")) config.password = envPass;

        return config;
    }
};

#endif // CONFIG_LOADER_HPP
