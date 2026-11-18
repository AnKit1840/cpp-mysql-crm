#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include <iomanip>

using namespace std;

/**
 * @brief Customer entity inheriting from User.
 * Demonstrates:
 * - Inheritance (reuses common identity fields from User)
 * - Polymorphism (overrides displayDetails() and getRole())
 */
class Customer : public User {
private:
    string address;

public:
    Customer() : User(), address("") {}
    Customer(int id, const string& name, const string& email,
             const string& phone, const string& address,
             const string& createdAt = "")
        : User(id, name, email, phone, createdAt), address(address) {}

    string getAddress() const { return address; }
    void setAddress(const string& newAddress) { address = newAddress; }

    void displayDetails() const override {
        cout << "Customer ID : " << id << "\n"
                  << "Name        : " << name << "\n"
                  << "Email       : " << email << "\n"
                  << "Phone       : " << phone << "\n"
                  << "Address     : " << address << "\n"
                  << "Registered  : " << (createdAt.empty() ? "N/A" : createdAt) << "\n";
    }

    string getRole() const override {
        return "Customer";
    }
};

#endif // CUSTOMER_HPP
