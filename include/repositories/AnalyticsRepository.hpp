#ifndef ANALYTICS_REPOSITORY_HPP
#define ANALYTICS_REPOSITORY_HPP

#include <vector>
#include <string>
#include "../database/DatabaseManager.hpp"

using namespace std;

struct RevenueSummary {
    int totalOrders = 0;
    double totalRevenue = 0.0;
    double averageOrderValue = 0.0;
};

struct MonthlyRevenue {
    string month;
    int ordersCount = 0;
    double revenue = 0.0;
    double avgOrderValue = 0.0;
};

struct CustomerSpending {
    int customerId = 0;
    string name;
    string email;
    int completedOrders = 0;
    double totalSpent = 0.0;
};

struct ProductSalesStat {
    int productId = 0;
    string productName;
    string categoryName;
    int unitsSold = 0;
    double totalRevenue = 0.0;
};

struct EmployeeWorkload {
    int employeeId = 0;
    string name;
    string role;
    int totalAssigned = 0;
    int activeTickets = 0;
    int resolvedTickets = 0;
};

struct TicketStatusCount {
    string status;
    string priority;
    int count = 0;
};

struct LowStockItem {
    int productId = 0;
    string name;
    string categoryName;
    int stockQuantity = 0;
    double price = 0.0;
};

class AnalyticsRepository {
private:
    DatabaseManager& db;

public:
    explicit AnalyticsRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    RevenueSummary getOverallRevenueMetrics();
    vector<MonthlyRevenue> getMonthlyRevenueTrend();
    vector<CustomerSpending> getTopCustomers(int limit = 5);
    vector<ProductSalesStat> getBestSellingProducts(int limit = 10);
    vector<EmployeeWorkload> getEmployeeWorkloads();
    vector<TicketStatusCount> getTicketStatusBreakdown();
    vector<LowStockItem> getLowStockAlerts(int threshold = 10);
};

#endif // ANALYTICS_REPOSITORY_HPP
