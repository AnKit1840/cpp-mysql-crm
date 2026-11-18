#include "../../include/services/TicketManager.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

TicketManager::TicketManager(TicketRepository tRepo, CustomerRepository cRepo, EmployeeRepository eRepo)
    : ticketRepo(tRepo), customerRepo(cRepo), employeeRepo(eRepo) {
    // Note: Caller can invoke syncFromDatabase() after DB connection is established
}

void TicketManager::syncFromDatabase() {
    // Clear in-memory structures
    while (!urgentQueue.empty()) urgentQueue.pop();
    while (!normalQueue.empty()) normalQueue.pop();
    queuedTicketIds.clear();

    // Query pending tickets from MySQL
    vector<SupportTicket> pending = ticketRepo.getPendingTickets();
    for (const auto& ticket : pending) {
        if (queuedTicketIds.find(ticket.getId()) == queuedTicketIds.end()) {
            queuedTicketIds.insert(ticket.getId());

            if (ticket.isUrgent()) {
                urgentQueue.push(ticket);
            } else {
                normalQueue.push(ticket);
            }
        }
    }
}

SupportTicket TicketManager::createTicket(int customerId, const string& subject,
                                         const string& description, TicketPriority priority) {
    auto customer = customerRepo.getById(customerId);
    if (!customer.has_value()) {
        throw invalid_argument("Customer ID #" + to_string(customerId) + " not found.");
    }
    if (subject.empty()) {
        throw invalid_argument("Ticket subject cannot be empty.");
    }
    if (description.empty()) {
        throw invalid_argument("Ticket description cannot be empty.");
    }

    SupportTicket ticket(0, customerId, 0, priority, TicketStatus::OPEN, subject, description, "", "", customer->getName(), "");
    if (!ticketRepo.createTicket(ticket)) {
        throw runtime_error("Failed to insert support ticket into database.");
    }

    // Route into appropriate in-memory queue
    if (queuedTicketIds.find(ticket.getId()) == queuedTicketIds.end()) {
        queuedTicketIds.insert(ticket.getId());
        if (ticket.isUrgent()) {
            urgentQueue.push(ticket);
        } else {
            normalQueue.push(ticket);
        }
    }

    return ticket;
}

optional<SupportTicket> TicketManager::processNextTicket(int employeeId, const string& notes) {
    auto employee = employeeRepo.getById(employeeId);
    if (!employee.has_value()) {
        throw invalid_argument("Employee ID #" + to_string(employeeId) + " not found.");
    }

    SupportTicket ticketToProcess;
    bool found = false;

    // 1. Process from Urgent Priority Queue first (CRITICAL, HIGH)
    while (!urgentQueue.empty() && !found) {
        ticketToProcess = urgentQueue.top();
        urgentQueue.pop();
        queuedTicketIds.erase(ticketToProcess.getId());

        // Refresh from DB to verify it wasn't already resolved by another action
        auto currentDbState = ticketRepo.getById(ticketToProcess.getId());
        if (currentDbState.has_value() && currentDbState->isPending()) {
            ticketToProcess = *currentDbState;
            found = true;
        }
    }

    // 2. If no urgent tickets, process from Standard FIFO Queue (NORMAL, LOW)
    while (!found && !normalQueue.empty()) {
        ticketToProcess = normalQueue.front();
        normalQueue.pop();
        queuedTicketIds.erase(ticketToProcess.getId());

        auto currentDbState = ticketRepo.getById(ticketToProcess.getId());
        if (currentDbState.has_value() && currentDbState->isPending()) {
            ticketToProcess = *currentDbState;
            found = true;
        }
    }

    if (!found) {
        return nullopt; // No tickets pending
    }

    // 3. Assign and transition to IN_PROGRESS in MySQL
    ticketRepo.assignTicket(ticketToProcess.getId(), employeeId);
    ticketRepo.addTicketUpdate(ticketToProcess.getId(), employeeId, 
                              notes.empty() ? "Assigned and moved to in-progress for analysis." : notes,
                              "OPEN -> IN_PROGRESS");

    ticketToProcess.setAssignedEmployeeId(employeeId);
    ticketToProcess.setEmployeeName(employee->getName());
    ticketToProcess.setStatus(TicketStatus::IN_PROGRESS);

    return ticketToProcess;
}

bool TicketManager::assignTicket(int ticketId, int employeeId) {
    auto ticket = ticketRepo.getById(ticketId);
    if (!ticket.has_value()) {
        throw invalid_argument("Ticket ID #" + to_string(ticketId) + " not found.");
    }
    auto employee = employeeRepo.getById(employeeId);
    if (!employee.has_value()) {
        throw invalid_argument("Employee ID #" + to_string(employeeId) + " not found.");
    }

    bool ok = ticketRepo.assignTicket(ticketId, employeeId);
    if (ok) {
        ticketRepo.addTicketUpdate(ticketId, employeeId, "Assigned to " + employee->getName(), "ASSIGNMENT");
    }
    return ok;
}

bool TicketManager::resolveTicket(int ticketId, int employeeId, const string& resolutionNotes) {
    auto ticket = ticketRepo.getById(ticketId);
    if (!ticket.has_value()) {
        throw invalid_argument("Ticket ID #" + to_string(ticketId) + " not found.");
    }

    bool ok = ticketRepo.resolveTicket(ticketId);
    if (ok) {
        ticketRepo.addTicketUpdate(ticketId, employeeId, resolutionNotes, "IN_PROGRESS -> RESOLVED");
        // Ensure removed from queued tracking
        queuedTicketIds.erase(ticketId);
    }
    return ok;
}

bool TicketManager::closeTicket(int ticketId, int employeeId, const string& closingNotes) {
    auto ticket = ticketRepo.getById(ticketId);
    if (!ticket.has_value()) {
        throw invalid_argument("Ticket ID #" + to_string(ticketId) + " not found.");
    }

    bool ok = ticketRepo.updateTicketStatus(ticketId, TicketStatus::CLOSED);
    if (ok) {
        ticketRepo.addTicketUpdate(ticketId, employeeId, closingNotes, "RESOLVED -> CLOSED");
        queuedTicketIds.erase(ticketId);
    }
    return ok;
}

QueueStats TicketManager::getQueueStatistics() const {
    QueueStats stats;
    stats.urgentQueueSize = urgentQueue.size();
    stats.normalQueueSize = normalQueue.size();
    stats.totalPending = stats.urgentQueueSize + stats.normalQueueSize;
    return stats;
}

vector<SupportTicket> TicketManager::getAllTickets() {
    return ticketRepo.getAll();
}

vector<SupportTicket> TicketManager::getCustomerTickets(int customerId) {
    return ticketRepo.getTicketsByCustomer(customerId);
}

vector<TicketUpdate> TicketManager::getTicketHistory(int ticketId) {
    return ticketRepo.getTicketHistory(ticketId);
}

optional<SupportTicket> TicketManager::getTicketById(int ticketId) {
    return ticketRepo.getById(ticketId);
}
