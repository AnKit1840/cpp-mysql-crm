#include "../../include/repositories/TicketRepository.hpp"
#include <sstream>

TicketRepository::TicketRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool TicketRepository::createTicket(SupportTicket& ticket) {
    std::string safeSubject = db.escapeString(ticket.getSubject());
    std::string safeDesc = db.escapeString(ticket.getDescription());
    std::string priorityStr = priorityToString(ticket.getPriority());
    std::string statusStr = statusToString(ticket.getStatus());

    std::stringstream ss;
    ss << "INSERT INTO support_tickets (customer_id, assigned_employee_id, priority, status, subject, description) VALUES ("
       << ticket.getCustomerId() << ", ";
    if (ticket.getAssignedEmployeeId() > 0) {
        ss << ticket.getAssignedEmployeeId();
    } else {
        ss << "NULL";
    }
    ss << ", '" << priorityStr << "', '" << statusStr << "', '" << safeSubject << "', '" << safeDesc << "');";

    db.executeNonQuery(ss.str());
    ticket.setId(static_cast<int>(db.getLastInsertId()));
    return ticket.getId() > 0;
}

bool TicketRepository::updateTicketStatus(int ticketId, TicketStatus newStatus) {
    std::string statusStr = statusToString(newStatus);
    std::stringstream ss;
    ss << "UPDATE support_tickets SET status = '" << statusStr << "' ";
    if (newStatus == TicketStatus::RESOLVED || newStatus == TicketStatus::CLOSED) {
        ss << ", resolved_at = CURRENT_TIMESTAMP ";
    }
    ss << "WHERE ticket_id = " << ticketId << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool TicketRepository::assignTicket(int ticketId, int employeeId) {
    std::stringstream ss;
    ss << "UPDATE support_tickets SET assigned_employee_id = " << employeeId << ", "
       << "status = CASE WHEN status = 'OPEN' THEN 'IN_PROGRESS' ELSE status END "
       << "WHERE ticket_id = " << ticketId << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool TicketRepository::resolveTicket(int ticketId) {
    return updateTicketStatus(ticketId, TicketStatus::RESOLVED);
}

bool TicketRepository::addTicketUpdate(int ticketId, int employeeId, const std::string& updateText, const std::string& statusChange) {
    std::string safeText = db.escapeString(updateText);
    std::string safeChange = db.escapeString(statusChange);

    std::stringstream ss;
    ss << "INSERT INTO ticket_updates (ticket_id, employee_id, update_text, status_change) VALUES ("
       << ticketId << ", " << employeeId << ", '" << safeText << "', '" << safeChange << "');";

    db.executeNonQuery(ss.str());
    return db.getLastInsertId() > 0;
}

std::optional<SupportTicket> TicketRepository::getById(int ticketId) {
    std::stringstream ss;
    ss << "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
       << "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
       << "FROM support_tickets t "
       << "LEFT JOIN customers c ON t.customer_id = c.customer_id "
       << "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
       << "WHERE t.ticket_id = " << ticketId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return std::nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return std::nullopt;
    }

    SupportTicket ticket(
        std::stoi(row[0]),
        std::stoi(row[1]),
        row[2] ? std::stoi(row[2]) : 0,
        stringToPriority(row[3] ? row[3] : "NORMAL"),
        stringToStatus(row[4] ? row[4] : "OPEN"),
        row[5] ? row[5] : "",
        row[6] ? row[6] : "",
        row[7] ? row[7] : "",
        row[8] ? row[8] : "",
        row[9] ? row[9] : "",
        row[10] ? row[10] : ""
    );

    mysql_free_result(res);
    return ticket;
}

std::vector<SupportTicket> TicketRepository::getPendingTickets() {
    std::vector<SupportTicket> list;
    std::string sql = "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
                      "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
                      "FROM support_tickets t "
                      "LEFT JOIN customers c ON t.customer_id = c.customer_id "
                      "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
                      "WHERE t.status IN ('OPEN', 'IN_PROGRESS') "
                      "ORDER BY t.ticket_id ASC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? std::stoi(row[2]) : 0,
            stringToPriority(row[3] ? row[3] : "NORMAL"),
            stringToStatus(row[4] ? row[4] : "OPEN"),
            row[5] ? row[5] : "",
            row[6] ? row[6] : "",
            row[7] ? row[7] : "",
            row[8] ? row[8] : "",
            row[9] ? row[9] : "",
            row[10] ? row[10] : ""
        );
    }
    mysql_free_result(res);
    return list;
}

std::vector<SupportTicket> TicketRepository::getTicketsByCustomer(int customerId) {
    std::vector<SupportTicket> list;
    std::stringstream ss;
    ss << "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
       << "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
       << "FROM support_tickets t "
       << "LEFT JOIN customers c ON t.customer_id = c.customer_id "
       << "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
       << "WHERE t.customer_id = " << customerId << " "
       << "ORDER BY t.created_at DESC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? std::stoi(row[2]) : 0,
            stringToPriority(row[3] ? row[3] : "NORMAL"),
            stringToStatus(row[4] ? row[4] : "OPEN"),
            row[5] ? row[5] : "",
            row[6] ? row[6] : "",
            row[7] ? row[7] : "",
            row[8] ? row[8] : "",
            row[9] ? row[9] : "",
            row[10] ? row[10] : ""
        );
    }
    mysql_free_result(res);
    return list;
}

std::vector<SupportTicket> TicketRepository::getTicketsByEmployee(int employeeId) {
    std::vector<SupportTicket> list;
    std::stringstream ss;
    ss << "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
       << "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
       << "FROM support_tickets t "
       << "LEFT JOIN customers c ON t.customer_id = c.customer_id "
       << "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
       << "WHERE t.assigned_employee_id = " << employeeId << " "
       << "ORDER BY t.created_at DESC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? std::stoi(row[2]) : 0,
            stringToPriority(row[3] ? row[3] : "NORMAL"),
            stringToStatus(row[4] ? row[4] : "OPEN"),
            row[5] ? row[5] : "",
            row[6] ? row[6] : "",
            row[7] ? row[7] : "",
            row[8] ? row[8] : "",
            row[9] ? row[9] : "",
            row[10] ? row[10] : ""
        );
    }
    mysql_free_result(res);
    return list;
}

std::vector<SupportTicket> TicketRepository::getAll() {
    std::vector<SupportTicket> list;
    std::string sql = "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
                      "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
                      "FROM support_tickets t "
                      "LEFT JOIN customers c ON t.customer_id = c.customer_id "
                      "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
                      "ORDER BY t.ticket_id DESC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            row[2] ? std::stoi(row[2]) : 0,
            stringToPriority(row[3] ? row[3] : "NORMAL"),
            stringToStatus(row[4] ? row[4] : "OPEN"),
            row[5] ? row[5] : "",
            row[6] ? row[6] : "",
            row[7] ? row[7] : "",
            row[8] ? row[8] : "",
            row[9] ? row[9] : "",
            row[10] ? row[10] : ""
        );
    }
    mysql_free_result(res);
    return list;
}

std::vector<TicketUpdate> TicketRepository::getTicketHistory(int ticketId) {
    std::vector<TicketUpdate> list;
    std::stringstream ss;
    ss << "SELECT u.update_id, u.ticket_id, u.employee_id, u.update_text, u.status_change, u.updated_at, e.name "
       << "FROM ticket_updates u "
       << "LEFT JOIN employees e ON u.employee_id = e.employee_id "
       << "WHERE u.ticket_id = " << ticketId << " "
       << "ORDER BY u.updated_at ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        list.emplace_back(
            std::stoi(row[0]),
            std::stoi(row[1]),
            std::stoi(row[2]),
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }
    mysql_free_result(res);
    return list;
}
