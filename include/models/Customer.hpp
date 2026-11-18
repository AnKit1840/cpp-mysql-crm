#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include <iomanip>

/**
 * @brief Customer entity inheriting from User.
 * Demonstrates:
 * - Inheritance (reuses common identity fields from User)
 * - Polymorphism (overrides displayDetails() and getRole())
 */
class Customer : public User {
private:
    std::string address;

public:
    Customer() : User(), address("") {}
    Customer(int id, const std::string& name, const std::string& email,
             const std::string& phone, const std::string& address,
             const std::string& createdAt = "")
        : User(id, name, email, phone, createdAt), address(address) {}

    std::string getAddress() const { return address; }
    void setAddress(const std::string& newAddress) { address = newAddress; }

    void displayDetails() const override {
        std::cout << "Customer ID : " << id << "\n"
                  << "Name        : " << name << "\n"
                  << "Email       : " << email << "\n"
                  << "Phone       : " << phone << "\n"
                  << "Address     : " << address << "\n"
                  << "Registered  : " << (createdAt.empty() ? "N/A" : createdAt) << "\n";
    }

    std::string getRole() const override {
        return "Customer";
    }
};

#endif // CUSTOMER_HPP
