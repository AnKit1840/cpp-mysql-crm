#include "../../include/repositories/EmployeeRepository.hpp"
#include <sstream>

EmployeeRepository::EmployeeRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool EmployeeRepository::addEmployee(Employee& employee) {
    std::string safeName = db.escapeString(employee.getName());
    std::string safeEmail = db.escapeString(employee.getEmail());
    std::string safePhone = db.escapeString(employee.getPhone());
    std::string safeRole = db.escapeString(employee.getEmployeeRole());
    std::string safeJoining = db.escapeString(employee.getJoiningDate().empty() ? "CURDATE()" : ("'" + employee.getJoiningDate() + "'"));

    std::stringstream ss;
    ss << "INSERT INTO employees (name, email, phone, role, joining_date) VALUES ('"
       << safeName << "', '" << safeEmail << "', '" << safePhone << "', '" << safeRole << "', " << safeJoining << ");";

    db.executeNonQuery(ss.str());
    employee.setId(static_cast<int>(db.getLastInsertId()));
    return employee.getId() > 0;
}

bool EmployeeRepository::updateEmployee(const Employee& employee) {
    std::string safeName = db.escapeString(employee.getName());
    std::string safeEmail = db.escapeString(employee.getEmail());
    std::string safePhone = db.escapeString(employee.getPhone());
    std::string safeRole = db.escapeString(employee.getEmployeeRole());
    std::string safeJoining = db.escapeString(employee.getJoiningDate());

    std::stringstream ss;
    ss << "UPDATE employees SET "
       << "name = '" << safeName << "', "
       << "email = '" << safeEmail << "', "
       << "phone = '" << safePhone << "', "
       << "role = '" << safeRole << "', "
       << "joining_date = '" << safeJoining << "' "
       << "WHERE employee_id = " << employee.getId() << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool EmployeeRepository::deleteEmployee(int employeeId) {
    std::stringstream ss;
    ss << "DELETE FROM employees WHERE employee_id = " << employeeId << ";";
    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

std::optional<Employee> EmployeeRepository::getById(int employeeId) {
    std::stringstream ss;
    ss << "SELECT employee_id, name, email, phone, role, joining_date FROM employees WHERE employee_id = " 
       << employeeId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Employee emp(
        std::stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return emp;
}

std::optional<Employee> EmployeeRepository::getByEmail(const std::string& email) {
    std::string safeEmail = db.escapeString(email);
    std::stringstream ss;
    ss << "SELECT employee_id, name, email, phone, role, joining_date FROM employees WHERE email = '" 
       << safeEmail << "';";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    Employee emp(
        std::stoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );

    mysql_free_result(res);
    return emp;
}

std::vector<Employee> EmployeeRepository::search(const std::string& searchTerm) {
    std::vector<Employee> list;
    std::string safeTerm = db.escapeString(searchTerm);

    std::stringstream ss;
    ss << "SELECT employee_id, name, email, phone, role, joining_date FROM employees "
       << "WHERE employee_id = '" << safeTerm << "' OR name LIKE '%" << safeTerm << "%' "
       << "OR email LIKE '%" << safeTerm << "%' OR role LIKE '%" << safeTerm << "%' "
       << "ORDER BY employee_id ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            row[1] ? row[1] : "",
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

std::vector<Employee> EmployeeRepository::getAll() {
    std::vector<Employee> list;
    MYSQL_RES* res = db.executeQuery("SELECT employee_id, name, email, phone, role, joining_date FROM employees ORDER BY employee_id ASC;");
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            row[1] ? row[1] : "",
            row[2] ? row[2] : "",
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : ""
        );
    }

    mysql_free_result(res);
    return list;
}

int EmployeeRepository::getAssignedTicketCount(int employeeId) {
    std::stringstream ss;
    ss << "SELECT COUNT(*) FROM support_tickets WHERE assigned_employee_id = " << employeeId << ";";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = std::stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}

int EmployeeRepository::getActiveTicketCount(int employeeId) {
    std::stringstream ss;
    ss << "SELECT COUNT(*) FROM support_tickets WHERE assigned_employee_id = " << employeeId 
       << " AND status IN ('OPEN', 'IN_PROGRESS');";
    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return 0;

    int count = 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && row[0]) {
        count = std::stoi(row[0]);
    }
    mysql_free_result(res);
    return count;
}
