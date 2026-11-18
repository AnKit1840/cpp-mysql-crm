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

using namespace std;

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
 * - queue for FIFO processing of normal/low priority tickets
 * - priority_queue with custom TicketComparator for critical/high priority tickets
 * - Older-ticket-first tie-breaker for identical priorities
 * - unordered_set for O(1) duplicate ticket detection
 * - State synchronization with MySQL persistence layer
 */
class TicketManager {
private:
    TicketRepository ticketRepo;
    CustomerRepository customerRepo;
    EmployeeRepository employeeRepo;

    // 1. Priority Queue for Urgent Tickets (CRITICAL, HIGH)
    priority_queue<SupportTicket, vector<SupportTicket>, TicketComparator> urgentQueue;

    // 2. FIFO Standard Queue for Normal / Low Tickets
    queue<SupportTicket> normalQueue;

    // 3. Set to track ticket IDs currently queued in memory (prevents duplicate enqueuing)
    unordered_set<int> queuedTicketIds;

public:
    explicit TicketManager(TicketRepository tRepo = TicketRepository(),
                           CustomerRepository cRepo = CustomerRepository(),
                           EmployeeRepository eRepo = EmployeeRepository());

    // Hydrate queues from MySQL on application startup
    void syncFromDatabase();

    // Create a new support ticket and route it into the appropriate queue
    SupportTicket createTicket(int customerId, const string& subject,
                               const string& description, TicketPriority priority);

    // Process the next available ticket (Urgent queue takes precedence over Normal queue)
    optional<SupportTicket> processNextTicket(int employeeId, const string& notes);

    // Manual status update & resolution
    bool assignTicket(int ticketId, int employeeId);
    bool resolveTicket(int ticketId, int employeeId, const string& resolutionNotes);
    bool closeTicket(int ticketId, int employeeId, const string& closingNotes);

    // Inspection
    QueueStats getQueueStatistics() const;
    vector<SupportTicket> getAllTickets();
    vector<SupportTicket> getCustomerTickets(int customerId);
    vector<TicketUpdate> getTicketHistory(int ticketId);
    optional<SupportTicket> getTicketById(int ticketId);
};

#endif // TICKET_MANAGER_HPP
