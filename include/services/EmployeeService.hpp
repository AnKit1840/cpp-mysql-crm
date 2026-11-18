#ifndef EMPLOYEE_SERVICE_HPP
#define EMPLOYEE_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Employee.hpp"
#include "../repositories/EmployeeRepository.hpp"

using namespace std;

class EmployeeService {
private:
    EmployeeRepository repo;
    bool isValidEmail(const string& email) const;

public:
    explicit EmployeeService(EmployeeRepository repository = EmployeeRepository());

    Employee registerEmployee(const string& name, const string& email,
                              const string& phone, const string& role,
                              const string& joiningDate = "");
    bool updateEmployee(int id, const string& name, const string& email,
                        const string& phone, const string& role,
                        const string& joiningDate);
    bool deleteEmployee(int id);

    optional<Employee> getEmployeeById(int id);
    optional<Employee> getEmployeeByEmail(const string& email);
    vector<Employee> searchEmployees(const string& query);
    vector<Employee> getAllEmployees();

    int getAssignedTicketCount(int employeeId);
};

#endif // EMPLOYEE_SERVICE_HPP
