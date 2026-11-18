#ifndef CONSOLE_UTILS_HPP
#define CONSOLE_UTILS_HPP

#include <iostream>
#include <string>
#include <limits>

using namespace std;

class ConsoleUtils {
public:
    static void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static string readLine(const string& prompt) {
        cout << prompt;
        string input;
        getline(cin, input);
        // Trim leading and trailing whitespace
        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        size_t end = input.find_last_not_of(" \t\r\n");
        return input.substr(start, end - start + 1);
    }

    static int readInt(const string& prompt, int defaultVal = -1) {
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            if (input.empty() && defaultVal != -1) {
                return defaultVal;
            }
            try {
                return stoi(input);
            } catch (...) {
                cout << "Invalid input. Please enter a valid integer.\n";
            }
        }
    }

    static double readDouble(const string& prompt, double defaultVal = -1.0) {
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            if (input.empty() && defaultVal >= 0.0) {
                return defaultVal;
            }
            try {
                return stod(input);
            } catch (...) {
                cout << "Invalid input. Please enter a valid numerical value.\n";
            }
        }
    }

    static void printHeader(const string& title) {
        cout << "\n========================================\n"
                  << "  " << title << "\n"
                  << "========================================\n";
    }
};

#endif // CONSOLE_UTILS_HPP
