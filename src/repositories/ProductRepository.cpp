#include "../../include/repositories/ProductRepository.hpp"
#include <sstream>

using namespace std;

ProductRepository::ProductRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool ProductRepository::addProduct(Product& product) {
    string safeName = db.escapeString(product.getName());
    string safeDesc = db.escapeString(product.getDescription());

    stringstream ss;
    ss << "INSERT INTO products (category_id, name, price, stock_quantity, description) VALUES ("
       << product.getCategoryId() << ", '" << safeName << "', "
       << product.getPrice() << ", " << product.getStockQuantity() << ", '" << safeDesc << "');";

    db.executeNonQuery(ss.str());
    product.setId(static_cast<int>(db.getLastInsertId()));
    return product.getId() > 0;
}

bool ProductRepository::updateProduct(const Product& product) {
    string safeName = db.escapeString(product.getName());
    string safeDesc = db.escapeString(product.getDescription());

    stringstream ss;
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
    stringstream ss;
    ss << "DELETE FROM products WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

optional<Product> ProductRepository::getById(int productId) {
    stringstream ss;
    ss << "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
       << "FROM products p "
       << "LEFT JOIN categories c ON p.category_id = c.category_id "
       << "WHERE p.product_id = " << productId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return nullopt;
    }

    Product prod(
        stoi(row[0]),
        stoi(row[1]),
        row[2] ? row[2] : "",
        stod(row[3]),
        stoi(row[4]),
        row[5] ? row[5] : "",
        row[6] ? row[6] : ""
    );

    mysql_free_result(res);
    return prod;
}

vector<Product> ProductRepository::search(const string& searchTerm) {
    vector<Product> list;
    string safeTerm = db.escapeString(searchTerm);

    stringstream ss;
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? row[2] : "",
            stod(row[3]),
            stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

vector<Product> ProductRepository::getAll() {
    vector<Product> list;
    string sql = "SELECT p.product_id, p.category_id, p.name, p.price, p.stock_quantity, p.description, c.name "
                      "FROM products p "
                      "LEFT JOIN categories c ON p.category_id = c.category_id "
                      "ORDER BY p.product_id ASC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? row[2] : "",
            stod(row[3]),
            stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

vector<Product> ProductRepository::getLowStock(int threshold) {
    vector<Product> list;
    stringstream ss;
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? row[2] : "",
            stod(row[3]),
            stoi(row[4]),
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

bool ProductRepository::updatePrice(int productId, double newPrice) {
    stringstream ss;
    ss << "UPDATE products SET price = " << newPrice << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::updateStock(int productId, int newStock) {
    stringstream ss;
    ss << "UPDATE products SET stock_quantity = " << newStock << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::deductStock(int productId, int quantity) {
    stringstream ss;
    ss << "UPDATE products SET stock_quantity = stock_quantity - " << quantity 
       << " WHERE product_id = " << productId << " AND stock_quantity >= " << quantity << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::restoreStock(int productId, int quantity) {
    stringstream ss;
    ss << "UPDATE products SET stock_quantity = stock_quantity + " << quantity 
       << " WHERE product_id = " << productId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool ProductRepository::isReferencedInOrders(int productId) {
    stringstream ss;
    ss << "SELECT COUNT(*) FROM order_items WHERE product_id = " << productId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return false;

    bool referenced = false;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        referenced = (stoi(row[0]) > 0);
    }
    mysql_free_result(res);
    return referenced;
}
