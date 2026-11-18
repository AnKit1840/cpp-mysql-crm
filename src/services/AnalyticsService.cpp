#include "../../include/services/AnalyticsService.hpp"

AnalyticsService::AnalyticsService(AnalyticsRepository repository) : repo(repository) {}

void AnalyticsService::displayOverallRevenue() {
    auto summary = repo.getOverallRevenueMetrics();
    std::cout << "\n==================================================\n"
              << "            EXECUTIVE REVENUE OVERVIEW            \n"
              << "==================================================\n"
              << "Total Completed Orders : " << summary.totalOrders << "\n"
              << "Gross Recognized Sales : $" << std::fixed << std::setprecision(2) << summary.totalRevenue << "\n"
              << "Average Order Value    : $" << summary.averageOrderValue << "\n"
              << "==================================================\n";
}

void AnalyticsService::displayMonthlyRevenueTrend() {
    auto list = repo.getMonthlyRevenueTrend();
    std::cout << "\n------------------------------------------------------------\n"
              << "                 MONTHLY SALES TREND                        \n"
              << "------------------------------------------------------------\n"
              << std::left << std::setw(12) << "Month"
              << std::setw(14) << "Orders Count"
              << std::setw(18) << "Monthly Revenue"
              << std::setw(16) << "Avg Order Value" << "\n"
              << "------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "No completed orders found.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(12) << item.month
                      << std::setw(14) << item.ordersCount
                      << "$" << std::fixed << std::setprecision(2) << std::setw(17) << item.revenue
                      << "$" << std::setw(15) << item.avgOrderValue << "\n";
        }
    }
    std::cout << "------------------------------------------------------------\n";
}

void AnalyticsService::displayTopCustomers(int limit) {
    auto list = repo.getTopCustomers(limit);
    std::cout << "\n--------------------------------------------------------------------------------\n"
              << "                   TOP " << limit << " HIGH-VALUE CUSTOMERS (VIP ACCOUNTS)              \n"
              << "--------------------------------------------------------------------------------\n"
              << std::left << std::setw(6) << "ID"
              << std::setw(20) << "Customer Name"
              << std::setw(30) << "Email"
              << std::setw(10) << "Orders"
              << std::setw(14) << "Total Spent" << "\n"
              << "--------------------------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "No purchase data available.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(6) << item.customerId
                      << std::setw(20) << item.name
                      << std::setw(30) << item.email
                      << std::setw(10) << item.completedOrders
                      << "$" << std::fixed << std::setprecision(2) << item.totalSpent << "\n";
        }
    }
    std::cout << "--------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayBestSellingProducts(int limit) {
    auto list = repo.getBestSellingProducts(limit);
    std::cout << "\n--------------------------------------------------------------------------------------\n"
              << "                      TOP BEST-SELLING PRODUCTS (BY REVENUE)                          \n"
              << "--------------------------------------------------------------------------------------\n"
              << std::left << std::setw(6) << "ID"
              << std::setw(34) << "Product Name"
              << std::setw(22) << "Category"
              << std::setw(12) << "Units Sold"
              << std::setw(14) << "Gross Revenue" << "\n"
              << "--------------------------------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "No product sales records found.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(6) << item.productId
                      << std::setw(34) << item.productName
                      << std::setw(22) << item.categoryName
                      << std::setw(12) << item.unitsSold
                      << "$" << std::fixed << std::setprecision(2) << item.totalRevenue << "\n";
        }
    }
    std::cout << "--------------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayEmployeeWorkload() {
    auto list = repo.getEmployeeWorkloads();
    std::cout << "\n--------------------------------------------------------------------------------------\n"
              << "                       EMPLOYEE SUPPORT TICKET WORKLOAD                               \n"
              << "--------------------------------------------------------------------------------------\n"
              << std::left << std::setw(6) << "ID"
              << std::setw(20) << "Employee Name"
              << std::setw(28) << "Role"
              << std::setw(16) << "Active Tickets"
              << std::setw(16) << "Resolved"
              << std::setw(14) << "Total Workload" << "\n"
              << "--------------------------------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "No employee data available.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(6) << item.employeeId
                      << std::setw(20) << item.name
                      << std::setw(28) << item.role
                      << std::setw(16) << item.activeTickets
                      << std::setw(16) << item.resolvedTickets
                      << std::setw(14) << item.totalAssigned << "\n";
        }
    }
    std::cout << "--------------------------------------------------------------------------------------\n";
}

void AnalyticsService::displayTicketStatusSummary() {
    auto list = repo.getTicketStatusBreakdown();
    std::cout << "\n------------------------------------------------------------\n"
              << "             SUPPORT TICKET MATRIX (STATUS & PRIORITY)      \n"
              << "------------------------------------------------------------\n"
              << std::left << std::setw(18) << "Priority"
              << std::setw(18) << "Status"
              << std::setw(10) << "Count" << "\n"
              << "------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "No support tickets recorded.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(18) << item.priority
                      << std::setw(18) << item.status
                      << std::setw(10) << item.count << "\n";
        }
    }
    std::cout << "------------------------------------------------------------\n";
}

void AnalyticsService::displayLowStockAlerts(int threshold) {
    auto list = repo.getLowStockAlerts(threshold);
    std::cout << "\n--------------------------------------------------------------------------------\n"
              << "             CRITICAL INVENTORY ALERT (STOCK <= " << threshold << " UNITS)              \n"
              << "--------------------------------------------------------------------------------\n"
              << std::left << std::setw(6) << "ID"
              << std::setw(34) << "Product Name"
              << std::setw(22) << "Category"
              << std::setw(12) << "Stock Qty"
              << std::setw(10) << "Price" << "\n"
              << "--------------------------------------------------------------------------------\n";

    if (list.empty()) {
        std::cout << "All inventory levels are healthy above threshold.\n";
    } else {
        for (const auto& item : list) {
            std::cout << std::left << std::setw(6) << item.productId
                      << std::setw(34) << item.name
                      << std::setw(22) << item.categoryName
                      << std::setw(12) << item.stockQuantity
                      << "$" << std::fixed << std::setprecision(2) << item.price << "\n";
        }
    }
    std::cout << "--------------------------------------------------------------------------------\n";
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
