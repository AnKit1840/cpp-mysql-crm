#ifndef SUPPORT_TICKET_HPP
#define SUPPORT_TICKET_HPP

#include <string>
#include <iostream>
#include <iomanip>

enum class TicketPriority {
    CRITICAL = 1,
    HIGH = 2,
    NORMAL = 3,
    LOW = 4
};

enum class TicketStatus {
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

inline std::string priorityToString(TicketPriority priority) {
    switch (priority) {
        case TicketPriority::CRITICAL: return "CRITICAL";
        case TicketPriority::HIGH:     return "HIGH";
        case TicketPriority::NORMAL:   return "NORMAL";
        case TicketPriority::LOW:      return "LOW";
        default:                       return "NORMAL";
    }
}

inline TicketPriority stringToPriority(const std::string& str) {
    if (str == "CRITICAL") return TicketPriority::CRITICAL;
    if (str == "HIGH")     return TicketPriority::HIGH;
    if (str == "NORMAL")   return TicketPriority::NORMAL;
    if (str == "LOW")      return TicketPriority::LOW;
    return TicketPriority::NORMAL;
}

inline std::string statusToString(TicketStatus status) {
    switch (status) {
        case TicketStatus::OPEN:        return "OPEN";
        case TicketStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TicketStatus::RESOLVED:    return "RESOLVED";
        case TicketStatus::CLOSED:      return "CLOSED";
        default:                        return "OPEN";
    }
}

inline TicketStatus stringToStatus(const std::string& str) {
    if (str == "OPEN")        return TicketStatus::OPEN;
    if (str == "IN_PROGRESS") return TicketStatus::IN_PROGRESS;
    if (str == "RESOLVED")    return TicketStatus::RESOLVED;
    if (str == "CLOSED")      return TicketStatus::CLOSED;
    return TicketStatus::OPEN;
}

/**
 * @brief Represents a customer support issue.
 * Used inside std::queue (FIFO) and std::priority_queue.
 */
class SupportTicket {
private:
    int id;
    int customerId;
    std::string customerName; // Join cache
    int assignedEmployeeId;   // 0 if unassigned
    std::string employeeName; // Join cache
    TicketPriority priority;
    TicketStatus status;
    std::string subject;
    std::string description;
    std::string createdAt;
    std::string resolvedAt;

public:
    SupportTicket()
        : id(0), customerId(0), customerName(""), assignedEmployeeId(0), employeeName(""),
          priority(TicketPriority::NORMAL), status(TicketStatus::OPEN),
          subject(""), description(""), createdAt(""), resolvedAt("") {}

    SupportTicket(int id, int customerId, int assignedEmployeeId,
                  TicketPriority priority, TicketStatus status,
                  const std::string& subject, const std::string& description,
                  const std::string& createdAt = "", const std::string& resolvedAt = "",
                  const std::string& customerName = "", const std::string& employeeName = "")
        : id(id), customerId(customerId), customerName(customerName),
          assignedEmployeeId(assignedEmployeeId), employeeName(employeeName),
          priority(priority), status(status), subject(subject),
          description(description), createdAt(createdAt), resolvedAt(resolvedAt) {}

    // Getters
    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    std::string getCustomerName() const { return customerName; }
    int getAssignedEmployeeId() const { return assignedEmployeeId; }
    std::string getEmployeeName() const { return employeeName; }
    TicketPriority getPriority() const { return priority; }
    TicketStatus getStatus() const { return status; }
    std::string getSubject() const { return subject; }
    std::string getDescription() const { return description; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getResolvedAt() const { return resolvedAt; }

    // Setters
    void setId(int newId) { id = newId; }
    void setCustomerId(int cId) { customerId = cId; }
    void setCustomerName(const std::string& name) { customerName = name; }
    void setAssignedEmployeeId(int empId) { assignedEmployeeId = empId; }
    void setEmployeeName(const std::string& name) { employeeName = name; }
    void setPriority(TicketPriority p) { priority = p; }
    void setStatus(TicketStatus s) { status = s; }
    void setSubject(const std::string& sub) { subject = sub; }
    void setDescription(const std::string& desc) { description = desc; }
    void setCreatedAt(const std::string& time) { createdAt = time; }
    void setResolvedAt(const std::string& time) { resolvedAt = time; }

    bool isUrgent() const {
        return priority == TicketPriority::CRITICAL || priority == TicketPriority::HIGH;
    }

    bool isPending() const {
        return status == TicketStatus::OPEN || status == TicketStatus::IN_PROGRESS;
    }

    void displayDetails() const {
        std::cout << "Ticket ID    : #" << id << "\n"
                  << "Customer     : " << (customerName.empty() ? ("Customer #" + std::to_string(customerId)) : customerName) << "\n"
                  << "Assigned To  : " << (assignedEmployeeId == 0 ? "Unassigned" : (employeeName.empty() ? ("Employee #" + std::to_string(assignedEmployeeId)) : employeeName)) << "\n"
                  << "Priority     : " << priorityToString(priority) << "\n"
                  << "Status       : " << statusToString(status) << "\n"
                  << "Subject      : " << subject << "\n"
                  << "Description  : " << description << "\n"
                  << "Created At   : " << createdAt << "\n"
                  << "Resolved At  : " << (resolvedAt.empty() ? "Pending" : resolvedAt) << "\n";
    }
};

/**
 * @brief Custom Comparator for std::priority_queue<SupportTicket>.
 * 
 * Demonstrates DSA Knowledge:
 * 1. Urgency: Higher priority tickets (CRITICAL=1, HIGH=2) are processed before lower ones.
 * 2. FIFO Tie-breaker: If two tickets share identical priority, the older ticket (lower ID / timestamp)
 *    must be processed first.
 *
 * Note: In C++ std::priority_queue (max-heap), returning `true` means element `a` has lower priority
 * than `b` (so `b` is placed higher).
 */
struct TicketComparator {
    bool operator()(const SupportTicket& a, const SupportTicket& b) const {
        // Lower numerical value represents higher priority (1=CRITICAL, 2=HIGH)
        if (a.getPriority() != b.getPriority()) {
            return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
        }
        // Tie-breaker: Older ticket (smaller ID) gets higher precedence
        return a.getId() > b.getId();
    }
};

#endif // SUPPORT_TICKET_HPP
