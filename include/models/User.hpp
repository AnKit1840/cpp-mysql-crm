#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Abstract Base Class representing any identity in the CRM system.
 * Demonstrates:
 * - Encapsulation (protected/private fields, controlled getters/setters)
 * - Abstraction & Runtime Polymorphism (pure virtual displayDetails() and getRole())
 * - RAII & Memory Safety (virtual destructor ensures proper cleanup of derived objects)
 */
class User {
protected:
    int id;
    string name;
    string email;
    string phone;
    string createdAt;

public:
    User() : id(0), name(""), email(""), phone(""), createdAt("") {}
    User(int id, const string& name, const string& email, 
         const string& phone, const string& createdAt = "")
        : id(id), name(name), email(email), phone(phone), createdAt(createdAt) {}

    virtual ~User() = default;

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getCreatedAt() const { return createdAt; }

    // Setters
    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setPhone(const string& newPhone) { phone = newPhone; }
    void setCreatedAt(const string& newCreatedAt) { createdAt = newCreatedAt; }

    // Pure virtual functions for runtime polymorphism
    virtual void displayDetails() const = 0;
    virtual string getRole() const = 0;
};

#endif // USER_HPP
