#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <vector>
#include <numeric>
#include <iomanip>
#include <iostream>
#include "OrderItem.hpp"

/**
 * @brief Order entity demonstrating Composition.
 * An Order contains and manages multiple OrderItem instances.
 */
class Order {
private:
    int id;
    int customerId;
    std::string customerName; // Joined convenience
    std::string orderDate;
    std::string status;       // "PENDING", "COMPLETED", "CANCELLED"
    double totalAmount;
    std::vector<OrderItem> items; // Composition

public:
    Order() : id(0), customerId(0), customerName(""), orderDate(""), status("PENDING"), totalAmount(0.0) {}
    Order(int id, int customerId, const std::string& orderDate, 
          const std::string& status = "PENDING", double totalAmount = 0.0,
          const std::string& customerName = "")
        : id(id), customerId(customerId), customerName(customerName),
          orderDate(orderDate), status(status), totalAmount(totalAmount) {}

    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    std::string getCustomerName() const { return customerName; }
    std::string getOrderDate() const { return orderDate; }
    std::string getStatus() const { return status; }
    double getTotalAmount() const { return totalAmount; }
    const std::vector<OrderItem>& getItems() const { return items; }
    std::vector<OrderItem>& getItems() { return items; }

    void setId(int newId) { id = newId; }
    void setCustomerId(int custId) { customerId = custId; }
    void setCustomerName(const std::string& name) { customerName = name; }
    void setOrderDate(const std::string& date) { orderDate = date; }
    void setStatus(const std::string& newStatus) { status = newStatus; }
    void setTotalAmount(double amount) { totalAmount = amount; }

    void addItem(const OrderItem& item) {
        items.push_back(item);
        calculateTotal();
    }

    void calculateTotal() {
        totalAmount = 0.0;
        for (const auto& item : items) {
            totalAmount += item.getSubtotal();
        }
    }

    bool isCancellable() const {
        // Only PENDING or newly placed COMPLETED orders can be cancelled with stock restoration
        return status == "PENDING" || status == "COMPLETED";
    }

    void displayOrderSummary() const {
        std::cout << "Order ID    : #" << id << "\n"
                  << "Customer ID : " << customerId << (customerName.empty() ? "" : " (" + customerName + ")") << "\n"
                  << "Date        : " << orderDate << "\n"
                  << "Status      : " << status << "\n"
                  << "Total Amount: $" << std::fixed << std::setprecision(2) << totalAmount << "\n";
        if (!items.empty()) {
            std::cout << "Line Items  :\n";
            for (const auto& item : items) {
                std::cout << "  - Product #" << item.getProductId() 
                          << (item.getProductName().empty() ? "" : " (" + item.getProductName() + ")")
                          << " | Qty: " << item.getQuantity()
                          << " @ $" << std::fixed << std::setprecision(2) << item.getUnitPrice()
                          << " = $" << item.getSubtotal() << "\n";
            }
        }
    }
};

#endif // ORDER_HPP
