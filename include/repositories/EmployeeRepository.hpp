#ifndef EMPLOYEE_REPOSITORY_HPP
#define EMPLOYEE_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/Employee.hpp"
#include "../database/DatabaseManager.hpp"

class EmployeeRepository {
private:
    DatabaseManager& db;

public:
    explicit EmployeeRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool addEmployee(Employee& employee);
    bool updateEmployee(const Employee& employee);
    bool deleteEmployee(int employeeId);

    std::optional<Employee> getById(int employeeId);
    std::optional<Employee> getByEmail(const std::string& email);
    std::vector<Employee> search(const std::string& searchTerm);
    std::vector<Employee> getAll();

    int getAssignedTicketCount(int employeeId);
    int getActiveTicketCount(int employeeId);
};

#endif // EMPLOYEE_REPOSITORY_HPP
