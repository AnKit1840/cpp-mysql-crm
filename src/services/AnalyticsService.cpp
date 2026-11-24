#include "../../include/services/AnalyticsService.hpp"

using namespace std;

AnalyticsService::AnalyticsService(AnalyticsRepository repository) : repo(repository) {}

void AnalyticsService::displayOverallRevenue() {
    auto summary = repo.getOverallRevenueMetrics();
    cout << "\n==================================================\n"
              << "            EXECUTIVE REVENUE OVERVIEW            \n"
              << "==================================================\n"
              << "Total Completed Orders : " << summary.totalOrders << "\n"
              << "Gross Recognized Sales : Rs. " << fixed << setprecision(2) << summary.totalRevenue << "\n"
              << "Average Order Value    : Rs. " << summary.averageOrderValue << "\n"
              << "==================================================\n";
}

void AnalyticsService::displayMonthlyRevenueTrend() {
    auto list = repo.getMonthlyRevenueTrend();
    cout << "\n------------------------------------------------------------\n"
              << "                 MONTHLY SALES TREND                        \n"
              << "------------------------------------------------------------\n"
              << left << setw(12) << "Month"
              << setw(14) << "Orders Count"
              << setw(18) << "Monthly Revenue"
              << setw(16) << "Avg Order Value" << "\n"
              << "------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "No completed orders found.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(12) << item.month
                      << setw(14) << item.ordersCount
                      << "Rs. " << fixed << setprecision(2) << setw(17) << item.revenue
                      << "Rs. " << setw(15) << item.avgOrderValue << "\n";
        }
    }
    cout << "------------------------------------------------------------\n";
}

void AnalyticsService::displayTopCustomers(int limit) {
    auto list = repo.getTopCustomers(limit);
    cout << "\n--------------------------------------------------------------------------------\n"
              << "                   TOP " << limit << " HIGH-VALUE CUSTOMERS (VIP ACCOUNTS)              \n"
              << "--------------------------------------------------------------------------------\n"
              << left << setw(6) << "ID"
              << setw(20) << "Customer Name"
              << setw(30) << "Email"
              << setw(10) << "Orders"
              << setw(14) << "Total Spent" << "\n"
              << "--------------------------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "No purchase data available.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(6) << item.customerId
                      << setw(20) << item.name
                      << setw(30) << item.email
                      << setw(10) << item.completedOrders
                      << "Rs. " << fixed << setprecision(2) << item.totalSpent << "\n";
        }
    }
    cout << "--------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayBestSellingProducts(int limit) {
    auto list = repo.getBestSellingProducts(limit);
    cout << "\n--------------------------------------------------------------------------------------\n"
              << "                      TOP BEST-SELLING PRODUCTS (BY REVENUE)                          \n"
              << "--------------------------------------------------------------------------------------\n"
              << left << setw(6) << "ID"
              << setw(34) << "Product Name"
              << setw(22) << "Category"
              << setw(12) << "Units Sold"
              << setw(14) << "Gross Revenue" << "\n"
              << "--------------------------------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "No product sales records found.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(6) << item.productId
                      << setw(34) << item.productName
                      << setw(22) << item.categoryName
                      << setw(12) << item.unitsSold
                      << "Rs. " << fixed << setprecision(2) << item.totalRevenue << "\n";
        }
    }
    cout << "--------------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayEmployeeWorkload() {
    auto list = repo.getEmployeeWorkloads();
    cout << "\n--------------------------------------------------------------------------------------\n"
              << "                       EMPLOYEE SUPPORT TICKET WORKLOAD                               \n"
              << "--------------------------------------------------------------------------------------\n"
              << left << setw(6) << "ID"
              << setw(20) << "Employee Name"
              << setw(28) << "Role"
              << setw(16) << "Active Tickets"
              << setw(16) << "Resolved"
              << setw(14) << "Total Workload" << "\n"
              << "--------------------------------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "No employee data available.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(6) << item.employeeId
                      << setw(20) << item.name
                      << setw(28) << item.role
                      << setw(16) << item.activeTickets
                      << setw(16) << item.resolvedTickets
                      << setw(14) << item.totalAssigned << "\n";
        }
    }
    cout << "--------------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayTicketStatusSummary() {
    auto list = repo.getTicketStatusBreakdown();
    cout << "\n------------------------------------------------------------\n"
              << "             SUPPORT TICKET MATRIX (STATUS & PRIORITY)      \n"
              << "------------------------------------------------------------\n"
              << left << setw(18) << "Priority"
              << setw(18) << "Status"
              << setw(10) << "Count" << "\n"
              << "------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "No support tickets recorded.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(18) << item.priority
                      << setw(18) << item.status
                      << setw(10) << item.count << "\n";
        }
    }
    cout << "------------------------------------------------------------\n";
}

void AnalyticsService::displayLowStockAlerts(int threshold) {
    auto list = repo.getLowStockAlerts(threshold);
    cout << "\n--------------------------------------------------------------------------------\n"
              << "             CRITICAL INVENTORY ALERT (STOCK <= " << threshold << " UNITS)              \n"
              << "--------------------------------------------------------------------------------\n"
              << left << setw(6) << "ID"
              << setw(34) << "Product Name"
              << setw(22) << "Category"
              << setw(12) << "Stock Qty"
              << setw(10) << "Price" << "\n"
              << "--------------------------------------------------------------------------------\n";

    if (list.empty()) {
        cout << "All inventory levels are healthy above threshold.\n";
    } else {
        for (const auto& item : list) {
            cout << left << setw(6) << item.productId
                      << setw(34) << item.name
                      << setw(22) << item.categoryName
                      << setw(12) << item.stockQuantity
                      << "Rs. " << fixed << setprecision(2) << item.price << "\n";
        }
    }
    cout << "--------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayExecutiveDashboard() {
    displayOverallRevenue();
    displayMonthlyRevenueTrend();
    displayTopCustomers(5);
    displayBestSellingProducts(5);
    displayEmployeeWorkload();
    displayTicketStatusSummary();
    displayLowStockAlerts(10);
}
