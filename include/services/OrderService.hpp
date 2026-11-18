#ifndef ORDER_SERVICE_HPP
#define ORDER_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <utility>
#include "../models/Order.hpp"
#include "../models/Payment.hpp"
#include "../repositories/OrderRepository.hpp"
#include "../repositories/ProductRepository.hpp"
#include "../repositories/CustomerRepository.hpp"

class OrderService {
private:
    OrderRepository orderRepo;
    ProductRepository productRepo;
    CustomerRepository customerRepo;

public:
    explicit OrderService(OrderRepository oRepo = OrderRepository(),
                          ProductRepository pRepo = ProductRepository(),
                          CustomerRepository cRepo = CustomerRepository());

    Order placeOrder(int customerId,
                     const std::vector<std::pair<int, int>>& requestedItems,
                     const std::string& paymentMethod = "CREDIT_CARD");

    bool cancelOrder(int orderId);

    std::optional<Order> getOrderDetails(int orderId);
    std::vector<Order> getCustomerOrders(int customerId);
    std::vector<Order> getAllOrders();
    std::vector<Payment> getOrderPayments(int orderId);
};

#endif // ORDER_SERVICE_HPP
