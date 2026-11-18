#ifndef TICKET_REPOSITORY_HPP
#define TICKET_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/SupportTicket.hpp"
#include "../models/TicketUpdate.hpp"
#include "../database/DatabaseManager.hpp"

class TicketRepository {
private:
    DatabaseManager& db;

public:
    explicit TicketRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool createTicket(SupportTicket& ticket);
    bool updateTicketStatus(int ticketId, TicketStatus newStatus);
    bool assignTicket(int ticketId, int employeeId);
    bool resolveTicket(int ticketId);
    bool addTicketUpdate(int ticketId, int employeeId, const std::string& updateText, const std::string& statusChange);

    std::optional<SupportTicket> getById(int ticketId);
    std::vector<SupportTicket> getPendingTickets();
    std::vector<SupportTicket> getTicketsByCustomer(int customerId);
    std::vector<SupportTicket> getTicketsByEmployee(int employeeId);
    std::vector<SupportTicket> getAll();
    std::vector<TicketUpdate> getTicketHistory(int ticketId);
};

#endif // TICKET_REPOSITORY_HPP
