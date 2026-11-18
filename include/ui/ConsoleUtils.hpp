#ifndef CONSOLE_UTILS_HPP
#define CONSOLE_UTILS_HPP

#include <iostream>
#include <string>
#include <limits>

class ConsoleUtils {
public:
    static void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    static std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        // Trim leading and trailing whitespace
        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = input.find_last_not_of(" \t\r\n");
        return input.substr(start, end - start + 1);
    }

    static int readInt(const std::string& prompt, int defaultVal = -1) {
        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);
            if (input.empty() && defaultVal != -1) {
                return defaultVal;
            }
            try {
                return std::stoi(input);
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid integer.\n";
            }
        }
    }

    static double readDouble(const std::string& prompt, double defaultVal = -1.0) {
        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);
            if (input.empty() && defaultVal >= 0.0) {
                return defaultVal;
            }
            try {
                return std::stod(input);
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid numerical value.\n";
            }
        }
    }

    static void printHeader(const std::string& title) {
        std::cout << "\n========================================\n"
                  << "  " << title << "\n"
                  << "========================================\n";
    }
};

#endif // CONSOLE_UTILS_HPP
