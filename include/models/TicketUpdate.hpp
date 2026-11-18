#ifndef TICKET_UPDATE_HPP
#define TICKET_UPDATE_HPP

#include <string>
#include <iostream>

using namespace std;

class TicketUpdate {
private:
    int id;
    int ticketId;
    int employeeId;
    string employeeName;
    string updateText;
    string statusChange;
    string updatedAt;

public:
    TicketUpdate()
        : id(0), ticketId(0), employeeId(0), employeeName(""),
          updateText(""), statusChange(""), updatedAt("") {}

    TicketUpdate(int id, int ticketId, int employeeId, const string& updateText,
                 const string& statusChange, const string& updatedAt = "",
                 const string& employeeName = "")
        : id(id), ticketId(ticketId), employeeId(employeeId), employeeName(employeeName),
          updateText(updateText), statusChange(statusChange), updatedAt(updatedAt) {}

    int getId() const { return id; }
    int getTicketId() const { return ticketId; }
    int getEmployeeId() const { return employeeId; }
    string getEmployeeName() const { return employeeName; }
    string getUpdateText() const { return updateText; }
    string getStatusChange() const { return statusChange; }
    string getUpdatedAt() const { return updatedAt; }

    void setId(int newId) { id = newId; }
    void setTicketId(int tId) { ticketId = tId; }
    void setEmployeeId(int eId) { employeeId = eId; }
    void setEmployeeName(const string& name) { employeeName = name; }
    void setUpdateText(const string& text) { updateText = text; }
    void setStatusChange(const string& change) { statusChange = change; }
    void setUpdatedAt(const string& time) { updatedAt = time; }

    void display() const {
        cout << "  - [" << updatedAt << "] Updated by " 
                  << (employeeName.empty() ? ("Employee #" + to_string(employeeId)) : employeeName)
                  << " (" << statusChange << "): " << updateText << "\n";
    }
};

#endif // TICKET_UPDATE_HPP
