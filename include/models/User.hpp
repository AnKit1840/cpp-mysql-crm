#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

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
    std::string name;
    std::string email;
    std::string phone;
    std::string createdAt;

public:
    User() : id(0), name(""), email(""), phone(""), createdAt("") {}
    User(int id, const std::string& name, const std::string& email, 
         const std::string& phone, const std::string& createdAt = "")
        : id(id), name(name), email(email), phone(phone), createdAt(createdAt) {}

    virtual ~User() = default;

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getCreatedAt() const { return createdAt; }

    // Setters
    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setCreatedAt(const std::string& newCreatedAt) { createdAt = newCreatedAt; }

    // Pure virtual functions for runtime polymorphism
    virtual void displayDetails() const = 0;
    virtual std::string getRole() const = 0;
};

#endif // USER_HPP
