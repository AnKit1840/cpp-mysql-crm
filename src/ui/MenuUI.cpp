#include "../../include/ui/MenuUI.hpp"
#include "../../include/ui/ConsoleUtils.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

MenuUI::MenuUI() {
    try {
        ticketManager.syncFromDatabase();
    } catch (const exception& e) {
        cerr << "[Warning] Could not initialize ticket queues from database: " << e.what() << "\n";
    }
}

void MenuUI::run() {
    while (true) {
        ConsoleUtils::printHeader("CRM MANAGEMENT SYSTEM");
        cout << "1. Customer Management\n"
                  << "2. Employee Management\n"
                  << "3. Product Management\n"
                  << "4. Order Management\n"
                  << "5. Support Ticket Management (Queue / Priority Queue)\n"
                  << "6. Sales Analytics\n"
                  << "7. Exit\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");

        switch (choice) {
            case 1: handleCustomerMenu(); break;
            case 2: handleEmployeeMenu(); break;
            case 3: handleProductMenu(); break;
            case 4: handleOrderMenu(); break;
            case 5: handleTicketMenu(); break;
            case 6: handleAnalyticsMenu(); break;
            case 7:
                cout << "\nExiting CRM Management System. Goodbye!\n";
                return;
            default:
                cout << "Invalid selection. Please choose 1-7.\n";
                ConsoleUtils::pause();
                break;
        }
    }
}

// ============================================================================
// MODULE 1: CUSTOMER MANAGEMENT
// ============================================================================

void MenuUI::handleCustomerMenu() {
    while (true) {
        ConsoleUtils::printHeader("CUSTOMER MANAGEMENT");
        cout << "1. Add New Customer\n"
                  << "2. Update Customer Details\n"
                  << "3. Delete Customer\n"
                  << "4. Search Customers (ID, Name, or Email)\n"
                  << "5. Display All Customers\n"
                  << "6. View Customer Order History\n"
                  << "7. View Customer Support Tickets\n"
                  << "8. Back to Main Menu\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 8) break;

        switch (choice) {
            case 1: addCustomerAction(); break;
            case 2: updateCustomerAction(); break;
            case 3: deleteCustomerAction(); break;
            case 4: searchCustomerAction(); break;
            case 5: displayCustomersAction(); break;
            case 6: viewCustomerOrdersAction(); break;
            case 7: viewCustomerTicketsAction(); break;
            default: cout << "Invalid option. Please choose 1-8.\n"; break;
        }
        ConsoleUtils::pause();
    }
}

void MenuUI::addCustomerAction() {
    ConsoleUtils::printHeader("ADD NEW CUSTOMER");
    string name = ConsoleUtils::readLine("Enter full name: ");
    string email = ConsoleUtils::readLine("Enter email: ");
    string phone = ConsoleUtils::readLine("Enter phone: ");
    string address = ConsoleUtils::readLine("Enter address: ");

    try {
        Customer c = customerService.registerCustomer(name, email, phone, address);
        cout << "\n[SUCCESS] Customer created successfully with ID #" << c.getId() << "!\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::updateCustomerAction() {
    ConsoleUtils::printHeader("UPDATE CUSTOMER DETAILS");
    int id = ConsoleUtils::readInt("Enter Customer ID to update: ");
    auto existing = customerService.getCustomerById(id);
    if (!existing.has_value()) {
        cout << "[ERROR] Customer ID #" << id << " not found.\n";
        return;
    }

    cout << "\nExisting details:\n";
    existing->displayDetails();
    cout << "\nEnter updated values (leave blank to keep existing):\n";

    string name = ConsoleUtils::readLine("New Name [" + existing->getName() + "]: ");
    if (name.empty()) name = existing->getName();

    string email = ConsoleUtils::readLine("New Email [" + existing->getEmail() + "]: ");
    if (email.empty()) email = existing->getEmail();

    string phone = ConsoleUtils::readLine("New Phone [" + existing->getPhone() + "]: ");
    if (phone.empty()) phone = existing->getPhone();

    string address = ConsoleUtils::readLine("New Address [" + existing->getAddress() + "]: ");
    if (address.empty()) address = existing->getAddress();

    try {
        if (customerService.updateCustomer(id, name, email, phone, address)) {
            cout << "\n[SUCCESS] Customer #" << id << " updated successfully!\n";
        } else {
            cout << "\n[ERROR] Failed to update customer record.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::deleteCustomerAction() {
    ConsoleUtils::printHeader("DELETE CUSTOMER");
    int id = ConsoleUtils::readInt("Enter Customer ID to delete: ");

    try {
        if (customerService.deleteCustomer(id)) {
            cout << "\n[SUCCESS] Customer #" << id << " deleted successfully.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::searchCustomerAction() {
    ConsoleUtils::printHeader("SEARCH CUSTOMERS");
    string term = ConsoleUtils::readLine("Enter search keyword (ID, Name, or Email): ");
    auto results = customerService.searchCustomers(term);

    if (results.empty()) {
        cout << "No matching customers found.\n";
        return;
    }

    cout << "\nFound " << results.size() << " match(es):\n";
    for (const auto& c : results) {
        cout << "----------------------------------------\n";
        c.displayDetails();
    }
}

void MenuUI::displayCustomersAction() {
    ConsoleUtils::printHeader("ALL CUSTOMERS");
    auto list = customerService.getAllCustomers();
    if (list.empty()) {
        cout << "No customer records found.\n";
        return;
    }

    cout << left << setw(6) << "ID"
              << setw(20) << "Name"
              << setw(28) << "Email"
              << setw(16) << "Phone"
              << "Address\n"
              << "--------------------------------------------------------------------------------\n";

    for (const auto& c : list) {
        cout << left << setw(6) << c.getId()
                  << setw(20) << c.getName()
                  << setw(28) << c.getEmail()
                  << setw(16) << c.getPhone()
                  << c.getAddress() << "\n";
    }
}

void MenuUI::viewCustomerOrdersAction() {
    ConsoleUtils::printHeader("CUSTOMER ORDER HISTORY");
    int id = ConsoleUtils::readInt("Enter Customer ID: ");
    auto orders = orderService.getCustomerOrders(id);

    if (orders.empty()) {
        cout << "No orders found for Customer #" << id << ".\n";
        return;
    }

    cout << "\nFound " << orders.size() << " order(s):\n";
    for (const auto& ord : orders) {
        auto fullOrder = orderService.getOrderDetails(ord.getId());
        if (fullOrder.has_value()) {
            cout << "----------------------------------------\n";
            fullOrder->displayOrderSummary();
        }
    }
}

void MenuUI::viewCustomerTicketsAction() {
    ConsoleUtils::printHeader("CUSTOMER SUPPORT TICKETS");
    int id = ConsoleUtils::readInt("Enter Customer ID: ");
    auto tickets = ticketManager.getCustomerTickets(id);

    if (tickets.empty()) {
        cout << "No support tickets found for Customer #" << id << ".\n";
        return;
    }

    cout << "\nFound " << tickets.size() << " ticket(s):\n";
    for (const auto& t : tickets) {
        cout << "----------------------------------------\n";
        t.displayDetails();
    }
}

// ============================================================================
// MODULE 2: EMPLOYEE MANAGEMENT
// ============================================================================

void MenuUI::handleEmployeeMenu() {
    while (true) {
        ConsoleUtils::printHeader("EMPLOYEE MANAGEMENT");
        cout << "1. Add New Employee\n"
                  << "2. Update Employee Details\n"
                  << "3. Delete Employee\n"
                  << "4. Search Employees\n"
                  << "5. Display All Employees\n"
                  << "6. Assign Employee to Support Ticket\n"
                  << "7. View Employee Ticket Workload\n"
                  << "8. Back to Main Menu\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 8) break;

        switch (choice) {
            case 1: addEmployeeAction(); break;
            case 2: updateEmployeeAction(); break;
            case 3: deleteEmployeeAction(); break;
            case 4: searchEmployeeAction(); break;
            case 5: displayEmployeesAction(); break;
            case 6: assignEmployeeToTicketAction(); break;
            case 7: viewEmployeeWorkloadAction(); break;
            default: cout << "Invalid option. Please choose 1-8.\n"; break;
        }
        ConsoleUtils::pause();
    }
}

void MenuUI::addEmployeeAction() {
    ConsoleUtils::printHeader("ADD NEW EMPLOYEE");
    string name = ConsoleUtils::readLine("Enter employee name: ");
    string email = ConsoleUtils::readLine("Enter internal email: ");
    string phone = ConsoleUtils::readLine("Enter phone: ");
    string role = ConsoleUtils::readLine("Enter role (e.g. Support Specialist, Tech Lead): ");
    string joining = ConsoleUtils::readLine("Enter joining date (YYYY-MM-DD or leave empty for today): ");

    try {
        Employee emp = employeeService.registerEmployee(name, email, phone, role, joining);
        cout << "\n[SUCCESS] Employee registered with ID #" << emp.getId() << "!\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::updateEmployeeAction() {
    ConsoleUtils::printHeader("UPDATE EMPLOYEE DETAILS");
    int id = ConsoleUtils::readInt("Enter Employee ID: ");
    auto existing = employeeService.getEmployeeById(id);
    if (!existing.has_value()) {
        cout << "[ERROR] Employee ID #" << id << " not found.\n";
        return;
    }

    cout << "\nExisting details:\n";
    existing->displayDetails();

    string name = ConsoleUtils::readLine("New Name [" + existing->getName() + "]: ");
    if (name.empty()) name = existing->getName();

    string email = ConsoleUtils::readLine("New Email [" + existing->getEmail() + "]: ");
    if (email.empty()) email = existing->getEmail();

    string phone = ConsoleUtils::readLine("New Phone [" + existing->getPhone() + "]: ");
    if (phone.empty()) phone = existing->getPhone();

    string role = ConsoleUtils::readLine("New Role [" + existing->getEmployeeRole() + "]: ");
    if (role.empty()) role = existing->getEmployeeRole();

    string joining = ConsoleUtils::readLine("New Joining Date [" + existing->getJoiningDate() + "]: ");
    if (joining.empty()) joining = existing->getJoiningDate();

    try {
        if (employeeService.updateEmployee(id, name, email, phone, role, joining)) {
            cout << "\n[SUCCESS] Employee #" << id << " updated successfully!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::deleteEmployeeAction() {
    ConsoleUtils::printHeader("DELETE EMPLOYEE");
    int id = ConsoleUtils::readInt("Enter Employee ID: ");

    try {
        if (employeeService.deleteEmployee(id)) {
            cout << "\n[SUCCESS] Employee #" << id << " deleted successfully.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::searchEmployeeAction() {
    ConsoleUtils::printHeader("SEARCH EMPLOYEES");
    string query = ConsoleUtils::readLine("Enter search keyword: ");
    auto results = employeeService.searchEmployees(query);

    if (results.empty()) {
        cout << "No employees found matching query.\n";
        return;
    }

    cout << "\nFound " << results.size() << " match(es):\n";
    for (const auto& emp : results) {
        cout << "----------------------------------------\n";
        emp.displayDetails();
    }
}

void MenuUI::displayEmployeesAction() {
    ConsoleUtils::printHeader("ALL EMPLOYEES");
    auto list = employeeService.getAllEmployees();
    if (list.empty()) {
        cout << "No employee records found.\n";
        return;
    }

    cout << left << setw(6) << "ID"
              << setw(20) << "Name"
              << setw(32) << "Email"
              << setw(24) << "Role"
              << "Joining Date\n"
              << "-----------------------------------------------------------------------------------------\n";

    for (const auto& emp : list) {
        cout << left << setw(6) << emp.getId()
                  << setw(20) << emp.getName()
                  << setw(32) << emp.getEmail()
                  << setw(24) << emp.getEmployeeRole()
                  << emp.getJoiningDate() << "\n";
    }
}

void MenuUI::assignEmployeeToTicketAction() {
    ConsoleUtils::printHeader("ASSIGN EMPLOYEE TO TICKET");
    int ticketId = ConsoleUtils::readInt("Enter Ticket ID: ");
    int empId = ConsoleUtils::readInt("Enter Employee ID to assign: ");

    try {
        if (ticketManager.assignTicket(ticketId, empId)) {
            cout << "\n[SUCCESS] Ticket #" << ticketId << " assigned to Employee #" << empId << "!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::viewEmployeeWorkloadAction() {
    ConsoleUtils::printHeader("EMPLOYEE TICKET WORKLOAD");
    analyticsService.displayEmployeeWorkload();
}

// ============================================================================
// MODULE 3: PRODUCT MANAGEMENT
// ============================================================================

void MenuUI::handleProductMenu() {
    while (true) {
        ConsoleUtils::printHeader("PRODUCT MANAGEMENT");
        cout << "1. Add New Product\n"
                  << "2. Update Product Details\n"
                  << "3. Update Product Price\n"
                  << "4. Update Inventory / Stock Quantity\n"
                  << "5. Delete Product\n"
                  << "6. Search Products\n"
                  << "7. Display All Products\n"
                  << "8. Display Low-Stock Products\n"
                  << "9. Back to Main Menu\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 9) break;

        switch (choice) {
            case 1: addProductAction(); break;
            case 2: updateProductAction(); break;
            case 3: updatePriceAction(); break;
            case 4: updateStockAction(); break;
            case 5: deleteProductAction(); break;
            case 6: searchProductAction(); break;
            case 7: displayProductsAction(); break;
            case 8: displayLowStockAction(); break;
            default: cout << "Invalid option. Please choose 1-9.\n"; break;
        }
        ConsoleUtils::pause();
    }
}

void MenuUI::addProductAction() {
    ConsoleUtils::printHeader("ADD NEW PRODUCT");
    int catId = ConsoleUtils::readInt("Enter Category ID (1=Software, 2=Hardware, 3=Networking, 4=Office): ");
    string name = ConsoleUtils::readLine("Enter product name: ");
    double price = ConsoleUtils::readDouble("Enter unit price ($): ");
    int stock = ConsoleUtils::readInt("Enter initial stock quantity: ");
    string desc = ConsoleUtils::readLine("Enter product description: ");

    try {
        Product p = productService.addProduct(catId, name, price, stock, desc);
        cout << "\n[SUCCESS] Product registered with ID #" << p.getId() << "!\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::updateProductAction() {
    ConsoleUtils::printHeader("UPDATE PRODUCT DETAILS");
    int id = ConsoleUtils::readInt("Enter Product ID: ");
    auto existing = productService.getProductById(id);
    if (!existing.has_value()) {
        cout << "[ERROR] Product ID #" << id << " not found.\n";
        return;
    }

    cout << "\nExisting details:\n";
    existing->displayDetails();

    int catId = ConsoleUtils::readInt("New Category ID [" + to_string(existing->getCategoryId()) + "]: ", existing->getCategoryId());
    string name = ConsoleUtils::readLine("New Name [" + existing->getName() + "]: ");
    if (name.empty()) name = existing->getName();

    double price = ConsoleUtils::readDouble("New Price [$" + to_string(existing->getPrice()) + "]: ", existing->getPrice());
    int stock = ConsoleUtils::readInt("New Stock [" + to_string(existing->getStockQuantity()) + "]: ", existing->getStockQuantity());
    string desc = ConsoleUtils::readLine("New Description: ");
    if (desc.empty()) desc = existing->getDescription();

    try {
        if (productService.updateProductDetails(id, catId, name, price, stock, desc)) {
            cout << "\n[SUCCESS] Product #" << id << " updated successfully!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::updatePriceAction() {
    ConsoleUtils::printHeader("UPDATE PRODUCT PRICE");
    int id = ConsoleUtils::readInt("Enter Product ID: ");
    double price = ConsoleUtils::readDouble("Enter new price ($): ");

    try {
        if (productService.updatePrice(id, price)) {
            cout << "\n[SUCCESS] Price updated for Product #" << id << "!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::updateStockAction() {
    ConsoleUtils::printHeader("UPDATE PRODUCT INVENTORY");
    int id = ConsoleUtils::readInt("Enter Product ID: ");
    int stock = ConsoleUtils::readInt("Enter new available stock quantity: ");

    try {
        if (productService.updateInventory(id, stock)) {
            cout << "\n[SUCCESS] Inventory stock updated for Product #" << id << "!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::deleteProductAction() {
    ConsoleUtils::printHeader("DELETE PRODUCT");
    int id = ConsoleUtils::readInt("Enter Product ID: ");

    try {
        if (productService.deleteProduct(id)) {
            cout << "\n[SUCCESS] Product #" << id << " deleted successfully.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::searchProductAction() {
    ConsoleUtils::printHeader("SEARCH PRODUCTS");
    string query = ConsoleUtils::readLine("Enter search keyword: ");
    auto results = productService.searchProducts(query);

    if (results.empty()) {
        cout << "No matching products found.\n";
        return;
    }

    cout << "\nFound " << results.size() << " product(s):\n";
    for (const auto& prod : results) {
        cout << "----------------------------------------\n";
        prod.displayDetails();
    }
}

void MenuUI::displayProductsAction() {
    ConsoleUtils::printHeader("ALL PRODUCTS");
    auto list = productService.getAllProducts();
    if (list.empty()) {
        cout << "No products available in catalogue.\n";
        return;
    }

    cout << left << setw(6) << "ID"
              << setw(34) << "Name"
              << setw(22) << "Category"
              << setw(12) << "Price"
              << "Stock Qty\n"
              << "--------------------------------------------------------------------------------------\n";

    for (const auto& p : list) {
        cout << left << setw(6) << p.getId()
                  << setw(34) << p.getName()
                  << setw(22) << p.getCategoryName()
                  << "$" << fixed << setprecision(2) << setw(11) << p.getPrice()
                  << p.getStockQuantity() << (p.isLowStock() ? " [LOW STOCK]" : "") << "\n";
    }
}

void MenuUI::displayLowStockAction() {
    ConsoleUtils::printHeader("LOW STOCK PRODUCTS");
    analyticsService.displayLowStockAlerts(10);
}

// ============================================================================
// MODULE 4: ORDER MANAGEMENT
// ============================================================================

void MenuUI::handleOrderMenu() {
    while (true) {
        ConsoleUtils::printHeader("ORDER MANAGEMENT");
        cout << "1. Place New Order (Multi-Item with ACID Transaction)\n"
                  << "2. View Order Details\n"
                  << "3. View Customer Order History\n"
                  << "4. Cancel Order (Rollback / Inventory Restoration)\n"
                  << "5. Display All Orders\n"
                  << "6. Back to Main Menu\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 6) break;

        switch (choice) {
            case 1: placeOrderAction(); break;
            case 2: viewOrderDetailsAction(); break;
            case 3: viewCustomerOrdersAction(); break;
            case 4: cancelOrderAction(); break;
            case 5: displayAllOrdersAction(); break;
            default: cout << "Invalid option. Please choose 1-6.\n"; break;
        }
        ConsoleUtils::pause();
    }
}

void MenuUI::placeOrderAction() {
    ConsoleUtils::printHeader("PLACE NEW ORDER (ACID TRANSACTION)");
    int customerId = ConsoleUtils::readInt("Enter Customer ID: ");

    vector<pair<int, int>> orderItems;
    while (true) {
        int prodId = ConsoleUtils::readInt("Enter Product ID to add (or 0 when done): ");
        if (prodId == 0) {
            if (orderItems.empty()) {
                cout << "Order must have at least one product. Please add an item.\n";
                continue;
            }
            break;
        }
        int qty = ConsoleUtils::readInt("Enter quantity: ");
        if (qty <= 0) {
            cout << "Quantity must be greater than zero.\n";
            continue;
        }
        orderItems.emplace_back(prodId, qty);
        cout << "Added Product #" << prodId << " (Qty: " << qty << ") to cart.\n";
    }

    cout << "\nSelect Payment Method:\n"
              << "1. CREDIT_CARD\n"
              << "2. DEBIT_CARD\n"
              << "3. UPI\n"
              << "4. BANK_TRANSFER\n"
              << "5. CASH\n";
    int payChoice = ConsoleUtils::readInt("Enter choice (1-5): ", 1);
    string method = "CREDIT_CARD";
    if (payChoice == 2) method = "DEBIT_CARD";
    else if (payChoice == 3) method = "UPI";
    else if (payChoice == 4) method = "BANK_TRANSFER";
    else if (payChoice == 5) method = "CASH";

    try {
        Order ord = orderService.placeOrder(customerId, orderItems, method);
        cout << "\n========================================\n"
                  << "       ORDER PLACED SUCCESSFULLY!       \n"
                  << "========================================\n";
        ord.displayOrderSummary();
        cout << "Payment Method: " << method << " (PAID)\n"
                  << "Inventory deducted automatically via database transaction.\n";
    } catch (const exception& e) {
        cout << "\n[TRANSACTION ROLLED BACK] " << e.what() << "\n"
                  << "No inventory or financial changes were committed.\n";
    }
}

void MenuUI::viewOrderDetailsAction() {
    ConsoleUtils::printHeader("VIEW ORDER DETAILS");
    int orderId = ConsoleUtils::readInt("Enter Order ID: ");
    auto order = orderService.getOrderDetails(orderId);

    if (!order.has_value()) {
        cout << "[ERROR] Order #" << orderId << " not found.\n";
        return;
    }

    order->displayOrderSummary();
    auto payments = orderService.getOrderPayments(orderId);
    if (!payments.empty()) {
        cout << "\nAssociated Payment History:\n";
        for (const auto& p : payments) {
            cout << "  ";
            p.display();
        }
    }
}

void MenuUI::cancelOrderAction() {
    ConsoleUtils::printHeader("CANCEL ORDER");
    int orderId = ConsoleUtils::readInt("Enter Order ID to cancel: ");

    try {
        if (orderService.cancelOrder(orderId)) {
            cout << "\n[SUCCESS] Order #" << orderId << " has been cancelled.\n"
                      << "All reserved inventory has been restored to catalogue.\n"
                      << "Associated payments marked as REFUNDED.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::displayAllOrdersAction() {
    ConsoleUtils::printHeader("ALL ORDERS");
    auto orders = orderService.getAllOrders();

    if (orders.empty()) {
        cout << "No orders found.\n";
        return;
    }

    cout << left << setw(8) << "Order ID"
              << setw(20) << "Customer"
              << setw(22) << "Date"
              << setw(14) << "Status"
              << "Total Amount\n"
              << "--------------------------------------------------------------------------------\n";

    for (const auto& ord : orders) {
        cout << left << setw(8) << ("#" + to_string(ord.getId()))
                  << setw(20) << ord.getCustomerName()
                  << setw(22) << ord.getOrderDate()
                  << setw(14) << ord.getStatus()
                  << "$" << fixed << setprecision(2) << ord.getTotalAmount() << "\n";
    }
}

// ============================================================================
// MODULE 5: SUPPORT TICKET MANAGEMENT (DSA QUEUES)
// ============================================================================

void MenuUI::handleTicketMenu() {
    while (true) {
        ConsoleUtils::printHeader("SUPPORT TICKET MANAGEMENT (QUEUE & PRIORITY QUEUE)");
        auto stats = ticketManager.getQueueStatistics();
        cout << "[In-Memory Queues] Urgent Priority Queue: " << stats.urgentQueueSize
                  << " | Normal FIFO Queue: " << stats.normalQueueSize
                  << " | Total Pending: " << stats.totalPending << "\n"
                  << "--------------------------------------------------------\n"
                  << "1. Create Support Ticket\n"
                  << "2. Process Next Ticket (Urgent Priority first, then FIFO Normal)\n"
                  << "3. View In-Memory Queue Status\n"
                  << "4. Assign Ticket to Employee\n"
                  << "5. Resolve Ticket\n"
                  << "6. View Ticket Details & Audit History\n"
                  << "7. Display All Tickets\n"
                  << "8. Back to Main Menu\n"
                  << "--------------------------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 8) break;

        switch (choice) {
            case 1: createTicketAction(); break;
            case 2: processNextTicketAction(); break;
            case 3: viewQueueStatusAction(); break;
            case 4: assignTicketAction(); break;
            case 5: resolveTicketAction(); break;
            case 6: viewTicketHistoryAction(); break;
            case 7: displayAllTicketsAction(); break;
            default: cout << "Invalid option. Please choose 1-8.\n"; break;
        }
        ConsoleUtils::pause();
    }
}

void MenuUI::createTicketAction() {
    ConsoleUtils::printHeader("CREATE SUPPORT TICKET");
    int customerId = ConsoleUtils::readInt("Enter Customer ID: ");
    string subject = ConsoleUtils::readLine("Enter Ticket Subject: ");
    string desc = ConsoleUtils::readLine("Enter Detailed Description: ");

    cout << "Select Priority Tier:\n"
              << "1. CRITICAL (Urgent - Highest Priority Queue)\n"
              << "2. HIGH     (Urgent - Priority Queue)\n"
              << "3. NORMAL   (Standard FIFO Queue)\n"
              << "4. LOW      (Standard FIFO Queue)\n";
    int pChoice = ConsoleUtils::readInt("Enter choice (1-4): ", 3);

    TicketPriority priority = TicketPriority::NORMAL;
    if (pChoice == 1) priority = TicketPriority::CRITICAL;
    else if (pChoice == 2) priority = TicketPriority::HIGH;
    else if (pChoice == 3) priority = TicketPriority::NORMAL;
    else if (pChoice == 4) priority = TicketPriority::LOW;

    try {
        SupportTicket ticket = ticketManager.createTicket(customerId, subject, desc, priority);
        cout << "\n[SUCCESS] Ticket #" << ticket.getId() << " created successfully!\n";
        if (ticket.isUrgent()) {
            cout << ">> Enqueued into priority_queue (Max-Heap by urgency & age tie-breaker).\n";
        } else {
            cout << ">> Enqueued into queue (First In, First Out).\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::processNextTicketAction() {
    ConsoleUtils::printHeader("PROCESS NEXT TICKET");
    int employeeId = ConsoleUtils::readInt("Enter Employee ID of handler: ");
    string notes = ConsoleUtils::readLine("Enter initial investigation notes: ");

    try {
        auto ticket = ticketManager.processNextTicket(employeeId, notes);
        if (!ticket.has_value()) {
            cout << "\nAll ticket queues are currently empty! No pending tickets.\n";
            return;
        }

        cout << "\n[SUCCESS] Next ticket popped and assigned for processing:\n"
                  << "----------------------------------------\n";
        ticket->displayDetails();
        cout << "----------------------------------------\n"
                  << "Status moved to IN_PROGRESS in database.\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::viewQueueStatusAction() {
    ConsoleUtils::printHeader("IN-MEMORY TICKET QUEUE DIAGNOSTICS");
    auto stats = ticketManager.getQueueStatistics();
    cout << "1. Urgent Priority Queue (priority_queue) : " << stats.urgentQueueSize << " ticket(s)\n"
              << "   - Ordering: CRITICAL > HIGH; older tickets first when equal priority.\n"
              << "2. Normal Standard Queue (queue)          : " << stats.normalQueueSize << " ticket(s)\n"
              << "   - Ordering: Strict FIFO (First-In, First-Out).\n"
              << "--------------------------------------------------------\n"
              << "Total In-Memory Pending Tickets                  : " << stats.totalPending << "\n";
}

void MenuUI::assignTicketAction() {
    ConsoleUtils::printHeader("ASSIGN TICKET");
    int ticketId = ConsoleUtils::readInt("Enter Ticket ID: ");
    int empId = ConsoleUtils::readInt("Enter Employee ID: ");

    try {
        if (ticketManager.assignTicket(ticketId, empId)) {
            cout << "\n[SUCCESS] Ticket #" << ticketId << " assigned to Employee #" << empId << "!\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::resolveTicketAction() {
    ConsoleUtils::printHeader("RESOLVE / CLOSE TICKET");
    int ticketId = ConsoleUtils::readInt("Enter Ticket ID to resolve: ");
    int empId = ConsoleUtils::readInt("Enter Employee ID resolving this ticket: ");
    string notes = ConsoleUtils::readLine("Enter resolution notes: ");

    try {
        if (ticketManager.resolveTicket(ticketId, empId, notes)) {
            cout << "\n[SUCCESS] Ticket #" << ticketId << " resolved and marked in database.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << "\n";
    }
}

void MenuUI::viewTicketHistoryAction() {
    ConsoleUtils::printHeader("TICKET DETAILS & AUDIT LOG");
    int ticketId = ConsoleUtils::readInt("Enter Ticket ID: ");
    auto ticket = ticketManager.getTicketById(ticketId);

    if (!ticket.has_value()) {
        cout << "[ERROR] Ticket #" << ticketId << " not found.\n";
        return;
    }

    ticket->displayDetails();
    auto history = ticketManager.getTicketHistory(ticketId);
    cout << "\nAudit Log & Updates History:\n";
    if (history.empty()) {
        cout << "  No historical updates logged yet.\n";
    } else {
        for (const auto& u : history) {
            u.display();
        }
    }
}

void MenuUI::displayAllTicketsAction() {
    ConsoleUtils::printHeader("ALL SUPPORT TICKETS");
    auto list = ticketManager.getAllTickets();
    if (list.empty()) {
        cout << "No support tickets found.\n";
        return;
    }

    cout << left << setw(6) << "ID"
              << setw(12) << "Priority"
              << setw(14) << "Status"
              << setw(20) << "Customer"
              << setw(20) << "Assigned To"
              << "Subject\n"
              << "------------------------------------------------------------------------------------------------\n";

    for (const auto& t : list) {
        cout << left << setw(6) << ("#" + to_string(t.getId()))
                  << setw(12) << priorityToString(t.getPriority())
                  << setw(14) << statusToString(t.getStatus())
                  << setw(20) << t.getCustomerName()
                  << setw(20) << (t.getAssignedEmployeeId() == 0 ? "Unassigned" : t.getEmployeeName())
                  << t.getSubject() << "\n";
    }
}

// ============================================================================
// MODULE 6: SALES ANALYTICS
// ============================================================================

void MenuUI::handleAnalyticsMenu() {
    while (true) {
        ConsoleUtils::printHeader("SALES & OPERATIONS ANALYTICS");
        cout << "1. Executive Revenue Overview & AOV\n"
                  << "2. Monthly Revenue Trend Analysis\n"
                  << "3. Top 5 High-Value Customers (VIPs)\n"
                  << "4. Best-Selling Products by Revenue\n"
                  << "5. Employee Ticket Workload Distribution\n"
                  << "6. Support Ticket Matrix (Status & Priority)\n"
                  << "7. Critical Low-Stock Inventory Alerts\n"
                  << "8. Complete Executive Dashboard (All Reports)\n"
                  << "9. Back to Main Menu\n"
                  << "----------------------------------------\n";

        int choice = ConsoleUtils::readInt("Enter your choice: ");
        if (choice == 9) break;

        switch (choice) {
            case 1: analyticsService.displayOverallRevenue(); break;
            case 2: analyticsService.displayMonthlyRevenueTrend(); break;
            case 3: analyticsService.displayTopCustomers(5); break;
            case 4: analyticsService.displayBestSellingProducts(10); break;
            case 5: analyticsService.displayEmployeeWorkload(); break;
            case 6: analyticsService.displayTicketStatusSummary(); break;
            case 7: analyticsService.displayLowStockAlerts(10); break;
            case 8: analyticsService.displayExecutiveDashboard(); break;
            default: cout << "Invalid option. Please choose 1-9.\n"; break;
        }
        ConsoleUtils::pause();
    }
}
