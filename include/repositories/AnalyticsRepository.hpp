#ifndef ANALYTICS_REPOSITORY_HPP
#define ANALYTICS_REPOSITORY_HPP

#include <vector>
#include <string>
#include "../database/DatabaseManager.hpp"

struct RevenueSummary {
    int totalOrders = 0;
    double totalRevenue = 0.0;
    double averageOrderValue = 0.0;
};

struct MonthlyRevenue {
    std::string month;
    int ordersCount = 0;
    double revenue = 0.0;
    double avgOrderValue = 0.0;
};

struct CustomerSpending {
    int customerId = 0;
    std::string name;
    std::string email;
    int completedOrders = 0;
    double totalSpent = 0.0;
};

struct ProductSalesStat {
    int productId = 0;
    std::string productName;
    std::string categoryName;
    int unitsSold = 0;
    double totalRevenue = 0.0;
};

struct EmployeeWorkload {
    int employeeId = 0;
    std::string name;
    std::string role;
    int totalAssigned = 0;
    int activeTickets = 0;
    int resolvedTickets = 0;
};

struct TicketStatusCount {
    std::string status;
    std::string priority;
    int count = 0;
};

struct LowStockItem {
    int productId = 0;
    std::string name;
    std::string categoryName;
    int stockQuantity = 0;
    double price = 0.0;
};

class AnalyticsRepository {
private:
    DatabaseManager& db;

public:
    explicit AnalyticsRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    RevenueSummary getOverallRevenueMetrics();
    std::vector<MonthlyRevenue> getMonthlyRevenueTrend();
    std::vector<CustomerSpending> getTopCustomers(int limit = 5);
    std::vector<ProductSalesStat> getBestSellingProducts(int limit = 10);
    std::vector<EmployeeWorkload> getEmployeeWorkloads();
    std::vector<TicketStatusCount> getTicketStatusBreakdown();
    std::vector<LowStockItem> getLowStockAlerts(int threshold = 10);
};

#endif // ANALYTICS_REPOSITORY_HPP
