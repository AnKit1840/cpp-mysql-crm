#ifndef EMPLOYEE_REPOSITORY_HPP
#define EMPLOYEE_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/Employee.hpp"
#include "../database/DatabaseManager.hpp"

using namespace std;

class EmployeeRepository {
private:
    DatabaseManager& db;

public:
    explicit EmployeeRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool addEmployee(Employee& employee);
    bool updateEmployee(const Employee& employee);
    bool deleteEmployee(int employeeId);

    optional<Employee> getById(int employeeId);
    optional<Employee> getByEmail(const string& email);
    vector<Employee> search(const string& searchTerm);
    vector<Employee> getAll();

    int getAssignedTicketCount(int employeeId);
    int getActiveTicketCount(int employeeId);
};

#endif // EMPLOYEE_REPOSITORY_HPP
