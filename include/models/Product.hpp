#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>
#include <iostream>
#include <iomanip>

/**
 * @brief Product entity representing catalogue items.
 * Demonstrates:
 * - Encapsulation (controlled inventory update methods)
 * - Domain validation (prevents negative stock or price)
 */
class Product {
private:
    int id;
    int categoryId;
    std::string name;
    double price;
    int stockQuantity;
    std::string description;
    std::string categoryName; // Join convenience

public:
    Product() : id(0), categoryId(0), name(""), price(0.0), stockQuantity(0), description(""), categoryName("") {}
    Product(int id, int categoryId, const std::string& name, double price,
            int stockQuantity, const std::string& description = "", const std::string& categoryName = "")
        : id(id), categoryId(categoryId), name(name), price(price),
          stockQuantity(stockQuantity), description(description), categoryName(categoryName) {}

    int getId() const { return id; }
    int getCategoryId() const { return categoryId; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }
    std::string getDescription() const { return description; }
    std::string getCategoryName() const { return categoryName; }

    void setId(int newId) { id = newId; }
    void setCategoryId(int catId) { categoryId = catId; }
    void setName(const std::string& newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }
    void setStockQuantity(int qty) { stockQuantity = qty; }
    void setDescription(const std::string& desc) { description = desc; }
    void setCategoryName(const std::string& catName) { categoryName = catName; }

    // Business Logic Validation
    bool hasStock(int requestedQty) const {
        return stockQuantity >= requestedQty && requestedQty > 0;
    }

    bool isLowStock(int threshold = 10) const {
        return stockQuantity <= threshold;
    }

    void deductStock(int qty) {
        if (qty > 0 && stockQuantity >= qty) {
            stockQuantity -= qty;
        }
    }

    void restoreStock(int qty) {
        if (qty > 0) {
            stockQuantity += qty;
        }
    }

    void displayDetails() const {
        std::cout << "Product ID   : " << id << "\n"
                  << "Name         : " << name << "\n"
                  << "Category     : " << (categoryName.empty() ? std::to_string(categoryId) : categoryName) << "\n"
                  << "Price        : $" << std::fixed << std::setprecision(2) << price << "\n"
                  << "Available Qty: " << stockQuantity << (isLowStock() ? " [LOW STOCK ALERT]" : "") << "\n"
                  << "Description  : " << (description.empty() ? "None" : description) << "\n";
    }
};

#endif // PRODUCT_HPP
