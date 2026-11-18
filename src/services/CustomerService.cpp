#include "../../include/services/CustomerService.hpp"
#include <regex>

using namespace std;

CustomerService::CustomerService(CustomerRepository repository) : repo(repository) {}

bool CustomerService::isValidEmail(const string& email) const {
    const regex emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(email, emailRegex);
}

Customer CustomerService::registerCustomer(const string& name, const string& email,
                                           const string& phone, const string& address) {
    if (name.empty()) {
        throw invalid_argument("Customer name cannot be empty.");
    }
    if (!isValidEmail(email)) {
        throw invalid_argument("Invalid email format: " + email);
    }
    if (phone.empty()) {
        throw invalid_argument("Customer phone number cannot be empty.");
    }

    // Check duplicate email
    if (repo.getByEmail(email).has_value()) {
        throw invalid_argument("A customer with email '" + email + "' already exists.");
    }

    Customer customer(0, name, email, phone, address);
    if (!repo.addCustomer(customer)) {
        throw runtime_error("Failed to insert customer record into database.");
    }
    return customer;
}

bool CustomerService::updateCustomer(int id, const string& name, const string& email,
                                     const string& phone, const string& address) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw invalid_argument("Customer ID #" + to_string(id) + " not found.");
    }

    if (!isValidEmail(email)) {
        throw invalid_argument("Invalid email format: " + email);
    }

    // Check if new email is already taken by another customer
    auto emailOwner = repo.getByEmail(email);
    if (emailOwner.has_value() && emailOwner->getId() != id) {
        throw invalid_argument("Email '" + email + "' is already in use by another customer.");
    }

    Customer updated(id, name, email, phone, address, existing->getCreatedAt());
    return repo.updateCustomer(updated);
}

bool CustomerService::canDeleteCustomer(int id, string& reason) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        reason = "Customer does not exist.";
        return false;
    }

    int orders = repo.getOrderCount(id);
    if (orders > 0) {
        reason = "Cannot delete: Customer has " + to_string(orders) + " associated order(s).";
        return false;
    }

    int tickets = repo.getTicketCount(id);
    if (tickets > 0) {
        reason = "Cannot delete: Customer has " + to_string(tickets) + " associated support ticket(s).";
        return false;
    }

    return true;
}

bool CustomerService::deleteCustomer(int id) {
    string reason;
    if (!canDeleteCustomer(id, reason)) {
        throw runtime_error(reason);
    }
    return repo.deleteCustomer(id);
}

optional<Customer> CustomerService::getCustomerById(int id) {
    return repo.getById(id);
}

optional<Customer> CustomerService::getCustomerByEmail(const string& email) {
    return repo.getByEmail(email);
}

vector<Customer> CustomerService::searchCustomers(const string& query) {
    return repo.search(query);
}

vector<Customer> CustomerService::getAllCustomers() {
    return repo.getAll();
}
