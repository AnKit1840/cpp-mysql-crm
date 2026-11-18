#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "User.hpp"
#include <iomanip>

/**
 * @brief Employee entity inheriting from User.
 * Demonstrates:
 * - Inheritance (reuses common identity fields from User)
 * - Polymorphism (overrides displayDetails() and getRole())
 */
class Employee : public User {
private:
    std::string employeeRole; // e.g., "Senior Support Specialist", "Account Manager"
    std::string joiningDate;

public:
    Employee() : User(), employeeRole(""), joiningDate("") {}
    Employee(int id, const std::string& name, const std::string& email,
             const std::string& phone, const std::string& employeeRole,
             const std::string& joiningDate, const std::string& createdAt = "")
        : User(id, name, email, phone, createdAt), 
          employeeRole(employeeRole), joiningDate(joiningDate) {}

    std::string getEmployeeRole() const { return employeeRole; }
    void setEmployeeRole(const std::string& role) { employeeRole = role; }

    std::string getJoiningDate() const { return joiningDate; }
    void setJoiningDate(const std::string& date) { joiningDate = date; }

    void displayDetails() const override {
        std::cout << "Employee ID : " << id << "\n"
                  << "Name        : " << name << "\n"
                  << "Email       : " << email << "\n"
                  << "Phone       : " << phone << "\n"
                  << "Role        : " << employeeRole << "\n"
                  << "Joining Date: " << (joiningDate.empty() ? "N/A" : joiningDate) << "\n";
    }

    std::string getRole() const override {
        return employeeRole;
    }
};

#endif // EMPLOYEE_HPP
