#ifndef CUSTOMER_REPOSITORY_HPP
#define CUSTOMER_REPOSITORY_HPP

#include <vector>
#include <optional>
#include <memory>
#include "../models/Customer.hpp"
#include "../database/DatabaseManager.hpp"

using namespace std;

class CustomerRepository {
private:
    DatabaseManager& db;

public:
    explicit CustomerRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool addCustomer(Customer& customer);
    bool updateCustomer(const Customer& customer);
    bool deleteCustomer(int customerId);
    
    optional<Customer> getById(int customerId);
    optional<Customer> getByEmail(const string& email);
    vector<Customer> search(const string& searchTerm);
    vector<Customer> getAll();

    int getOrderCount(int customerId);
    int getTicketCount(int customerId);
};

#endif // CUSTOMER_REPOSITORY_HPP
