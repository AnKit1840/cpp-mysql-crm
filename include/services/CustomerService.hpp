#ifndef CUSTOMER_SERVICE_HPP
#define CUSTOMER_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Customer.hpp"
#include "../repositories/CustomerRepository.hpp"

using namespace std;

class CustomerService {
private:
    CustomerRepository repo;

    bool isValidEmail(const string& email) const;

public:
    explicit CustomerService(CustomerRepository repository = CustomerRepository());

    Customer registerCustomer(const string& name, const string& email,
                             const string& phone, const string& address);
    bool updateCustomer(int id, const string& name, const string& email,
                        const string& phone, const string& address);
    bool deleteCustomer(int id);

    optional<Customer> getCustomerById(int id);
    optional<Customer> getCustomerByEmail(const string& email);
    vector<Customer> searchCustomers(const string& query);
    vector<Customer> getAllCustomers();

    bool canDeleteCustomer(int id, string& reason);
};

#endif // CUSTOMER_SERVICE_HPP
