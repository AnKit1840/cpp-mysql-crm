#ifndef ORDER_ITEM_HPP
#define ORDER_ITEM_HPP

#include <string>

/**
 * @brief Represents a single line item within an Order.
 */
class OrderItem {
private:
    int id;
    int orderId;
    int productId;
    std::string productName; // Joined convenience
    int quantity;
    double unitPrice;
    double subtotal;

public:
    OrderItem() : id(0), orderId(0), productId(0), productName(""), quantity(0), unitPrice(0.0), subtotal(0.0) {}
    OrderItem(int id, int orderId, int productId, int quantity, double unitPrice,
              double subtotal = 0.0, const std::string& productName = "")
        : id(id), orderId(orderId), productId(productId), productName(productName),
          quantity(quantity), unitPrice(unitPrice), 
          subtotal(subtotal == 0.0 ? quantity * unitPrice : subtotal) {}

    int getId() const { return id; }
    int getOrderId() const { return orderId; }
    int getProductId() const { return productId; }
    std::string getProductName() const { return productName; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    double getSubtotal() const { return subtotal; }

    void setId(int newId) { id = newId; }
    void setOrderId(int ordId) { orderId = ordId; }
    void setProductId(int prodId) { productId = prodId; }
    void setProductName(const std::string& name) { productName = name; }
    void setQuantity(int qty) { 
        quantity = qty; 
        subtotal = quantity * unitPrice;
    }
    void setUnitPrice(double price) { 
        unitPrice = price; 
        subtotal = quantity * unitPrice;
    }
    void setSubtotal(double subtot) { subtotal = subtot; }
};

#endif // ORDER_ITEM_HPP
