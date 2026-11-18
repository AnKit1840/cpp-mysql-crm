#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <string>

using namespace std;

class Category {
private:
    int id;
    string name;
    string description;

public:
    Category() : id(0), name(""), description("") {}
    Category(int id, const string& name, const string& description = "")
        : id(id), name(name), description(description) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getDescription() const { return description; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setDescription(const string& newDesc) { description = newDesc; }
};

#endif // CATEGORY_HPP
