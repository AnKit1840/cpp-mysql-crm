#ifndef TICKET_UPDATE_HPP
#define TICKET_UPDATE_HPP

#include <string>
#include <iostream>

class TicketUpdate {
private:
    int id;
    int ticketId;
    int employeeId;
    std::string employeeName;
    std::string updateText;
    std::string statusChange;
    std::string updatedAt;

public:
    TicketUpdate()
        : id(0), ticketId(0), employeeId(0), employeeName(""),
          updateText(""), statusChange(""), updatedAt("") {}

    TicketUpdate(int id, int ticketId, int employeeId, const std::string& updateText,
                 const std::string& statusChange, const std::string& updatedAt = "",
                 const std::string& employeeName = "")
        : id(id), ticketId(ticketId), employeeId(employeeId), employeeName(employeeName),
          updateText(updateText), statusChange(statusChange), updatedAt(updatedAt) {}

    int getId() const { return id; }
    int getTicketId() const { return ticketId; }
    int getEmployeeId() const { return employeeId; }
    std::string getEmployeeName() const { return employeeName; }
    std::string getUpdateText() const { return updateText; }
    std::string getStatusChange() const { return statusChange; }
    std::string getUpdatedAt() const { return updatedAt; }

    void setId(int newId) { id = newId; }
    void setTicketId(int tId) { ticketId = tId; }
    void setEmployeeId(int eId) { employeeId = eId; }
    void setEmployeeName(const std::string& name) { employeeName = name; }
    void setUpdateText(const std::string& text) { updateText = text; }
    void setStatusChange(const std::string& change) { statusChange = change; }
    void setUpdatedAt(const std::string& time) { updatedAt = time; }

    void display() const {
        std::cout << "  - [" << updatedAt << "] Updated by " 
                  << (employeeName.empty() ? ("Employee #" + std::to_string(employeeId)) : employeeName)
                  << " (" << statusChange << "): " << updateText << "\n";
    }
};

#endif // TICKET_UPDATE_HPP
