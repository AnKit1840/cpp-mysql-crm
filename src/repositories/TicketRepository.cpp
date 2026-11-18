#include "../../include/repositories/TicketRepository.hpp"
#include <sstream>

using namespace std;

TicketRepository::TicketRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool TicketRepository::createTicket(SupportTicket& ticket) {
    string safeSubject = db.escapeString(ticket.getSubject());
    string safeDesc = db.escapeString(ticket.getDescription());
    string priorityStr = priorityToString(ticket.getPriority());
    string statusStr = statusToString(ticket.getStatus());

    stringstream ss;
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
    string statusStr = statusToString(newStatus);
    stringstream ss;
    ss << "UPDATE support_tickets SET status = '" << statusStr << "' ";
    if (newStatus == TicketStatus::RESOLVED || newStatus == TicketStatus::CLOSED) {
        ss << ", resolved_at = CURRENT_TIMESTAMP ";
    }
    ss << "WHERE ticket_id = " << ticketId << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool TicketRepository::assignTicket(int ticketId, int employeeId) {
    stringstream ss;
    ss << "UPDATE support_tickets SET assigned_employee_id = " << employeeId << ", "
       << "status = CASE WHEN status = 'OPEN' THEN 'IN_PROGRESS' ELSE status END "
       << "WHERE ticket_id = " << ticketId << ";";

    db.executeNonQuery(ss.str());
    return db.getAffectedRows() > 0;
}

bool TicketRepository::resolveTicket(int ticketId) {
    return updateTicketStatus(ticketId, TicketStatus::RESOLVED);
}

bool TicketRepository::addTicketUpdate(int ticketId, int employeeId, const string& updateText, const string& statusChange) {
    string safeText = db.escapeString(updateText);
    string safeChange = db.escapeString(statusChange);

    stringstream ss;
    ss << "INSERT INTO ticket_updates (ticket_id, employee_id, update_text, status_change) VALUES ("
       << ticketId << ", " << employeeId << ", '" << safeText << "', '" << safeChange << "');";

    db.executeNonQuery(ss.str());
    return db.getLastInsertId() > 0;
}

optional<SupportTicket> TicketRepository::getById(int ticketId) {
    stringstream ss;
    ss << "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
       << "t.subject, t.description, t.created_at, t.resolved_at, c.name, e.name "
       << "FROM support_tickets t "
       << "LEFT JOIN customers c ON t.customer_id = c.customer_id "
       << "LEFT JOIN employees e ON t.assigned_employee_id = e.employee_id "
       << "WHERE t.ticket_id = " << ticketId << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return nullopt;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return nullopt;
    }

    SupportTicket ticket(
        stoi(row[0]),
        stoi(row[1]),
        row[2] ? stoi(row[2]) : 0,
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

vector<SupportTicket> TicketRepository::getPendingTickets() {
    vector<SupportTicket> list;
    string sql = "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? stoi(row[2]) : 0,
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

vector<SupportTicket> TicketRepository::getTicketsByCustomer(int customerId) {
    vector<SupportTicket> list;
    stringstream ss;
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? stoi(row[2]) : 0,
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

vector<SupportTicket> TicketRepository::getTicketsByEmployee(int employeeId) {
    vector<SupportTicket> list;
    stringstream ss;
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? stoi(row[2]) : 0,
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

vector<SupportTicket> TicketRepository::getAll() {
    vector<SupportTicket> list;
    string sql = "SELECT t.ticket_id, t.customer_id, t.assigned_employee_id, t.priority, t.status, "
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
            stoi(row[0]),
            stoi(row[1]),
            row[2] ? stoi(row[2]) : 0,
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

vector<TicketUpdate> TicketRepository::getTicketHistory(int ticketId) {
    vector<TicketUpdate> list;
    stringstream ss;
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
            stoi(row[0]),
            stoi(row[1]),
            stoi(row[2]),
            row[3] ? row[3] : "",
            row[4] ? row[4] : "",
            row[5] ? row[5] : "",
            row[6] ? row[6] : ""
        );
    }
    mysql_free_result(res);
    return list;
}
