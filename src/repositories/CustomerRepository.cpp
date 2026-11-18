#include "../../include/repositories/CustomerRepository.hpp"
#include <sstream>

using namespace std;

CustomerRepository::CustomerRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool CustomerRepository::addCustomer(Customer& customer) {
    string safeName = db.escapeString(customer.getName());
    string safeEmail = db.escapeString(customer.getEmail());
    string safePhone = db.escapeString(customer.getPhone());
    string safeAddress = db.escapeString(customer.getAddress());

    stringstream ss;
    ss << "INSERT INTO customers (name, email, phone, address) VALUES ('"
       << safeName << "', '" << safeEmail << "', '" << safePhone << "', '" << safeAddress << "');";

    db.executeNonQuery(ss.str());
    customer.setId(static_cast<int>(db.getLastInsertId()));
    return customer.getId() > 0;
}

bool CustomerRepository::updateCustomer(const Customer& customer) {
    string safeName = db.escapeString(customer.getName());
    string safeEmail = db.escapeString(customer.getEmail());
    string safePhone = db.escapeString(customer.getPhone());
    string safeAddress = db.escapeString(customer.getAddress());

    stringstream ss;
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
    stringstream ss;
    ss << "DELETE FROM customers WHERE customer_id = " << customerId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

optional<Customer> CustomerRepository::getById(int customerId) {
    stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers WHERE customer_id = " 
       << customerId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return nullopt;
    }

    Customer customer(
        stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return customer;
}

optional<Customer> CustomerRepository::getByEmail(const string& email) {
    string safeEmail = db.escapeString(email);
    stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers WHERE email = '" 
       << safeEmail << "';";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return nullopt;
    }

    Customer customer(
        stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return customer;
}

vector<Customer> CustomerRepository::search(const string& searchTerm) {
    vector<Customer> list;
    string safeTerm = db.escapeString(searchTerm);

    stringstream ss;
    ss << "SELECT customer_id, name, email, phone, address, created_at FROM customers "
       << "WHERE customer_id = '" << safeTerm << "' OR name LIKE '%" << safeTerm << "%' OR email LIKE '%" << safeTerm << "%' "
       << "ORDER BY customer_id ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            stoi(row[0]),
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

vector<Customer> CustomerRepository::getAll() {
    vector<Customer> list;
    MYSQL_RES* res = db.executeQuery("SELECT customer_id, name, email, phone, address, created_at FROM customers ORDER BY customer_id ASC;");
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            stoi(row[0]),
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
    stringstream ss;
    ss << "SELECT COUNT(*) FROM orders WHERE customer_id = " << customerId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}

int CustomerRepository::getTicketCount(int customerId) {
    stringstream ss;
    ss << "SELECT COUNT(*) FROM support_tickets WHERE customer_id = " << customerId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}
