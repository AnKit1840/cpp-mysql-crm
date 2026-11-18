#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string>
#include <iomanip>
#include <iostream>

class Payment {
private:
    int id;
    int orderId;
    std::string paymentDate;
    double amount;
    std::string paymentMethod; // "CASH", "CREDIT_CARD", "DEBIT_CARD", "UPI", "BANK_TRANSFER"
    std::string status;        // "PAID", "REFUNDED", "FAILED"

public:
    Payment() : id(0), orderId(0), paymentDate(""), amount(0.0), paymentMethod("CASH"), status("PAID") {}
    Payment(int id, int orderId, const std::string& paymentDate, double amount,
            const std::string& paymentMethod = "CASH", const std::string& status = "PAID")
        : id(id), orderId(orderId), paymentDate(paymentDate), amount(amount),
          paymentMethod(paymentMethod), status(status) {}

    int getId() const { return id; }
    int getOrderId() const { return orderId; }
    std::string getPaymentDate() const { return paymentDate; }
    double getAmount() const { return amount; }
    std::string getPaymentMethod() const { return paymentMethod; }
    std::string getStatus() const { return status; }

    void setId(int newId) { id = newId; }
    void setOrderId(int ordId) { orderId = ordId; }
    void setPaymentDate(const std::string& date) { paymentDate = date; }
    void setAmount(double amt) { amount = amt; }
    void setPaymentMethod(const std::string& method) { paymentMethod = method; }
    void setStatus(const std::string& newStatus) { status = newStatus; }

    void display() const {
        std::cout << "Payment #" << id << " | Order #" << orderId 
                  << " | Date: " << paymentDate
                  << " | Method: " << paymentMethod
                  << " | Amount: $" << std::fixed << std::setprecision(2) << amount
                  << " | Status: " << status << "\n";
    }
};

#endif // PAYMENT_HPP
