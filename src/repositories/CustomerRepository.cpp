#include "../../include/repositories/CustomerRepository.hpp"
#include <sstream>

CustomerRepository::CustomerRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool CustomerRepository::addCustomer(Customer& customer) {
    std::string safeName = db.escapeString(customer.getName());
    std::string safeEmail = db.escapeString(customer.getEmail());
    std::string safePhone = db.escapeString(customer.getPhone());
    std::string safeAddress = db.escapeString(customer.getAddress());

    std::stringstream ss;
    ss << "INSERT INTO customers (name, email, phone, address) VALUES ('"
       << safeName << "', '" << safeEmail << "', '" << safePhone << "', '" << safeAddress << "');";

    db.executeNonQuery(ss.str());
    customer.setId(static_cast<int>(db.getLastInsertId()));
    return customer.getId() > 0;
}

bool CustomerRepository::updateCustomer(const Customer& customer) {
    std::string safeName = db.escapeString(customer.getName());
    std::string safeEmail = db.escapeString(customer.getEmail());
    std::string safePhone = db.escapeString(customer.getPhone());
    std::string safeAddress = db.escapeString(customer.getAddress());

    std::stringstream ss;
    ss << "UPDATE customers SET "
       << "name = '" << safeName << "', "
       << "email = '" << safeEmail << "', "
       << "phone = '" << safePhone << "', "
       << "address = '" << safeAddress << "' "
       << "WHERE customer_id = " << customer.getId() << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool CustomerRepository::deleteCustomer(int customerId) {
    std::stringstream ss;
    ss << "DELETE FROM customers WHERE customer_id = " << customerId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

std::optional<Customer> CustomerRepository::getById(int customerId) {
    std::stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers WHERE customer_id = " 
       << customerId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Customer customer(
        std::stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return customer;
}

std::optional<Customer> CustomerRepository::getByEmail(const std::string& email) {
    std::string safeEmail = db.escapeString(email);
    std::stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers WHERE email = '" 
       << safeEmail << "';";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Customer customer(
        std::stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return customer;
}

std::vector<Customer> CustomerRepository::search(const std::string& searchTerm) {
    std::vector<Customer> list;
    std::string safeTerm = db.escapeString(searchTerm);

    std::stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers "
       << "WHERE customer_id = '" << safeTerm << "' OR name LIKE '%" << safeTerm << "%' OR email LIKE '%" << safeTerm << "%' "
       << "ORDER BY customer_id ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            row[1] ? row[1] : "",
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

std::vector<Customer> CustomerRepository::getAll() {
    std::vector<Customer> list;
    MYSQL_RES* res = db.executeQuery("SELECT customer_id, name, email, phone, address, created_at FROM customers ORDER BY customer_id ASC;");
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            row[1] ? row[1] : "",
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

int CustomerRepository::getOrderCount(int customerId) {
    std::stringstream ss;
    ss << "SELECT COUNT(*) FROM orders WHERE customer_id = " << customerId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = std::stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}

int CustomerRepository::getTicketCount(int customerId) {
    std::stringstream ss;
    ss << "SELECT COUNT(*) FROM support_tickets WHERE customer_id = " << customerId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = std::stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}
