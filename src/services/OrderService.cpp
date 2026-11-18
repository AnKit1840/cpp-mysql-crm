#include "../../include/services/OrderService.hpp"
#include <stdexcept>

OrderService::OrderService(OrderRepository oRepo, ProductRepository pRepo, CustomerRepository cRepo)
    : orderRepo(oRepo), productRepo(pRepo), customerRepo(cRepo) {}

Order OrderService::placeOrder(int customerId,
                               const std::vector<std::pair<int, int>>& requestedItems,
                               const std::string& paymentMethod) {
    // 1. Validate customer existence
    auto customer = customerRepo.getById(customerId);
    if (!customer.has_value()) {
        throw std::invalid_argument("Customer ID #" + std::to_string(customerId) + " not found.");
    }

    if (requestedItems.empty()) {
        throw std::invalid_argument("Order must contain at least one product item.");
    }

    // 2. Pre-validate stock and product details
    Order newOrder(0, customerId, "", "COMPLETED", 0.0, customer->getName());

    for (const auto& itemPair : requestedItems) {
        int productId = itemPair.first;
        int quantity = itemPair.second;

        if (quantity <= 0) {
            throw std::invalid_argument("Requested quantity must be greater than zero for product #" + std::to_string(productId));
        }

        auto product = productRepo.getById(productId);
        if (!product.has_value()) {
            throw std::invalid_argument("Product ID #" + std::to_string(productId) + " not found.");
        }

        if (!product->hasStock(quantity)) {
            throw std::runtime_error("Insufficient stock for Product '" + product->getName() + 
                                     "'. Requested: " + std::to_string(quantity) + 
                                     ", Available: " + std::to_string(product->getStockQuantity()));
        }

        OrderItem item(0, 0, productId, quantity, product->getPrice(), quantity * product->getPrice(), product->getName());
        newOrder.addItem(item);
    }

    newOrder.calculateTotal();

    // 3. Prepare payment
    Payment payment(0, 0, "", newOrder.getTotalAmount(), paymentMethod, "PAID");

    // 4. Execute atomic transaction (insert order, insert items, deduct stock, record payment)
    if (!orderRepo.placeOrderTransaction(newOrder, payment)) {
        throw std::runtime_error("Database transaction failed during order placement.");
    }

    return newOrder;
}

bool OrderService::cancelOrder(int orderId) {
    auto order = orderRepo.getById(orderId);
    if (!order.has_value()) {
        throw std::invalid_argument("Order ID #" + std::to_string(orderId) + " not found.");
    }

    if (order->getStatus() == "CANCELLED") {
        throw std::runtime_error("Order #" + std::to_string(orderId) + " is already cancelled.");
    }

    return orderRepo.cancelOrderTransaction(orderId);
}

std::optional<Order> OrderService::getOrderDetails(int orderId) {
    return orderRepo.getById(orderId);
}

std::vector<Order> OrderService::getCustomerOrders(int customerId) {
    return orderRepo.getOrdersByCustomer(customerId);
}

std::vector<Order> OrderService::getAllOrders() {
    return orderRepo.getAll();
}

std::vector<Payment> OrderService::getOrderPayments(int orderId) {
    return orderRepo.getPaymentsForOrder(orderId);
}
