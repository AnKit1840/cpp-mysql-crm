#ifndef MENU_UI_HPP
#define MENU_UI_HPP

#include "../services/CustomerService.hpp"
#include "../services/EmployeeService.hpp"
#include "../services/ProductService.hpp"
#include "../services/OrderService.hpp"
#include "../services/TicketManager.hpp"
#include "../services/AnalyticsService.hpp"

class MenuUI {
private:
    CustomerService customerService;
    EmployeeService employeeService;
    ProductService productService;
    OrderService orderService;
    TicketManager ticketManager;
    AnalyticsService analyticsService;

    // Submenu Handlers
    void handleCustomerMenu();
    void handleEmployeeMenu();
    void handleProductMenu();
    void handleOrderMenu();
    void handleTicketMenu();
    void handleAnalyticsMenu();

    // Specific Action Helpers
    void addCustomerAction();
    void updateCustomerAction();
    void deleteCustomerAction();
    void searchCustomerAction();
    void displayCustomersAction();
    void viewCustomerOrdersAction();
    void viewCustomerTicketsAction();

    void addEmployeeAction();
    void updateEmployeeAction();
    void deleteEmployeeAction();
    void searchEmployeeAction();
    void displayEmployeesAction();
    void assignEmployeeToTicketAction();
    void viewEmployeeWorkloadAction();

    void addProductAction();
    void updateProductAction();
    void updatePriceAction();
    void updateStockAction();
    void deleteProductAction();
    void searchProductAction();
    void displayProductsAction();
    void displayLowStockAction();

    void placeOrderAction();
    void viewOrderDetailsAction();
    void cancelOrderAction();
    void displayAllOrdersAction();

    void createTicketAction();
    void processNextTicketAction();
    void viewQueueStatusAction();
    void assignTicketAction();
    void resolveTicketAction();
    void viewTicketHistoryAction();
    void displayAllTicketsAction();

public:
    MenuUI();
    void run();
};

#endif // MENU_UI_HPP
