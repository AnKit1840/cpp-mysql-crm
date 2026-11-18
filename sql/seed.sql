-- ============================================================================
-- CRM System Seed Data
-- Database: crm_db
-- Description: Realistic initial sample dataset for testing and placement demo
-- ============================================================================

USE crm_db;

-- Clean existing data in reverse dependency order
SET FOREIGN_KEY_CHECKS = 0;
TRUNCATE TABLE ticket_updates;
TRUNCATE TABLE support_tickets;
TRUNCATE TABLE payments;
TRUNCATE TABLE order_items;
TRUNCATE TABLE orders;
TRUNCATE TABLE products;
TRUNCATE TABLE categories;
TRUNCATE TABLE employees;
TRUNCATE TABLE customers;
SET FOREIGN_KEY_CHECKS = 1;

-- ----------------------------------------------------------------------------
-- 1. SEED CATEGORIES
-- ----------------------------------------------------------------------------
INSERT INTO categories (category_id, name, description) VALUES
(1, 'Enterprise Software', 'CRM, ERP, and operating software licenses'),
(2, 'Computer Hardware', 'Laptops, servers, workstations, and peripherals'),
(3, 'Networking Equipment', 'Routers, switches, patch cables, and access points'),
(4, 'Office Electronics', 'Monitors, printers, docks, and surge protectors');

-- ----------------------------------------------------------------------------
-- 2. SEED CUSTOMERS
-- ----------------------------------------------------------------------------
INSERT INTO customers (customer_id, name, email, phone, address, created_at) VALUES
(1, 'Alice Smith', 'alice.smith@acmecorp.com', '+1-555-0101', '100 Silicon Blvd, San Jose, CA', '2026-01-10 09:30:00'),
(2, 'Bob Johnson', 'bob.johnson@apextech.io', '+1-555-0102', '450 Innovation Way, Austin, TX', '2026-01-15 14:15:00'),
(3, 'Charlie Brown', 'charlie.b@nexusglobal.com', '+1-555-0103', '789 Market St, New York, NY', '2026-02-01 11:00:00'),
(4, 'Diana Prince', 'diana.p@themyscira.org', '+1-555-0104', '222 Harbor View, Seattle, WA', '2026-02-12 16:45:00'),
(5, 'Evan Wright', 'evan.wright@summitlogistics.com', '+1-555-0105', '330 Commerce Dr, Chicago, IL', '2026-03-05 10:20:00'),
(6, 'Fiona Gallagher', 'fiona.g@southsideconsulting.com', '+1-555-0106', '12 Michigan Ave, Chicago, IL', '2026-03-18 13:00:00'),
(7, 'George Clark', 'george.clark@pioneerenergy.com', '+1-555-0107', '88 Greenway Blvd, Denver, CO', '2026-04-02 08:50:00'),
(8, 'Hannah Abbott', 'hannah.a@badgercorp.net', '+1-555-0108', '505 Oak St, Boston, MA', '2026-04-10 17:10:00');

-- ----------------------------------------------------------------------------
-- 3. SEED EMPLOYEES
-- ----------------------------------------------------------------------------
INSERT INTO employees (employee_id, name, email, phone, role, joining_date) VALUES
(1, 'Sarah Connor', 'sarah.connor@crmsystem.internal', '+1-555-1001', 'Senior Support Specialist', '2024-03-01'),
(2, 'James Holden', 'james.holden@crmsystem.internal', '+1-555-1002', 'Support Engineer', '2024-06-15'),
(3, 'Naomi Nagata', 'naomi.nagata@crmsystem.internal', '+1-555-1003', 'Technical Lead', '2023-11-01'),
(4, 'Alex Kamal', 'alex.kamal@crmsystem.internal', '+1-555-1004', 'Account Manager', '2025-01-10'),
(5, 'Amos Burton', 'amos.burton@crmsystem.internal', '+1-555-1005', 'Customer Success Rep', '2025-02-20');

-- ----------------------------------------------------------------------------
-- 4. SEED PRODUCTS
-- ----------------------------------------------------------------------------
INSERT INTO products (product_id, category_id, name, price, stock_quantity, description) VALUES
(1, 1, 'CRM Enterprise Annual License', 1200.00, 50, 'Annual single-tenant enterprise CRM license with priority support'),
(2, 1, 'Cloud Backup Pro (1TB)', 180.00, 100, 'Automated enterprise cloud backup subscription per year'),
(3, 2, 'Dell PowerEdge R750 Server', 3500.00, 8, 'Dual Xeon 32-core rackmount server with 128GB RAM (Low stock)'),
(4, 2, 'ThinkPad X1 Carbon Gen 11', 1450.00, 25, '14-inch Ultrabook Core i7, 32GB RAM, 1TB SSD'),
(5, 2, '4K UltraSharp 27" Monitor', 420.00, 4, 'IPS 4K HDR USB-C hub display (Low stock)'),
(6, 3, 'Cisco Catalyst 24-Port Switch', 850.00, 15, 'Gigabit managed PoE+ rackmount switch'),
(7, 3, 'Ubiquiti UniFi 6 Pro AP', 160.00, 30, 'Wi-Fi 6 dual-band gigabit wireless access point'),
(8, 3, 'Cat6 Ethernet Patch Cable 50ft', 18.50, 200, 'High-speed snagless shielded ethernet cable'),
(9, 4, 'Thunderbolt 4 Docking Station', 220.00, 5, 'Dual 4K display dock with 96W power delivery (Low stock)'),
(10, 4, 'Ergonomic Wireless Keyboard & Mouse', 75.00, 45, 'Quiet-touch mechanical Bluetooth productivity combo');

-- ----------------------------------------------------------------------------
-- 5. SEED ORDERS
-- ----------------------------------------------------------------------------
INSERT INTO orders (order_id, customer_id, order_date, status, total_amount) VALUES
(1, 1, '2026-02-10 10:15:00', 'COMPLETED', 4700.00),
(2, 2, '2026-02-14 11:30:00', 'COMPLETED', 1450.00),
(3, 3, '2026-02-25 15:45:00', 'COMPLETED', 1030.00),
(4, 1, '2026-03-02 09:00:00', 'COMPLETED', 7000.00),
(5, 4, '2026-03-10 14:20:00', 'COMPLETED', 1200.00),
(6, 5, '2026-03-15 16:10:00', 'COMPLETED', 180.00),
(7, 2, '2026-03-22 13:00:00', 'COMPLETED', 850.00),
(8, 6, '2026-04-01 11:15:00', 'PENDING',   580.00),
(9, 3, '2026-04-05 10:40:00', 'PENDING',   1450.00),
(10, 7, '2026-04-08 17:00:00', 'CANCELLED', 220.00);

-- ----------------------------------------------------------------------------
-- 6. SEED ORDER_ITEMS
-- ----------------------------------------------------------------------------
INSERT INTO order_items (order_item_id, order_id, product_id, quantity, unit_price, subtotal) VALUES
-- Order 1: Alice (1 License + 1 Server)
(1, 1, 1, 1, 1200.00, 1200.00),
(2, 1, 3, 1, 3500.00, 3500.00),
-- Order 2: Bob (1 ThinkPad)
(3, 2, 4, 1, 1450.00, 1450.00),
-- Order 3: Charlie (1 Cisco switch + 10 cables)
(4, 3, 6, 1, 850.00, 850.00),
(5, 3, 8, 10, 18.00, 180.00),
-- Order 4: Alice (2 Servers)
(6, 4, 3, 2, 3500.00, 7000.00),
-- Order 5: Diana (1 License)
(7, 5, 1, 1, 1200.00, 1200.00),
-- Order 6: Evan (1 Cloud Backup)
(8, 6, 2, 1, 180.00, 180.00),
-- Order 7: Bob (1 Cisco switch)
(9, 7, 6, 1, 850.00, 850.00),
-- Order 8: Fiona (1 Monitor + 2 Keyboards)
(10, 8, 5, 1, 430.00, 430.00),
(11, 8, 10, 2, 75.00, 150.00),
-- Order 9: Charlie (1 ThinkPad)
(12, 9, 4, 1, 1450.00, 1450.00),
-- Order 10: George (1 Docking Station - Cancelled)
(13, 10, 9, 1, 220.00, 220.00);

-- ----------------------------------------------------------------------------
-- 7. SEED PAYMENTS
-- ----------------------------------------------------------------------------
INSERT INTO payments (payment_id, order_id, payment_date, amount, payment_method, status) VALUES
(1, 1, '2026-02-10 10:18:00', 4700.00, 'BANK_TRANSFER', 'PAID'),
(2, 2, '2026-02-14 11:32:00', 1450.00, 'CREDIT_CARD',  'PAID'),
(3, 3, '2026-02-25 15:47:00', 1030.00, 'UPI',          'PAID'),
(4, 4, '2026-03-02 09:05:00', 7000.00, 'BANK_TRANSFER', 'PAID'),
(5, 5, '2026-03-10 14:22:00', 1200.00, 'CREDIT_CARD',  'PAID'),
(6, 6, '2026-03-15 16:12:00', 180.00,  'DEBIT_CARD',   'PAID'),
(7, 7, '2026-03-22 13:05:00', 850.00,  'CREDIT_CARD',  'PAID'),
(8, 8, '2026-04-01 11:16:00', 580.00,  'UPI',          'PAID'),
(9, 9, '2026-04-05 10:42:00', 1450.00, 'BANK_TRANSFER', 'PAID'),
(10, 10, '2026-04-08 17:05:00', 220.00, 'CREDIT_CARD', 'REFUNDED');

-- ----------------------------------------------------------------------------
-- 8. SEED SUPPORT_TICKETS
-- (Covers CRITICAL, HIGH, NORMAL, LOW priorities and multiple statuses)
-- ----------------------------------------------------------------------------
INSERT INTO support_tickets (ticket_id, customer_id, assigned_employee_id, priority, status, subject, description, created_at, resolved_at) VALUES
(1, 1, 1, 'CRITICAL', 'RESOLVED', 'Database connectivity failure on PowerEdge', 'Primary node unreachable after sudden power flicker.', '2026-02-12 08:30:00', '2026-02-12 11:45:00'),
(2, 2, 2, 'HIGH',     'RESOLVED', 'VPN gateway handshake timeout', 'Employees in Austin cannot authenticate via RADIUS.', '2026-02-16 10:00:00', '2026-02-16 13:20:00'),
(3, 3, 1, 'NORMAL',   'CLOSED',   'Inquiry regarding bulk cable orders', 'Need quotation for 500 units of shielded Cat6 cables.', '2026-03-01 14:00:00', '2026-03-02 09:30:00'),
(4, 4, 3, 'CRITICAL', 'IN_PROGRESS', 'Production CRM API returning 500 error', 'License check daemon crashing on startup after patch.', '2026-04-10 09:15:00', NULL),
(5, 1, 2, 'HIGH',     'OPEN',     'Second server fan redundancy warning', 'BMC alert showing RPM drop in redundant fan bay 3.', '2026-04-11 11:00:00', NULL),
(6, 5, 2, 'NORMAL',   'OPEN',     'Request automated weekly backup reports', 'Configure automated PDF email delivery every Monday.', '2026-04-12 15:30:00', NULL),
(7, 6, NULL, 'LOW',    'OPEN',     'Clarification on monitor return policy', 'Need details on dead-pixel replacement conditions.', '2026-04-13 16:45:00', NULL),
(8, 2, NULL, 'HIGH',   'OPEN',     'Secondary switch packet drop detected', 'Interface gig1/0/12 experiencing high CRC error rates.', '2026-04-14 08:00:00', NULL);

-- ----------------------------------------------------------------------------
-- 9. SEED TICKET_UPDATES
-- ----------------------------------------------------------------------------
INSERT INTO ticket_updates (update_id, ticket_id, employee_id, update_text, status_change, updated_at) VALUES
(1, 1, 1, 'Replaced failed power supply cable and ran BMC diagnostics. All green.', 'IN_PROGRESS -> RESOLVED', '2026-02-12 11:40:00'),
(2, 2, 2, 'Updated expired root CA certificate on the Austin VPN gateway.', 'IN_PROGRESS -> RESOLVED', '2026-02-16 13:15:00'),
(3, 3, 1, 'Sent custom volume quotation with 15% corporate discount.', 'OPEN -> CLOSED', '2026-03-02 09:25:00'),
(4, 4, 3, 'Identified null pointer dereference in SSL validation module. Building hotfix.', 'OPEN -> IN_PROGRESS', '2026-04-10 11:30:00');
