#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <vector>
#include <numeric>
#include <iomanip>
#include <iostream>
#include "OrderItem.hpp"

using namespace std;

/**
 * @brief Order entity demonstrating Composition.
 * An Order contains and manages multiple OrderItem instances.
 */
class Order {
private:
    int id;
    int customerId;
    string customerName; // Joined convenience
    string orderDate;
    string status;       // "PENDING", "COMPLETED", "CANCELLED"
    double totalAmount;
    vector<OrderItem> items; // Composition

public:
    Order() : id(0), customerId(0), customerName(""), orderDate(""), status("PENDING"), totalAmount(0.0) {}
    Order(int id, int customerId, const string& orderDate, 
          const string& status = "PENDING", double totalAmount = 0.0,
          const string& customerName = "")
        : id(id), customerId(customerId), customerName(customerName),
          orderDate(orderDate), status(status), totalAmount(totalAmount) {}

    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    string getCustomerName() const { return customerName; }
    string getOrderDate() const { return orderDate; }
    string getStatus() const { return status; }
    double getTotalAmount() const { return totalAmount; }
    const vector<OrderItem>& getItems() const { return items; }
    vector<OrderItem>& getItems() { return items; }

    void setId(int newId) { id = newId; }
    void setCustomerId(int custId) { customerId = custId; }
    void setCustomerName(const string& name) { customerName = name; }
    void setOrderDate(const string& date) { orderDate = date; }
    void setStatus(const string& newStatus) { status = newStatus; }
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
        cout << "Order ID    : #" << id << "\n"
                  << "Customer ID : " << customerId << (customerName.empty() ? "" : " (" + customerName + ")") << "\n"
                  << "Date        : " << orderDate << "\n"
                  << "Status      : " << status << "\n"
                  << "Total Amount: $" << fixed << setprecision(2) << totalAmount << "\n";
        if (!items.empty()) {
            cout << "Line Items  :\n";
            for (const auto& item : items) {
                cout << "  - Product #" << item.getProductId() 
                          << (item.getProductName().empty() ? "" : " (" + item.getProductName() + ")")
                          << " | Qty: " << item.getQuantity()
                          << " @ $" << fixed << setprecision(2) << item.getUnitPrice()
                          << " = $" << item.getSubtotal() << "\n";
            }
        }
    }
};

#endif // ORDER_HPP
