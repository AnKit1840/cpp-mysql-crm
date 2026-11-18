#ifndef CUSTOMER_REPOSITORY_HPP
#define CUSTOMER_REPOSITORY_HPP

#include <vector>
#include <optional>
#include <memory>
#include "../models/Customer.hpp"
#include "../database/DatabaseManager.hpp"

class CustomerRepository {
private:
    DatabaseManager& db;

public:
    explicit CustomerRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool addCustomer(Customer& customer);
    bool updateCustomer(const Customer& customer);
    bool deleteCustomer(int customerId);
    
    std::optional<Customer> getById(int customerId);
    std::optional<Customer> getByEmail(const std::string& email);
    std::vector<Customer> search(const std::string& searchTerm);
    std::vector<Customer> getAll();

    int getOrderCount(int customerId);
    int getTicketCount(int customerId);
};

#endif // CUSTOMER_REPOSITORY_HPP
