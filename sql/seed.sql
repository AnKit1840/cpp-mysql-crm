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
(1, 'Kitchenware', 'Cookware and everyday kitchen tools'),
(2, 'Home Appliances', 'Small appliances for Indian homes'),
(3, 'Handloom', 'Clothing and accessories from weaving clusters'),
(4, 'Stationery', 'Paper goods and writing supplies');

-- ----------------------------------------------------------------------------
-- 2. SEED CUSTOMERS
-- ----------------------------------------------------------------------------
INSERT INTO customers (customer_id, name, email, phone, address, created_at) VALUES
(1, 'Aditi Kulkarni', 'aditi.kulkarni@example.com', '+91 90000 10001', '18 Karve Road, Pune', '2026-01-10 09:30:00'),
(2, 'Arjun Iyer', 'arjun.iyer@example.com', '+91 90000 10002', '42 Adyar Main Road, Chennai', '2026-01-15 14:15:00'),
(3, 'Meera Shah', 'meera.shah@example.com', '+91 90000 10003', '7 Navrangpura Road, Ahmedabad', '2026-02-01 11:00:00'),
(4, 'Harpreet Kaur', 'harpreet.kaur@example.com', '+91 90000 10004', '26 Model Town, Ludhiana', '2026-02-12 16:45:00'),
(5, 'Rohan Das', 'rohan.das@example.com', '+91 90000 10005', '14 Lake Road, Kolkata', '2026-03-05 10:20:00'),
(6, 'Nisha Menon', 'nisha.menon@example.com', '+91 90000 10006', '31 Kaloor Road, Kochi', '2026-03-18 13:00:00'),
(7, 'Imran Khan', 'imran.khan@example.com', '+91 90000 10007', '9 Mansarovar, Jaipur', '2026-04-02 08:50:00'),
(8, 'Kavya Reddy', 'kavya.reddy@example.com', '+91 90000 10008', '12 Ameerpet Road, Hyderabad', '2026-04-10 17:10:00');

-- ----------------------------------------------------------------------------
-- 3. SEED EMPLOYEES
-- ----------------------------------------------------------------------------
INSERT INTO employees (employee_id, name, email, phone, role, joining_date) VALUES
(1, 'Sneha Patil', 'sneha.patil@example.com', '+91 90000 10009', 'Senior Support Specialist', '2024-03-01'),
(2, 'Vikram Nair', 'vikram.nair@example.com', '+91 90000 10010', 'Support Engineer', '2024-06-15'),
(3, 'Ananya Sen', 'ananya.sen@example.com', '+91 90000 10011', 'Technical Lead', '2023-11-01'),
(4, 'Kabir Joshi', 'kabir.joshi@example.com', '+91 90000 10012', 'Account Manager', '2025-01-10'),
(5, 'Farah Ali', 'farah.ali@example.com', '+91 90000 10013', 'Customer Success Rep', '2025-02-20');

-- ----------------------------------------------------------------------------
-- 4. SEED PRODUCTS
-- ----------------------------------------------------------------------------
INSERT INTO products (product_id, category_id, name, price, stock_quantity, description) VALUES
(1, 1, 'Hawkins pressure cooker', 2450.00, 50, 'Three-litre cooker for dal and rice'),
(2, 1, 'Pigeon tawa', 890.00, 100, 'Flat iron tawa for dosa and roti'),
(3, 2, 'Sujata mixer grinder', 6290.00, 8, 'Mixer with steel jars for chutneys and masalas'),
(4, 2, 'Bajaj ceiling fan', 2350.00, 25, 'Three-blade fan in ivory'),
(5, 2, 'Usha dry iron', 1150.00, 4, 'Lightweight iron with a temperature dial'),
(6, 3, 'Sambalpuri cotton saree', 3200.00, 15, 'Cotton ikat saree from Odisha'),
(7, 3, 'Kullu wool stole', 1250.00, 30, 'Patterned wool stole from Himachal Pradesh'),
(8, 3, 'Gamosa cotton towel', 250.00, 200, 'White cotton towel with a woven red border'),
(9, 4, 'Classmate notebook pack', 360.00, 5, 'Six ruled notebooks for school or office'),
(10, 4, 'Apsara pencil box', 65.00, 45, 'Box of ten drawing pencils');

-- ----------------------------------------------------------------------------
-- 5. SEED ORDERS
-- ----------------------------------------------------------------------------
INSERT INTO orders (order_id, customer_id, order_date, status, total_amount) VALUES
(1, 1, '2026-02-10 10:15:00', 'COMPLETED', 8740.00),
(2, 2, '2026-02-14 11:30:00', 'COMPLETED', 2350.00),
(3, 3, '2026-02-25 15:45:00', 'COMPLETED', 5700.00),
(4, 1, '2026-03-02 09:00:00', 'COMPLETED', 12580.00),
(5, 4, '2026-03-10 14:20:00', 'COMPLETED', 2450.00),
(6, 5, '2026-03-15 16:10:00', 'COMPLETED', 890.00),
(7, 2, '2026-03-22 13:00:00', 'COMPLETED', 3200.00),
(8, 6, '2026-04-01 11:15:00', 'PENDING', 1280.00),
(9, 3, '2026-04-05 10:40:00', 'PENDING', 2350.00),
(10, 7, '2026-04-08 17:00:00', 'CANCELLED', 360.00);

-- ----------------------------------------------------------------------------
-- 6. SEED ORDER_ITEMS
-- ----------------------------------------------------------------------------
INSERT INTO order_items (order_item_id, order_id, product_id, quantity, unit_price, subtotal) VALUES
(1, 1, 1, 1, 2450.00, 2450.00),
(2, 1, 3, 1, 6290.00, 6290.00),
(3, 2, 4, 1, 2350.00, 2350.00),
(4, 3, 6, 1, 3200.00, 3200.00),
(5, 3, 8, 10, 250.00, 2500.00),
(6, 4, 3, 2, 6290.00, 12580.00),
(7, 5, 1, 1, 2450.00, 2450.00),
(8, 6, 2, 1, 890.00, 890.00),
(9, 7, 6, 1, 3200.00, 3200.00),
(10, 8, 5, 1, 1150.00, 1150.00),
(11, 8, 10, 2, 65.00, 130.00),
(12, 9, 4, 1, 2350.00, 2350.00),
(13, 10, 9, 1, 360.00, 360.00);

-- ----------------------------------------------------------------------------
-- 7. SEED PAYMENTS
-- ----------------------------------------------------------------------------
INSERT INTO payments (payment_id, order_id, payment_date, amount, payment_method, status) VALUES
(1, 1, '2026-02-10 10:18:00', 8740.00, 'BANK_TRANSFER', 'PAID'),
(2, 2, '2026-02-14 11:32:00', 2350.00, 'CREDIT_CARD',  'PAID'),
(3, 3, '2026-02-25 15:47:00', 5700.00, 'UPI',          'PAID'),
(4, 4, '2026-03-02 09:05:00', 12580.00, 'BANK_TRANSFER', 'PAID'),
(5, 5, '2026-03-10 14:22:00', 2450.00, 'CREDIT_CARD',  'PAID'),
(6, 6, '2026-03-15 16:12:00', 890.00,  'DEBIT_CARD',   'PAID'),
(7, 7, '2026-03-22 13:05:00', 3200.00,  'CREDIT_CARD',  'PAID'),
(8, 8, '2026-04-01 11:16:00', 1280.00,  'UPI',          'PAID'),
(9, 9, '2026-04-05 10:42:00', 2350.00, 'BANK_TRANSFER', 'PAID'),
(10, 10, '2026-04-08 17:05:00', 360.00, 'CREDIT_CARD', 'REFUNDED');

-- ----------------------------------------------------------------------------
-- 8. SEED SUPPORT_TICKETS
-- (Covers CRITICAL, HIGH, NORMAL, LOW priorities and multiple statuses)
-- ----------------------------------------------------------------------------
INSERT INTO support_tickets (ticket_id, customer_id, assigned_employee_id, priority, status, subject, description, created_at, resolved_at) VALUES
(1, 1, 1, 'CRITICAL', 'RESOLVED', 'Mixer stopped during use', 'My mixer stopped while grinding coconut. Can someone check it?', '2026-02-12 08:30:00', '2026-02-12 11:45:00'),
(2, 2, 2, 'HIGH',     'RESOLVED', 'Fan makes a rattling sound', 'The fan we received in Chennai rattles at the highest speed.', '2026-02-16 10:00:00', '2026-02-16 13:20:00'),
(3, 3, 1, 'NORMAL',   'CLOSED',   'Bulk order for gamosas', 'Could you send a quote for 50 gamosas for our Bihu gathering?', '2026-03-01 14:00:00', '2026-03-02 09:30:00'),
(4, 4, 3, 'CRITICAL', 'IN_PROGRESS', 'Pressure cooker lid leaks steam', 'Steam escapes around the lid. I have stopped using the cooker.', '2026-04-10 09:15:00', NULL),
(5, 1, 2, 'HIGH',     'OPEN',     'Crack in mixer jar', 'One of the two mixers arrived with a cracked chutney jar.', '2026-04-11 11:00:00', NULL),
(6, 5, 2, 'NORMAL',   'OPEN',     'Seasoning a new tawa', 'How should I season the tawa before making my first dosa?', '2026-04-12 15:30:00', NULL),
(7, 6, NULL, 'LOW',    'OPEN',     'Change delivery address', 'Can you deliver the iron and pencils to my office in Kaloor?', '2026-04-13 16:45:00', NULL),
(8, 2, NULL, 'HIGH',   'OPEN',     'Wrong saree colour', 'I ordered the blue Sambalpuri saree but received a green one.', '2026-04-14 08:00:00', NULL);

-- ----------------------------------------------------------------------------
-- 9. SEED TICKET_UPDATES
-- ----------------------------------------------------------------------------
INSERT INTO ticket_updates (update_id, ticket_id, employee_id, update_text, status_change, updated_at) VALUES
(1, 1, 1, 'Replaced the mixer and confirmed it runs with the customer.', 'IN_PROGRESS -> RESOLVED', '2026-02-12 11:40:00'),
(2, 2, 2, 'The technician tightened the fan mount. Arjun confirmed the noise has stopped.', 'IN_PROGRESS -> RESOLVED', '2026-02-16 13:15:00'),
(3, 3, 1, 'Emailed Meera a quote for 50 gamosas with delivery to Ahmedabad.', 'OPEN -> CLOSED', '2026-03-02 09:25:00'),
(4, 4, 3, 'Asked Harpreet to keep the cooker aside and arranged a warranty inspection.', 'OPEN -> IN_PROGRESS', '2026-04-10 11:30:00');
