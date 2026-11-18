#ifndef ORDER_REPOSITORY_HPP
#define ORDER_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/Order.hpp"
#include "../models/Payment.hpp"
#include "../database/DatabaseManager.hpp"

using namespace std;

class OrderRepository {
private:
    DatabaseManager& db;

public:
    explicit OrderRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    // Atomic Transactional Operations
    bool placeOrderTransaction(Order& order, Payment& payment);
    bool cancelOrderTransaction(int orderId);

    optional<Order> getById(int orderId);
    vector<Order> getOrdersByCustomer(int customerId);
    vector<Order> getAll();
    vector<Payment> getPaymentsForOrder(int orderId);
};

#endif // ORDER_REPOSITORY_HPP
