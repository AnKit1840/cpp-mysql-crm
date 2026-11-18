#ifndef CUSTOMER_SERVICE_HPP
#define CUSTOMER_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Customer.hpp"
#include "../repositories/CustomerRepository.hpp"

class CustomerService {
private:
    CustomerRepository repo;

    bool isValidEmail(const std::string& email) const;

public:
    explicit CustomerService(CustomerRepository repository = CustomerRepository());

    Customer registerCustomer(const std::string& name, const std::string& email,
                             const std::string& phone, const std::string& address);
    bool updateCustomer(int id, const std::string& name, const std::string& email,
                        const std::string& phone, const std::string& address);
    bool deleteCustomer(int id);

    std::optional<Customer> getCustomerById(int id);
    std::optional<Customer> getCustomerByEmail(const std::string& email);
    std::vector<Customer> searchCustomers(const std::string& query);
    std::vector<Customer> getAllCustomers();

    bool canDeleteCustomer(int id, std::string& reason);
};

#endif // CUSTOMER_SERVICE_HPP
