#include "../../include/repositories/AnalyticsRepository.hpp"
#include <sstream>

using namespace std;

AnalyticsRepository::AnalyticsRepository(DatabaseManager& dbManager) : db(dbManager) {}

RevenueSummary AnalyticsRepository::getOverallRevenueMetrics() {
    RevenueSummary summary;
    string sql = "SELECT COUNT(order_id), COALESCE(SUM(total_amount), 0.0), COALESCE(AVG(total_amount), 0.0) "
                      "FROM orders WHERE status = 'COMPLETED';";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return summary;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        summary.totalOrders = row[0] ? stoi(row[0]) : 0;
        summary.totalRevenue = row[1] ? stod(row[1]) : 0.0;
        summary.averageOrderValue = row[2] ? stod(row[2]) : 0.0;
    }
    mysql_free_result(res);
    return summary;
}

vector<MonthlyRevenue> AnalyticsRepository::getMonthlyRevenueTrend() {
    vector<MonthlyRevenue> list;
    string sql = "SELECT DATE_FORMAT(order_date, '%Y-%m') AS sales_month, "
                      "COUNT(order_id), SUM(total_amount), ROUND(AVG(total_amount), 2) "
                      "FROM orders WHERE status = 'COMPLETED' "
                      "GROUP BY DATE_FORMAT(order_date, '%Y-%m') "
                      "ORDER BY sales_month ASC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        MonthlyRevenue item;
        item.month = row[0] ? row[0] : "";
        item.ordersCount = row[1] ? stoi(row[1]) : 0;
        item.revenue = row[2] ? stod(row[2]) : 0.0;
        item.avgOrderValue = row[3] ? stod(row[3]) : 0.0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}

vector<CustomerSpending> AnalyticsRepository::getTopCustomers(int limit) {
    vector<CustomerSpending> list;
    stringstream ss;
    ss << "SELECT c.customer_id, c.name, c.email, COUNT(o.order_id), SUM(o.total_amount) "
       << "FROM customers c "
       << "INNER JOIN orders o ON c.customer_id = o.customer_id "
       << "WHERE o.status = 'COMPLETED' "
       << "GROUP BY c.customer_id, c.name, c.email "
       << "ORDER BY SUM(o.total_amount) DESC "
       << "LIMIT " << limit << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        CustomerSpending item;
        item.customerId = row[0] ? stoi(row[0]) : 0;
        item.name = row[1] ? row[1] : "";
        item.email = row[2] ? row[2] : "";
        item.completedOrders = row[3] ? stoi(row[3]) : 0;
        item.totalSpent = row[4] ? stod(row[4]) : 0.0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}

vector<ProductSalesStat> AnalyticsRepository::getBestSellingProducts(int limit) {
    vector<ProductSalesStat> list;
    stringstream ss;
    ss << "SELECT p.product_id, p.name, cat.name, SUM(oi.quantity), SUM(oi.subtotal) "
       << "FROM products p "
       << "INNER JOIN categories cat ON p.category_id = cat.category_id "
       << "INNER JOIN order_items oi ON p.product_id = oi.product_id "
       << "INNER JOIN orders o ON oi.order_id = o.order_id "
       << "WHERE o.status = 'COMPLETED' "
       << "GROUP BY p.product_id, p.name, cat.name "
       << "ORDER BY SUM(oi.subtotal) DESC "
       << "LIMIT " << limit << ";";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        ProductSalesStat item;
        item.productId = row[0] ? stoi(row[0]) : 0;
        item.productName = row[1] ? row[1] : "";
        item.categoryName = row[2] ? row[2] : "";
        item.unitsSold = row[3] ? stoi(row[3]) : 0;
        item.totalRevenue = row[4] ? stod(row[4]) : 0.0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}

vector<EmployeeWorkload> AnalyticsRepository::getEmployeeWorkloads() {
    vector<EmployeeWorkload> list;
    string sql = "SELECT e.employee_id, e.name, e.role, "
                      "COUNT(t.ticket_id) AS total_assigned, "
                      "SUM(CASE WHEN t.status IN ('OPEN', 'IN_PROGRESS') THEN 1 ELSE 0 END) AS active_tickets, "
                      "SUM(CASE WHEN t.status IN ('RESOLVED', 'CLOSED') THEN 1 ELSE 0 END) AS resolved_tickets "
                      "FROM employees e "
                      "LEFT JOIN support_tickets t ON e.employee_id = t.assigned_employee_id "
                      "GROUP BY e.employee_id, e.name, e.role "
                      "ORDER BY active_tickets DESC, total_assigned DESC;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        EmployeeWorkload item;
        item.employeeId = row[0] ? stoi(row[0]) : 0;
        item.name = row[1] ? row[1] : "";
        item.role = row[2] ? row[2] : "";
        item.totalAssigned = row[3] ? stoi(row[3]) : 0;
        item.activeTickets = row[4] ? stoi(row[4]) : 0;
        item.resolvedTickets = row[5] ? stoi(row[5]) : 0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}

vector<TicketStatusCount> AnalyticsRepository::getTicketStatusBreakdown() {
    vector<TicketStatusCount> list;
    string sql = "SELECT status, priority, COUNT(ticket_id) "
                      "FROM support_tickets "
                      "GROUP BY status, priority "
                      "ORDER BY FIELD(priority, 'CRITICAL', 'HIGH', 'NORMAL', 'LOW'), status;";

    MYSQL_RES* res = db.executeQuery(sql);
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        TicketStatusCount item;
        item.status = row[0] ? row[0] : "";
        item.priority = row[1] ? row[1] : "";
        item.count = row[2] ? stoi(row[2]) : 0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}

vector<LowStockItem> AnalyticsRepository::getLowStockAlerts(int threshold) {
    vector<LowStockItem> list;
    stringstream ss;
    ss << "SELECT p.product_id, p.name, cat.name, p.stock_quantity, p.price "
       << "FROM products p "
       << "INNER JOIN categories cat ON p.category_id = cat.category_id "
       << "WHERE p.stock_quantity <= " << threshold << " "
       << "ORDER BY p.stock_quantity ASC;";

    MYSQL_RES* res = db.executeQuery(ss.str());
    if (!res) return list;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        LowStockItem item;
        item.productId = row[0] ? stoi(row[0]) : 0;
        item.name = row[1] ? row[1] : "";
        item.categoryName = row[2] ? row[2] : "";
        item.stockQuantity = row[3] ? stoi(row[3]) : 0;
        item.price = row[4] ? stod(row[4]) : 0.0;
        list.push_back(item);
    }
    mysql_free_result(res);
    return list;
}
