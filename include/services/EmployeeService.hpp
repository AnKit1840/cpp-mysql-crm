#ifndef EMPLOYEE_SERVICE_HPP
#define EMPLOYEE_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Employee.hpp"
#include "../repositories/EmployeeRepository.hpp"

class EmployeeService {
private:
    EmployeeRepository repo;
    bool isValidEmail(const std::string& email) const;

public:
    explicit EmployeeService(EmployeeRepository repository = EmployeeRepository());

    Employee registerEmployee(const std::string& name, const std::string& email,
                              const std::string& phone, const std::string& role,
                              const std::string& joiningDate = "");
    bool updateEmployee(int id, const std::string& name, const std::string& email,
                        const std::string& phone, const std::string& role,
                        const std::string& joiningDate);
    bool deleteEmployee(int id);

    std::optional<Employee> getEmployeeById(int id);
    std::optional<Employee> getEmployeeByEmail(const std::string& email);
    std::vector<Employee> searchEmployees(const std::string& query);
    std::vector<Employee> getAllEmployees();

    int getAssignedTicketCount(int employeeId);
};

#endif // EMPLOYEE_SERVICE_HPP
