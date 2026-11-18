#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "User.hpp"
#include <iomanip>

using namespace std;

/**
 * @brief Employee entity inheriting from User.
 * Demonstrates:
 * - Inheritance (reuses common identity fields from User)
 * - Polymorphism (overrides displayDetails() and getRole())
 */
class Employee : public User {
private:
    string employeeRole; // e.g., "Senior Support Specialist", "Account Manager"
    string joiningDate;

public:
    Employee() : User(), employeeRole(""), joiningDate("") {}
    Employee(int id, const string& name, const string& email,
             const string& phone, const string& employeeRole,
             const string& joiningDate, const string& createdAt = "")
        : User(id, name, email, phone, createdAt), 
          employeeRole(employeeRole), joiningDate(joiningDate) {}

    string getEmployeeRole() const { return employeeRole; }
    void setEmployeeRole(const string& role) { employeeRole = role; }

    string getJoiningDate() const { return joiningDate; }
    void setJoiningDate(const string& date) { joiningDate = date; }

    void displayDetails() const override {
        cout << "Employee ID : " << id << "\n"
                  << "Name        : " << name << "\n"
                  << "Email       : " << email << "\n"
                  << "Phone       : " << phone << "\n"
                  << "Role        : " << employeeRole << "\n"
                  << "Joining Date: " << (joiningDate.empty() ? "N/A" : joiningDate) << "\n";
    }

    string getRole() const override {
        return employeeRole;
    }
};

#endif // EMPLOYEE_HPP
