-- ============================================================================
-- CRM System Database Schema
-- Database: crm_db
-- Standard: MySQL 8.0+
-- Description: Core 9-table normalized schema for Customer Relationship Management
-- ============================================================================

CREATE DATABASE IF NOT EXISTS crm_db;
USE crm_db;

-- Disable foreign key checks during schema re-creation
SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS ticket_updates;
DROP TABLE IF EXISTS support_tickets;
DROP TABLE IF EXISTS payments;
DROP TABLE IF EXISTS order_items;
DROP TABLE IF EXISTS orders;
DROP TABLE IF EXISTS products;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS customers;

SET FOREIGN_KEY_CHECKS = 1;

-- ----------------------------------------------------------------------------
-- 1. CUSTOMERS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE customers (
    customer_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    phone VARCHAR(20) NOT NULL,
    address VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_customer_email (email),
    INDEX idx_customer_name (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 2. EMPLOYEES TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE employees (
    employee_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    phone VARCHAR(20) NOT NULL,
    role VARCHAR(50) NOT NULL,
    joining_date DATE NOT NULL,
    INDEX idx_employee_email (email),
    INDEX idx_employee_role (role)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 3. CATEGORIES TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE,
    description VARCHAR(255) NULL,
    INDEX idx_category_name (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 4. PRODUCTS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,
    category_id INT NOT NULL,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0.00),
    stock_quantity INT NOT NULL CHECK (stock_quantity >= 0),
    description TEXT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_products_category
        FOREIGN KEY (category_id) REFERENCES categories(category_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    INDEX idx_product_category (category_id),
    INDEX idx_product_stock (stock_quantity),
    INDEX idx_product_name (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 5. ORDERS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE orders (
    order_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT NOT NULL,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('PENDING', 'COMPLETED', 'CANCELLED') NOT NULL DEFAULT 'PENDING',
    total_amount DECIMAL(10, 2) NOT NULL DEFAULT 0.00 CHECK (total_amount >= 0.00),
    CONSTRAINT fk_orders_customer
        FOREIGN KEY (customer_id) REFERENCES customers(customer_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    INDEX idx_orders_customer (customer_id),
    INDEX idx_orders_date (order_date),
    INDEX idx_orders_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 6. ORDER_ITEMS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE order_items (
    order_item_id INT AUTO_INCREMENT PRIMARY KEY,
    order_id INT NOT NULL,
    product_id INT NOT NULL,
    quantity INT NOT NULL CHECK (quantity > 0),
    unit_price DECIMAL(10, 2) NOT NULL CHECK (unit_price >= 0.00),
    subtotal DECIMAL(10, 2) NOT NULL CHECK (subtotal >= 0.00),
    CONSTRAINT fk_items_order
        FOREIGN KEY (order_id) REFERENCES orders(order_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_items_product
        FOREIGN KEY (product_id) REFERENCES products(product_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    INDEX idx_order_items_order (order_id),
    INDEX idx_order_items_product (product_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 7. PAYMENTS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE payments (
    payment_id INT AUTO_INCREMENT PRIMARY KEY,
    order_id INT NOT NULL,
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    amount DECIMAL(10, 2) NOT NULL CHECK (amount >= 0.00),
    payment_method ENUM('CASH', 'CREDIT_CARD', 'DEBIT_CARD', 'UPI', 'BANK_TRANSFER') NOT NULL,
    status ENUM('PAID', 'REFUNDED', 'FAILED') NOT NULL DEFAULT 'PAID',
    CONSTRAINT fk_payments_order
        FOREIGN KEY (order_id) REFERENCES orders(order_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    INDEX idx_payments_order (order_id),
    INDEX idx_payments_status (status),
    INDEX idx_payments_date (payment_date)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 8. SUPPORT_TICKETS TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE support_tickets (
    ticket_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT NOT NULL,
    assigned_employee_id INT NULL,
    priority ENUM('CRITICAL', 'HIGH', 'NORMAL', 'LOW') NOT NULL DEFAULT 'NORMAL',
    status ENUM('OPEN', 'IN_PROGRESS', 'RESOLVED', 'CLOSED') NOT NULL DEFAULT 'OPEN',
    subject VARCHAR(150) NOT NULL,
    description TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    resolved_at TIMESTAMP NULL,
    CONSTRAINT fk_tickets_customer
        FOREIGN KEY (customer_id) REFERENCES customers(customer_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_tickets_employee
        FOREIGN KEY (assigned_employee_id) REFERENCES employees(employee_id)
        ON DELETE SET NULL ON UPDATE CASCADE,
    INDEX idx_tickets_customer (customer_id),
    INDEX idx_tickets_employee (assigned_employee_id),
    INDEX idx_tickets_priority (priority),
    INDEX idx_tickets_status (status),
    INDEX idx_tickets_created_at (created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ----------------------------------------------------------------------------
-- 9. TICKET_UPDATES TABLE
-- ----------------------------------------------------------------------------
CREATE TABLE ticket_updates (
    update_id INT AUTO_INCREMENT PRIMARY KEY,
    ticket_id INT NOT NULL,
    employee_id INT NOT NULL,
    update_text TEXT NOT NULL,
    status_change VARCHAR(50) NOT NULL,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_updates_ticket
        FOREIGN KEY (ticket_id) REFERENCES support_tickets(ticket_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_updates_employee
        FOREIGN KEY (employee_id) REFERENCES employees(employee_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    INDEX idx_updates_ticket (ticket_id),
    INDEX idx_updates_employee (employee_id),
    INDEX idx_updates_date (updated_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
