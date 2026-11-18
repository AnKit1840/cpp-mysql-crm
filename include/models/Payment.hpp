#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class Payment {
private:
    int id;
    int orderId;
    string paymentDate;
    double amount;
    string paymentMethod; // "CASH", "CREDIT_CARD", "DEBIT_CARD", "UPI", "BANK_TRANSFER"
    string status;        // "PAID", "REFUNDED", "FAILED"

public:
    Payment() : id(0), orderId(0), paymentDate(""), amount(0.0), paymentMethod("CASH"), status("PAID") {}
    Payment(int id, int orderId, const string& paymentDate, double amount,
            const string& paymentMethod = "CASH", const string& status = "PAID")
        : id(id), orderId(orderId), paymentDate(paymentDate), amount(amount),
          paymentMethod(paymentMethod), status(status) {}

    int getId() const { return id; }
    int getOrderId() const { return orderId; }
    string getPaymentDate() const { return paymentDate; }
    double getAmount() const { return amount; }
    string getPaymentMethod() const { return paymentMethod; }
    string getStatus() const { return status; }

    void setId(int newId) { id = newId; }
    void setOrderId(int ordId) { orderId = ordId; }
    void setPaymentDate(const string& date) { paymentDate = date; }
    void setAmount(double amt) { amount = amt; }
    void setPaymentMethod(const string& method) { paymentMethod = method; }
    void setStatus(const string& newStatus) { status = newStatus; }

    void display() const {
        cout << "Payment #" << id << " | Order #" << orderId 
                  << " | Date: " << paymentDate
                  << " | Method: " << paymentMethod
                  << " | Amount: $" << fixed << setprecision(2) << amount
                  << " | Status: " << status << "\n";
    }
};

#endif // PAYMENT_HPP
