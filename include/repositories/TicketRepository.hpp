#ifndef TICKET_REPOSITORY_HPP
#define TICKET_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/SupportTicket.hpp"
#include "../models/TicketUpdate.hpp"
#include "../database/DatabaseManager.hpp"

using namespace std;

class TicketRepository {
private:
    DatabaseManager& db;

public:
    explicit TicketRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool createTicket(SupportTicket& ticket);
    bool updateTicketStatus(int ticketId, TicketStatus newStatus);
    bool assignTicket(int ticketId, int employeeId);
    bool resolveTicket(int ticketId);
    bool addTicketUpdate(int ticketId, int employeeId, const string& updateText, const string& statusChange);

    optional<SupportTicket> getById(int ticketId);
    vector<SupportTicket> getPendingTickets();
    vector<SupportTicket> getTicketsByCustomer(int customerId);
    vector<SupportTicket> getTicketsByEmployee(int employeeId);
    vector<SupportTicket> getAll();
    vector<TicketUpdate> getTicketHistory(int ticketId);
};

#endif // TICKET_REPOSITORY_HPP
