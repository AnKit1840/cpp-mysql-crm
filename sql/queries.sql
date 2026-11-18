-- ============================================================================
-- CRM System — Essential SQL Queries (Placement Portfolio Reference)
-- Database: crm_db
-- Description: 15+ production-grade queries demonstrating joins, subqueries,
--              aggregations, grouping, and business analytics.
-- ============================================================================

USE crm_db;

-- ----------------------------------------------------------------------------
-- QUERY 1: Total Revenue Generated from Completed Orders
-- Concepts: Aggregate function SUM(), WHERE filtering on order status.
-- Purpose: Quick executive KPI metric showing overall recognized business revenue.
-- ----------------------------------------------------------------------------
SELECT 
    COUNT(order_id) AS total_completed_orders,
    COALESCE(SUM(total_amount), 0.00) AS gross_revenue
FROM orders
WHERE status = 'COMPLETED';


-- ----------------------------------------------------------------------------
-- QUERY 2: Monthly Revenue Trend Analysis
-- Concepts: DATE_FORMAT(), GROUP BY, ORDER BY, aggregate SUM().
-- Purpose: Visualizes month-over-month sales performance to detect seasonal growth.
-- ----------------------------------------------------------------------------
SELECT 
    DATE_FORMAT(order_date, '%Y-%m') AS sales_month,
    COUNT(order_id) AS orders_count,
    SUM(total_amount) AS monthly_revenue,
    ROUND(AVG(total_amount), 2) AS avg_order_value
FROM orders
WHERE status = 'COMPLETED'
GROUP BY DATE_FORMAT(order_date, '%Y-%m')
ORDER BY sales_month ASC;


-- ----------------------------------------------------------------------------
-- QUERY 3: Overall Sales Metrics (Total Orders & Average Order Value - AOV)
-- Concepts: Aggregate functions (COUNT, SUM, AVG), status filtering.
-- Purpose: Provides core e-commerce/CRM benchmark metrics.
-- ----------------------------------------------------------------------------
SELECT 
    COUNT(order_id) AS total_orders,
    COALESCE(SUM(total_amount), 0.00) AS total_revenue,
    COALESCE(ROUND(AVG(total_amount), 2), 0.00) AS average_order_value
FROM orders
WHERE status = 'COMPLETED';


-- ----------------------------------------------------------------------------
-- QUERY 4: Top 5 Highest-Spending Customers (VIP Identification)
-- Concepts: INNER JOIN (customers + orders), GROUP BY, SUM(), ORDER BY DESC, LIMIT.
-- Purpose: Identifies high-net-worth accounts for dedicated account management.
-- ----------------------------------------------------------------------------
SELECT 
    c.customer_id,
    c.name AS customer_name,
    c.email,
    c.phone,
    COUNT(o.order_id) AS completed_orders,
    SUM(o.total_amount) AS total_spent
FROM customers c
INNER JOIN orders o ON c.customer_id = o.customer_id
WHERE o.status = 'COMPLETED'
GROUP BY c.customer_id, c.name, c.email, c.phone
ORDER BY total_spent DESC
LIMIT 5;


-- ----------------------------------------------------------------------------
-- QUERY 5: Best-Selling Products by Units Sold and Revenue
-- Concepts: Multiple INNER JOINs (products + categories + order_items + orders).
-- Purpose: Determines which inventory generates the most revenue and highest demand.
-- ----------------------------------------------------------------------------
SELECT 
    p.product_id,
    p.name AS product_name,
    cat.name AS category_name,
    p.price AS current_unit_price,
    SUM(oi.quantity) AS total_units_sold,
    SUM(oi.subtotal) AS total_product_revenue
FROM products p
INNER JOIN categories cat ON p.category_id = cat.category_id
INNER JOIN order_items oi ON p.product_id = oi.product_id
INNER JOIN orders o ON oi.order_id = o.order_id
WHERE o.status = 'COMPLETED'
GROUP BY p.product_id, p.name, cat.name, p.price
ORDER BY total_product_revenue DESC;


-- ----------------------------------------------------------------------------
-- QUERY 6: Low-Stock Inventory Alert (Critical Reorder List)
-- Concepts: INNER JOIN, WHERE with comparison threshold, ORDER BY ASC.
-- Purpose: Alerts warehouse managers when stock is dangerously low (<= 10 units).
-- ----------------------------------------------------------------------------
SELECT 
    p.product_id,
    p.name AS product_name,
    cat.name AS category_name,
    p.stock_quantity,
    p.price
FROM products p
INNER JOIN categories cat ON p.category_id = cat.category_id
WHERE p.stock_quantity <= 10
ORDER BY p.stock_quantity ASC;


-- ----------------------------------------------------------------------------
-- QUERY 7: Detailed Customer Order & Line-Item History
-- Concepts: 4-Table JOIN (customers, orders, order_items, products), ORDER BY.
-- Purpose: Customer 360-degree view showing complete order breakdown and items.
-- ----------------------------------------------------------------------------
SELECT 
    c.name AS customer_name,
    o.order_id,
    o.order_date,
    o.status AS order_status,
    p.name AS product_name,
    oi.quantity,
    oi.unit_price,
    oi.subtotal
FROM customers c
INNER JOIN orders o ON c.customer_id = o.customer_id
INNER JOIN order_items oi ON o.order_id = oi.order_id
INNER JOIN products p ON oi.product_id = p.product_id
WHERE c.customer_id = 1
ORDER BY o.order_date DESC, oi.order_item_id ASC;


-- ----------------------------------------------------------------------------
-- QUERY 8: Employee Ticket Workload & Assignment Distribution
-- Concepts: LEFT JOIN (includes employees with 0 tickets), COUNT(t.ticket_id),
--           Conditional aggregation using SUM(CASE ... END).
-- Purpose: Evaluates support desk resource allocation and detects employee burnout.
-- ----------------------------------------------------------------------------
SELECT 
    e.employee_id,
    e.name AS employee_name,
    e.role,
    COUNT(t.ticket_id) AS total_assigned_tickets,
    SUM(CASE WHEN t.status IN ('OPEN', 'IN_PROGRESS') THEN 1 ELSE 0 END) AS active_tickets,
    SUM(CASE WHEN t.status IN ('RESOLVED', 'CLOSED') THEN 1 ELSE 0 END) AS resolved_tickets
FROM employees e
LEFT JOIN support_tickets t ON e.employee_id = t.assigned_employee_id
GROUP BY e.employee_id, e.name, e.role
ORDER BY active_tickets DESC, total_assigned_tickets DESC;


-- ----------------------------------------------------------------------------
-- QUERY 9: Support Ticket Summary by Status and Priority Matrix
-- Concepts: GROUP BY multiple columns, COUNT(), ORDER BY FIELD().
-- Purpose: Operational dashboard showing current ticket backlog and severity.
-- ----------------------------------------------------------------------------
SELECT 
    priority,
    status,
    COUNT(ticket_id) AS ticket_count
FROM support_tickets
GROUP BY priority, status
ORDER BY FIELD(priority, 'CRITICAL', 'HIGH', 'NORMAL', 'LOW'), status;


-- ----------------------------------------------------------------------------
-- QUERY 10: Category-Level Revenue Contribution
-- Concepts: INNER JOIN across 3 tables, aggregate SUM, percentage calculation.
-- Purpose: Identifies most lucrative business sectors (e.g. Hardware vs Software).
-- ----------------------------------------------------------------------------
SELECT 
    cat.category_id,
    cat.name AS category_name,
    COUNT(DISTINCT o.order_id) AS distinct_orders,
    SUM(oi.quantity) AS total_items_sold,
    SUM(oi.subtotal) AS category_revenue
FROM categories cat
INNER JOIN products p ON cat.category_id = p.category_id
INNER JOIN order_items oi ON p.product_id = oi.product_id
INNER JOIN orders o ON oi.order_id = o.order_id
WHERE o.status = 'COMPLETED'
GROUP BY cat.category_id, cat.name
ORDER BY category_revenue DESC;


-- ----------------------------------------------------------------------------
-- QUERY 11: Inactive Customers (Registered Leads with Zero Orders)
-- Concepts: LEFT JOIN where right key IS NULL (anti-join).
-- Purpose: Identifies dormant accounts for marketing re-engagement campaigns.
-- ----------------------------------------------------------------------------
SELECT 
    c.customer_id,
    c.name AS customer_name,
    c.email,
    c.phone,
    c.created_at AS registration_date
FROM customers c
LEFT JOIN orders o ON c.customer_id = o.customer_id
WHERE o.order_id IS NULL;


-- ----------------------------------------------------------------------------
-- QUERY 12: High-Value Customers Having Open Support Issues
-- Concepts: Subqueries with IN and EXISTS, correlating customer value with support risk.
-- Purpose: Prevents customer churn by prioritizing tickets from top spenders.
-- ----------------------------------------------------------------------------
SELECT 
    c.customer_id,
    c.name AS customer_name,
    c.email,
    st.ticket_id,
    st.priority,
    st.subject,
    st.status
FROM customers c
INNER JOIN support_tickets st ON c.customer_id = st.customer_id
WHERE st.status IN ('OPEN', 'IN_PROGRESS')
  AND c.customer_id IN (
      SELECT o.customer_id
      FROM orders o
      WHERE o.status = 'COMPLETED'
      GROUP BY o.customer_id
      HAVING SUM(o.total_amount) > 1000.00
  )
ORDER BY FIELD(st.priority, 'CRITICAL', 'HIGH', 'NORMAL', 'LOW');


-- ----------------------------------------------------------------------------
-- QUERY 13: Average Ticket Resolution Time (in Hours) by Priority
-- Concepts: TIMESTAMPDIFF(HOUR, ...), AVG(), WHERE resolved_at IS NOT NULL.
-- Purpose: Measures SLA compliance across Critical vs Normal severity tiers.
-- ----------------------------------------------------------------------------
SELECT 
    priority,
    COUNT(ticket_id) AS resolved_count,
    ROUND(AVG(TIMESTAMPDIFF(MINUTE, created_at, resolved_at) / 60.0), 2) AS avg_resolution_hours
FROM support_tickets
WHERE resolved_at IS NOT NULL
GROUP BY priority
ORDER BY avg_resolution_hours ASC;


-- ----------------------------------------------------------------------------
-- QUERY 14: Products with No Order History (Dead Stock Detection)
-- Concepts: Correlated Subquery with NOT EXISTS.
-- Purpose: Identifies unsold catalogue items for clearance or review.
-- ----------------------------------------------------------------------------
SELECT 
    p.product_id,
    p.name AS product_name,
    cat.name AS category_name,
    p.price,
    p.stock_quantity
FROM products p
INNER JOIN categories cat ON p.category_id = cat.category_id
WHERE NOT EXISTS (
    SELECT 1 
    FROM order_items oi 
    WHERE oi.product_id = p.product_id
);


-- ----------------------------------------------------------------------------
-- QUERY 15: Payment Method Distribution & Failure Rate Analysis
-- Concepts: GROUP BY, aggregate SUM(), conditional COUNT(), percentage.
-- Purpose: Evaluates payment gateway reliability (e.g. UPI vs Credit Card vs Wire).
-- ----------------------------------------------------------------------------
SELECT 
    payment_method,
    COUNT(payment_id) AS total_transactions,
    SUM(amount) AS total_volume,
    SUM(CASE WHEN status = 'PAID' THEN 1 ELSE 0 END) AS successful_payments,
    SUM(CASE WHEN status = 'REFUNDED' THEN 1 ELSE 0 END) AS refunded_payments,
    SUM(CASE WHEN status = 'FAILED' THEN 1 ELSE 0 END) AS failed_payments
FROM payments
GROUP BY payment_method
ORDER BY total_volume DESC;


-- ----------------------------------------------------------------------------
-- QUERY 16: Safe Customer Deletion Pre-Check (Referential Integrity Check)
-- Concepts: Subqueries in SELECT clause to count child records.
-- Purpose: Verifies whether a customer can be safely deleted without FK violations.
-- ----------------------------------------------------------------------------
SELECT 
    c.customer_id,
    c.name,
    (SELECT COUNT(*) FROM orders WHERE customer_id = c.customer_id) AS order_count,
    (SELECT COUNT(*) FROM support_tickets WHERE customer_id = c.customer_id) AS ticket_count,
    CASE 
        WHEN (SELECT COUNT(*) FROM orders WHERE customer_id = c.customer_id) > 0 
          OR (SELECT COUNT(*) FROM support_tickets WHERE customer_id = c.customer_id) > 0
        THEN 'CANNOT_DELETE_HAS_DEPENDENCIES'
        ELSE 'SAFE_TO_DELETE'
    END AS deletion_status
FROM customers c
WHERE c.customer_id = 1;
