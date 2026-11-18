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

using namespace std;

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
                     const vector<pair<int, int>>& requestedItems,
                     const string& paymentMethod = "CREDIT_CARD");

    bool cancelOrder(int orderId);

    optional<Order> getOrderDetails(int orderId);
    vector<Order> getCustomerOrders(int customerId);
    vector<Order> getAllOrders();
    vector<Payment> getOrderPayments(int orderId);
};

#endif // ORDER_SERVICE_HPP
