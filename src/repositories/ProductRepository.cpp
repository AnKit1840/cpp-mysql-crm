#include "../../include/repositories/ProductRepository.hpp"
#include <sstream>

ProductRepository::ProductRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool ProductRepository::addProduct(Product& product) {
    std::string safeName = db.escapeString(product.getName());
    std::string safeDesc = db.escapeString(product.getDescription());

    std::stringstream ss;
    ss << "INSERT INTO products (category_id, name, price, stock_quantity, description) VALUES ("
       << product.getCategoryId() << ", '" << safeName << "', "
       << product.getPrice() << ", " << product.getStockQuantity() << ", '" << safeDesc << "');";

    db.executeNonQuery(ss.str());
    product.setId(static_cast<int>(db.getLastInsertId()));
    return product.getId() > 0;
}

bool ProductRepository::updateProduct(const Product& product) {
    std::string safeName = db.escapeString(product.getName());
    std::string safeDesc = db.escapeString(product.getDescription());

    std::stringstream ss;
    ss << "UPDATE products SET "
       << "category_id = " << product.getCategoryId() << ", "
       << "name = '" << safeName << "', "
       << "price = " << product.getPrice() << ", "
       << "stock_quantity = " << product.getStockQuantity() << ", "
       << "description = '" << safeDesc << "' "
       << "WHERE product_id = " << product.getId() << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::deleteProduct(int productId) {
    std::stringstream ss;
    ss << "DELETE FROM products WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

std::optional<Product> ProductRepository::getById(int productId) {
    std::stringstream ss;
    ss << "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
       << "FROM products p "
       << "LEFT JOIN categories c ON p.category_id = c.category_id "
       << "WHERE p.product_id = " << productId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Product prod(
        std::stoi(row[0]),
        std::stoi(row[1]),
        row[2] ? row[2] : "",
        std::stod(row[3]),
        std::stoi(row[4]),
        row[5] ? row[5] : "",
        row[6] ? row[6] : ""
    );

    mysql_free_result(res);
    return prod;
}

std::vector<Product> ProductRepository::search(const std::string& searchTerm) {
    std::vector<Product> list;
    std::string safeTerm = db.escapeString(searchTerm);

    std::stringstream ss;
    ss << "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
       << "FROM products p "
       << "LEFT JOIN categories c ON p.category_id = c.category_id "
       << "WHERE p.product_id = '" << safeTerm << "' OR p.name LIKE '%" << safeTerm << "%' OR c.name LIKE '%" << safeTerm << "%' "
       << "ORDER BY p.product_id ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            std::stod(row[3]),
            std::stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

std::vector<Product> ProductRepository::getAll() {
    std::vector<Product> list;
    std::string sql = "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
                      "FROM products p "
                      "LEFT JOIN categories c ON p.category_id = c.category_id "
                      "ORDER BY p.product_id ASC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            std::stod(row[3]),
            std::stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

std::vector<Product> ProductRepository::getLowStock(int threshold) {
    std::vector<Product> list;
    std::stringstream ss;
    ss << "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
       << "FROM products p "
       << "LEFT JOIN categories c ON p.category_id = c.category_id "
       << "WHERE p.stock_quantity <= " << threshold << " "
       << "ORDER BY p.stock_quantity ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? row[2] : "",
            std::stod(row[3]),
            std::stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

bool ProductRepository::updatePrice(int productId, double newPrice) {
    std::stringstream ss;
    ss << "UPDATE products SET price = " << newPrice << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::updateStock(int productId, int newStock) {
    std::stringstream ss;
    ss << "UPDATE products SET stock_quantity = " << newStock << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::deductStock(int productId, int quantity) {
    std::stringstream ss;
    ss << "UPDATE products SET stock_quantity = stock_quantity - " << quantity 
       << " WHERE product_id = " << productId << " AND stock_quantity >= " << quantity << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::restoreStock(int productId, int quantity) {
    std::stringstream ss;
    ss << "UPDATE products SET stock_quantity = stock_quantity + " << quantity 
       << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::isReferencedInOrders(int productId) {
    std::stringstream ss;
    ss << "SELECT COUNT(*) FROM order_items WHERE product_id = " << productId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return false;

    bool referenced = false;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        referenced = (std::stoi(row[0]) > 0);
    }
    mysql_free_result(res);
    return referenced;
}
