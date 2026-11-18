#include "../../include/services/EmployeeService.hpp"
#include <regex>

EmployeeService::EmployeeService(EmployeeRepository repository) : repo(repository) {}

bool EmployeeService::isValidEmail(const std::string& email) const {
    const std::regex emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailRegex);
}

Employee EmployeeService::registerEmployee(const std::string& name, const std::string& email,
                                          const std::string& phone, const std::string& role,
                                          const std::string& joiningDate) {
    if (name.empty()) {
        throw std::invalid_argument("Employee name cannot be empty.");
    }
    if (!isValidEmail(email)) {
        throw std::invalid_argument("Invalid email format: " + email);
    }
    if (phone.empty()) {
        throw std::invalid_argument("Employee phone number cannot be empty.");
    }
    if (role.empty()) {
        throw std::invalid_argument("Employee role cannot be empty.");
    }

    if (repo.getByEmail(email).has_value()) {
        throw std::invalid_argument("An employee with email '" + email + "' already exists.");
    }

    Employee emp(0, name, email, phone, role, joiningDate);
    if (!repo.addEmployee(emp)) {
        throw std::runtime_error("Failed to insert employee record into database.");
    }
    return emp;
}

bool EmployeeService::updateEmployee(int id, const std::string& name, const std::string& email,
                                    const std::string& phone, const std::string& role,
                                    const std::string& joiningDate) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Employee ID #" + std::to_string(id) + " not found.");
    }

    if (!isValidEmail(email)) {
        throw std::invalid_argument("Invalid email format: " + email);
    }

    auto emailOwner = repo.getByEmail(email);
    if (emailOwner.has_value() && emailOwner->getId() != id) {
        throw std::invalid_argument("Email '" + email + "' is already in use by another employee.");
    }

    Employee updated(id, name, email, phone, role, joiningDate, existing->getCreatedAt());
    return repo.updateEmployee(updated);
}

bool EmployeeService::deleteEmployee(int id) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Employee does not exist.");
    }

    int activeTickets = repo.getActiveTicketCount(id);
    if (activeTickets > 0) {
        throw std::runtime_error("Cannot delete employee: Has " + std::to_string(activeTickets) + 
                                 " active open/in-progress ticket(s). Reassign them first.");
    }

    return repo.deleteEmployee(id);
}

std::optional<Employee> EmployeeService::getEmployeeById(int id) {
    return repo.getById(id);
}

std::optional<Employee> EmployeeService::getEmployeeByEmail(const std::string& email) {
    return repo.getByEmail(email);
}

std::vector<Employee> EmployeeService::searchEmployees(const std::string& query) {
    return repo.search(query);
}

std::vector<Employee> EmployeeService::getAllEmployees() {
    return repo.getAll();
}

int EmployeeService::getAssignedTicketCount(int employeeId) {
    return repo.getAssignedTicketCount(employeeId);
}
