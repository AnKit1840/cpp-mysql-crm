#ifndef ANALYTICS_SERVICE_HPP
#define ANALYTICS_SERVICE_HPP

#include <iostream>
#include <iomanip>
#include "../repositories/AnalyticsRepository.hpp"

class AnalyticsService {
private:
    AnalyticsRepository repo;

public:
    explicit AnalyticsService(AnalyticsRepository repository = AnalyticsRepository());

    void displayOverallRevenue();
    void displayMonthlyRevenueTrend();
    void displayTopCustomers(int limit = 5);
    void displayBestSellingProducts(int limit = 10);
    void displayEmployeeWorkload();
    void displayTicketStatusSummary();
    void displayLowStockAlerts(int threshold = 10);

    // Displays all reports together in an executive dashboard
    void displayExecutiveDashboard();
};

#endif // ANALYTICS_SERVICE_HPP
