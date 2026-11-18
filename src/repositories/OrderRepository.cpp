#include "../../include/repositories/OrderRepository.hpp"
#include <sstream>

OrderRepository::OrderRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool OrderRepository::placeOrderTransaction(Order& order, Payment& payment) {
    if (order.getItems().empty()) {
        throw DatabaseException("Cannot place order with zero line items.");
    }

    // RAII Transaction Guard: rolls back automatically if any exception is thrown
    TransactionGuard guard(db);

    // 1. Insert order header
    std::stringstream ssOrder;
    ssOrder << "INSERT INTO orders (customer_id, status, total_amount) VALUES ("
            << order.getCustomerId() << ", '" << order.getStatus() << "', "
            << order.getTotalAmount() << ");";

    db.executeNonQuery(ssOrder.str());
    int orderId = static_cast<int>(db.getLastInsertId());
    order.setId(orderId);

    // 2. Insert line items and deduct inventory atomically
    for (auto& item : order.getItems()) {
        item.setOrderId(orderId);

        // Deduct inventory with strict concurrency validation (stock >= quantity)
        std::stringstream ssStock;
        ssStock << "UPDATE products SET stock_quantity = stock_quantity - " << item.getQuantity()
                << " WHERE product_id = " << item.getProductId()
                << " AND stock_quantity >= " << item.getQuantity() << ";";

        db.executeNonQuery(ssStock.str());
        if (db.getAffectedRows() == 0) {
            throw DatabaseException("Insufficient inventory available for Product ID #" + std::to_string(item.getProductId()));
        }

        // Insert item record
        std::stringstream ssItem;
        ssItem << "INSERT INTO order_items (order_id, product_id, quantity, unit_price, subtotal) VALUES ("
               << orderId << ", " << item.getProductId() << ", "
               << item.getQuantity() << ", " << item.getUnitPrice() << ", "
               << item.getSubtotal() << ");";

        db.executeNonQuery(ssItem.str());
        item.setId(static_cast<int>(db.getLastInsertId()));
    }

    // 3. Record payment
    payment.setOrderId(orderId);
    std::string safeMethod = db.escapeString(payment.getPaymentMethod());
    std::string safeStatus = db.escapeString(payment.getStatus());

    std::stringstream ssPayment;
    ssPayment << "INSERT INTO payments (order_id, amount, payment_method, status) VALUES ("
              << orderId << ", " << payment.getAmount() << ", '"
              << safeMethod << "', '" << safeStatus << "');";

    db.executeNonQuery(ssPayment.str());
    payment.setId(static_cast<int>(db.getLastInsertId()));

    // 4. Commit all transaction changes together
    guard.commit();
    return true;
}

bool OrderRepository::cancelOrderTransaction(int orderId) {
    TransactionGuard guard(db);

    // 1. Verify order exists and is eligible for cancellation
    std::stringstream ssCheck;
    ssCheck << "SELECT status FROM orders WHERE order_id = " << orderId << ";";
    MYSQL_RES* res = db.executeQuery(ssCheck.str());
    if (!res) return false;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        throw DatabaseException("Order ID #" + std::to_string(orderId) + " does not exist.");
    }

    std::string currentStatus = row[0] ? row[0] : "";
    mysql_free_result(res);

    if (currentStatus == "CANCELLED") {
        throw DatabaseException("Order ID #" + std::to_string(orderId) + " is already cancelled.");
    }

    // 2. Fetch line items to restore inventory
    std::stringstream ssItems;
    ssItems << "SELECT product_id, quantity FROM order_items WHERE order_id = " << orderId << ";";
    MYSQL_RES* resItems = db.executeQuery(ssItems.str());
    if (resItems) {
        MYSQL_ROW itemRow;
        while ((itemRow = mysql_fetch_row(resItems)) != nullptr) {
            int prodId = std::stoi(itemRow[0]);
            int qty = std::stoi(itemRow[1]);

            std::stringstream ssRestore;
            ssRestore << "UPDATE products SET stock_quantity = stock_quantity + " << qty
                      << " WHERE product_id = " << prodId << ";";
            db.executeNonQuery(ssRestore.str());
        }
        mysql_free_result(resItems);
    }

    // 3. Update order status to CANCELLED
    std::stringstream ssUpdateOrder;
    ssUpdateOrder << "UPDATE orders SET status = 'CANCELLED' WHERE order_id = " << orderId << ";";
    db.executeNonQuery(ssUpdateOrder.str());

    // 4. Update payment records to REFUNDED
    std::stringstream ssUpdatePayment;
    ssUpdatePayment << "UPDATE payments SET status = 'REFUNDED' WHERE order_id = " << orderId << ";";
    db.executeNonQuery(ssUpdatePayment.str());

    guard.commit();
    return true;
}

std::optional<Order> OrderRepository::getById(int orderId) {
    std::stringstream ss;
    ss << "SELECT o.order_id, o.customer_id, o.order_date, o.status, o.total_amount, c.name "
       << "FROM orders o "
       << "LEFT JOIN customers c ON o.customer_id = c.customer_id "
       << "WHERE o.order_id = " << orderId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Order order(
        std::stoi(row[0]),
        std::stoi(row[1]),
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        std::stod(row[4]),
        row[5] ? row[5] : ""
    );
    mysql_free_result(res);

    // Fetch line items
    std::stringstream ssItems;
    ssItems << "SELECT oi.order_item_id, oi.order_id, oi.product_id, oi.quantity, oi.unit_price, oi.subtotal, p.name "
            << "FROM order_items oi "
            << "LEFT JOIN products p ON oi.product_id = p.product_id "
            << "WHERE oi.order_id = " << orderId << " "
            << "ORDER BY oi.order_item_id ASC;";

    MYSQL_RES* resItems = db.executeQuery(ssItems.str());
    if (resItems) {
        MYSQL_ROW itemRow;
        while ((itemRow = mysql_fetch_row(resItems)) != nullptr) {
            order.addItem(OrderItem(
                std::stoi(itemRow[0]),
                std::stoi(itemRow[1]),
                std::stoi(itemRow[2]),
                std::stoi(itemRow[3]),
                std::stod(itemRow[4]),
                std::stod(itemRow[5]),
                itemRow[6] ? itemRow[6] : ""
            ));
        }
        mysql_free_result(resItems);
    }

    return order;
}

std::vector<Order> OrderRepository::getOrdersByCustomer(int customerId) {
    std::vector<Order> list;
    std::stringstream ss;
    ss << "SELECT o.order_id, o.customer_id, o.order_date, o.status, o.total_amount, c.name "
       << "FROM orders o "
       << "LEFT JOIN customers c ON o.customer_id = c.customer_id "
       << "WHERE o.customer_id = " << customerId << " "
       << "ORDER BY o.order_date DESC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            std::stod(row[4]),
            row[5] ? row[5] : ""
        );
    }
    mysql_free_result(res);

    return list;
}

std::vector<Order> OrderRepository::getAll() {
    std::vector<Order> list;
    std::string sql = "SELECT o.order_id, o.customer_id, o.order_date, o.status, o.total_amount, c.name "
                      "FROM orders o "
                      "LEFT JOIN customers c ON o.customer_id = c.customer_id "
                      "ORDER BY o.order_id DESC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            std::stod(row[4]),
            row[5] ? row[5] : ""
        );
    }
    mysql_free_result(res);

    return list;
}

std::vector<Payment> OrderRepository::getPaymentsForOrder(int orderId) {
    std::vector<Payment> list;
    std::stringstream ss;
    ss << "SELECT payment_id, order_id, payment_date, amount, payment_method, status "
       << "FROM payments WHERE order_id = " << orderId << " ORDER BY payment_date DESC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            std::stod(row[3]),
            row[4] ? row[4] : "CASH",
            row[5] ? row[5] : "PAID"
        );
    }
    mysql_free_result(res);

    return list;
}
