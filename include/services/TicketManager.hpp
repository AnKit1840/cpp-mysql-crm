#ifndef TICKET_MANAGER_HPP
#define TICKET_MANAGER_HPP

#include <queue>
#include <vector>
#include <unordered_set>
#include <optional>
#include <string>
#include "../models/SupportTicket.hpp"
#include "../models/TicketUpdate.hpp"
#include "../repositories/TicketRepository.hpp"
#include "../repositories/CustomerRepository.hpp"
#include "../repositories/EmployeeRepository.hpp"

struct QueueStats {
    size_t urgentQueueSize = 0;
    size_t normalQueueSize = 0;
    size_t totalPending = 0;
};

/**
 * @brief TicketManager handles in-memory Queue and Priority Queue data structures
 * synchronized with MySQL.
 * 
 * Demonstrates:
 * - std::queue for FIFO processing of normal/low priority tickets
 * - std::priority_queue with custom TicketComparator for critical/high priority tickets
 * - Older-ticket-first tie-breaker for identical priorities
 * - std::unordered_set for O(1) duplicate ticket detection
 * - State synchronization with MySQL persistence layer
 */
class TicketManager {
private:
    TicketRepository ticketRepo;
    CustomerRepository customerRepo;
    EmployeeRepository employeeRepo;

    // 1. Priority Queue for Urgent Tickets (CRITICAL, HIGH)
    std::priority_queue<SupportTicket, std::vector<SupportTicket>, TicketComparator> urgentQueue;

    // 2. FIFO Standard Queue for Normal / Low Tickets
    std::queue<SupportTicket> normalQueue;

    // 3. Set to track ticket IDs currently queued in memory (prevents duplicate enqueuing)
    std::unordered_set<int> queuedTicketIds;

public:
    explicit TicketManager(TicketRepository tRepo = TicketRepository(),
                           CustomerRepository cRepo = CustomerRepository(),
                           EmployeeRepository eRepo = EmployeeRepository());

    // Hydrate queues from MySQL on application startup
    void syncFromDatabase();

    // Create a new support ticket and route it into the appropriate queue
    SupportTicket createTicket(int customerId, const std::string& subject,
                               const std::string& description, TicketPriority priority);

    // Process the next available ticket (Urgent queue takes precedence over Normal queue)
    std::optional<SupportTicket> processNextTicket(int employeeId, const std::string& notes);

    // Manual status update & resolution
    bool assignTicket(int ticketId, int employeeId);
    bool resolveTicket(int ticketId, int employeeId, const std::string& resolutionNotes);
    bool closeTicket(int ticketId, int employeeId, const std::string& closingNotes);

    // Inspection
    QueueStats getQueueStatistics() const;
    std::vector<SupportTicket> getAllTickets();
    std::vector<SupportTicket> getCustomerTickets(int customerId);
    std::vector<TicketUpdate> getTicketHistory(int ticketId);
    std::optional<SupportTicket> getTicketById(int ticketId);
};

#endif // TICKET_MANAGER_HPP
