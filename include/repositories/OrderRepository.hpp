#ifndef ORDER_REPOSITORY_HPP
#define ORDER_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/Order.hpp"
#include "../models/Payment.hpp"
#include "../database/DatabaseManager.hpp"

class OrderRepository {
private:
    DatabaseManager& db;

public:
    explicit OrderRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    // Atomic Transactional Operations
    bool placeOrderTransaction(Order& order, Payment& payment);
    bool cancelOrderTransaction(int orderId);

    std::optional<Order> getById(int orderId);
    std::vector<Order> getOrdersByCustomer(int customerId);
    std::vector<Order> getAll();
    std::vector<Payment> getPaymentsForOrder(int orderId);
};

#endif // ORDER_REPOSITORY_HPP
