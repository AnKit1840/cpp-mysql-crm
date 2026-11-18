#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <string>

class Category {
private:
    int id;
    std::string name;
    std::string description;

public:
    Category() : id(0), name(""), description("") {}
    Category(int id, const std::string& name, const std::string& description = "")
        : id(id), name(name), description(description) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
};

#endif // CATEGORY_HPP
