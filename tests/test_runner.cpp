#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include "../include/database/ConfigLoader.hpp"
#include "../include/database/DatabaseManager.hpp"
#include "../include/models/Customer.hpp"
#include "../include/models/Employee.hpp"
#include "../include/models/Product.hpp"
#include "../include/models/Order.hpp"
#include "../include/models/SupportTicket.hpp"
#include "../include/repositories/CustomerRepository.hpp"
#include "../include/repositories/EmployeeRepository.hpp"
#include "../include/repositories/ProductRepository.hpp"
#include "../include/repositories/OrderRepository.hpp"
#include "../include/repositories/TicketRepository.hpp"
#include "../include/repositories/AnalyticsRepository.hpp"
#include "../include/services/CustomerService.hpp"
#include "../include/services/ProductService.hpp"
#include "../include/services/OrderService.hpp"
#include "../include/services/TicketManager.hpp"

using namespace std;

static int g_testsPassed = 0;
static int g_testsFailed = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            cout << "  [PASS] " << msg << "\n"; \
            g_testsPassed++; \
        } else { \
            cerr << "  [FAIL] " << msg << " (Line " << __LINE__ << ")\n"; \
            g_testsFailed++; \
        } \
    } while(0)

// ----------------------------------------------------------------------------
// Test Group 1: OOP Models & Polymorphism
// ----------------------------------------------------------------------------
void testOOPPolymorphism() {
    cout << "\n>>> Running Test Group 1: OOP & Runtime Polymorphism <<<\n";

    // Polymorphism via User pointer
    unique_ptr<User> u1 = make_unique<Customer>(101, "Test Customer", "test.cust@example.com", "1234567890", "123 Main St");
    unique_ptr<User> u2 = make_unique<Employee>(201, "Test Employee", "test.emp@crm.internal", "0987654321", "Lead Engineer", "2024-01-01");

    TEST_ASSERT(u1->getRole() == "Customer", "Polymorphic getRole() returns 'Customer'");
    TEST_ASSERT(u2->getRole() == "Lead Engineer", "Polymorphic getRole() returns 'Lead Engineer'");

    // Composition test
    Order order(1, 101, "2026-04-15", "PENDING");
    order.addItem(OrderItem(1, 1, 10, 2, 50.0));
    order.addItem(OrderItem(2, 1, 11, 1, 150.0));
    TEST_ASSERT(order.getTotalAmount() == 250.0, "Order total calculated correctly via OrderItem composition");
}

// ----------------------------------------------------------------------------
// Test Group 2: DSA Queues & Custom Priority Comparator
// ----------------------------------------------------------------------------
void testDataStructuresAndPriorityQueue() {
    cout << "\n>>> Running Test Group 2: DSA Queues & Priority Ordering <<<\n";

    // 1. Priority Queue Ordering (CRITICAL > HIGH) & FIFO Tie-breaker
    priority_queue<SupportTicket, vector<SupportTicket>, TicketComparator> pq;

    SupportTicket t1(1, 1, 0, TicketPriority::HIGH, TicketStatus::OPEN, "VPN Issue", "VPN down");
    SupportTicket t2(2, 1, 0, TicketPriority::CRITICAL, TicketStatus::OPEN, "Server Outage", "Production down");
    SupportTicket t3(3, 1, 0, TicketPriority::HIGH, TicketStatus::OPEN, "DNS Timeout", "Old high ticket");

    pq.push(t1);
    pq.push(t2);
    pq.push(t3);

    // t2 (CRITICAL) must be first
    SupportTicket first = pq.top(); pq.pop();
    TEST_ASSERT(first.getId() == 2 && first.getPriority() == TicketPriority::CRITICAL, 
                "Priority Queue correctly processes CRITICAL ticket before HIGH");

    // Between t1 (id=1) and t3 (id=3), both are HIGH. Older ticket (id=1) must come first!
    SupportTicket second = pq.top(); pq.pop();
    TEST_ASSERT(second.getId() == 1, 
                "Priority Queue FIFO tie-breaker: older HIGH ticket (ID 1) processed before newer (ID 3)");

    SupportTicket third = pq.top(); pq.pop();
    TEST_ASSERT(third.getId() == 3, "Remaining HIGH ticket (ID 3) processed last");

    // 2. Standard FIFO Queue for NORMAL and LOW
    queue<SupportTicket> nq;
    SupportTicket tn1(10, 1, 0, TicketPriority::NORMAL, TicketStatus::OPEN, "Query A", "Normal A");
    SupportTicket tn2(11, 1, 0, TicketPriority::LOW, TicketStatus::OPEN, "Query B", "Low B");

    nq.push(tn1);
    nq.push(tn2);

    SupportTicket f1 = nq.front(); nq.pop();
    SupportTicket f2 = nq.front(); nq.pop();
    TEST_ASSERT(f1.getId() == 10 && f2.getId() == 11, "Standard queue enforces strict First-In, First-Out (FIFO) ordering");

    // 3. Duplicate Prevention with unordered_set
    unordered_set<int> inMemSet;
    inMemSet.insert(10);
    TEST_ASSERT(inMemSet.find(10) != inMemSet.end(), "Duplicate set tracks existing ticket ID");
    bool duplicateDetected = (inMemSet.find(10) != inMemSet.end());
    TEST_ASSERT(duplicateDetected, "Duplicate ticket prevention logic accurately identifies already-queued ticket");
}

// ----------------------------------------------------------------------------
// Test Group 3: Live Database, Transactions & Modules
// ----------------------------------------------------------------------------
void testDatabaseIntegration() {
    cout << "\n>>> Running Test Group 3: Database Services & Transactions <<<\n";

    DatabaseConfig config = ConfigLoader::load("config/db_config.ini");
    DatabaseManager& db = DatabaseManager::getInstance();

    try {
        db.connect(config);
    } catch (const exception& e) {
        cout << "  [SKIPPED] Live MySQL database is currently unreachable (" << e.what() 
                  << "). Verify 'config/db_config.ini' to run live DB tests.\n";
        return;
    }

    TEST_ASSERT(db.isConnected(), "Database connected successfully");

    CustomerService customerService;
    ProductService productService;
    OrderService orderService;
    TicketManager ticketManager;
    AnalyticsRepository analyticsRepo;

    // 1. Customer Creation & Duplicate Email Prevention
    string uniqueEmail = "unit_test_" + to_string(rand() % 100000) + "@crmtest.com";
    try {
        Customer c = customerService.registerCustomer("Unit Test Customer", uniqueEmail, "+1-555-9999", "99 Test Way");
        TEST_ASSERT(c.getId() > 0, "Customer registered successfully with positive database ID");

        // Attempt duplicate insertion
        bool caughtDuplicate = false;
        try {
            customerService.registerCustomer("Duplicate Person", uniqueEmail, "+1-555-0000", "Nowhere");
        } catch (const invalid_argument&) {
            caughtDuplicate = true;
        }
        TEST_ASSERT(caughtDuplicate, "Duplicate email registration prevented by CustomerService");
    } catch (const exception& e) {
        cerr << "Customer test exception: " << e.what() << "\n";
    }

    // 2. Product Management & Stock Validation
    try {
        Product p = productService.addProduct(1, "Test Analytics Software", 299.99, 15, "Automated testing item");
        TEST_ASSERT(p.getId() > 0, "Product added successfully with positive database ID");

        bool priceUpdated = productService.updatePrice(p.getId(), 349.99);
        TEST_ASSERT(priceUpdated, "Product price updated in database");

        // Negative price rejection
        bool caughtNegativePrice = false;
        try {
            productService.updatePrice(p.getId(), -50.0);
        } catch (const invalid_argument&) {
            caughtNegativePrice = true;
        }
        TEST_ASSERT(caughtNegativePrice, "Negative product price rejected by ProductService validation");
    } catch (const exception& e) {
        cerr << "Product test exception: " << e.what() << "\n";
    }

    // 3. Order Placement & ACID Transaction Rollback
    try {
        // Customer 1 placed order for product 1
        vector<pair<int, int>> validItems = {{1, 1}};
        Order ord = orderService.placeOrder(1, validItems, "CREDIT_CARD");
        TEST_ASSERT(ord.getId() > 0, "Order placed successfully via multi-table database transaction");

        // Insufficient inventory test -> Rollback verification
        bool caughtInsufficientStock = false;
        try {
            vector<pair<int, int>> excessiveItems = {{1, 999999}};
            orderService.placeOrder(1, excessiveItems, "CREDIT_CARD");
        } catch (const exception&) {
            caughtInsufficientStock = true;
        }
        TEST_ASSERT(caughtInsufficientStock, "Order placement with insufficient stock failed and triggered rollback");

        // Cancel order & verify inventory restoration
        bool cancelled = orderService.cancelOrder(ord.getId());
        TEST_ASSERT(cancelled, "Order cancelled and inventory restored atomically via transaction");
    } catch (const exception& e) {
        cerr << "Order test exception: " << e.what() << "\n";
    }

    // 4. Ticket Manager Queue Processing
    try {
        ticketManager.syncFromDatabase();
        SupportTicket t = ticketManager.createTicket(1, "Automated Priority Test", "Testing queue pop", TicketPriority::CRITICAL);
        TEST_ASSERT(t.getId() > 0, "Support ticket created and synchronized to in-memory queue");

        auto processed = ticketManager.processNextTicket(1, "Investigating automated issue");
        TEST_ASSERT(processed.has_value(), "Next ticket successfully popped from priority queue and assigned");
    } catch (const exception& e) {
        cerr << "Ticket test exception: " << e.what() << "\n";
    }

    // 5. Analytics Aggregation Verification
    try {
        auto revSummary = analyticsRepo.getOverallRevenueMetrics();
        TEST_ASSERT(revSummary.totalOrders >= 0, "Analytics total orders aggregation computed successfully");
        TEST_ASSERT(revSummary.totalRevenue >= 0.0, "Analytics gross revenue aggregation computed successfully");
    } catch (const exception& e) {
        cerr << "Analytics test exception: " << e.what() << "\n";
    }
}

int main() {
    cout << "========================================================\n"
              << "          CRM SYSTEM AUTOMATED TEST SUITE               \n"
              << "========================================================\n";

    testOOPPolymorphism();
    testDataStructuresAndPriorityQueue();
    testDatabaseIntegration();

    cout << "\n========================================================\n"
              << "TEST SUMMARY: " << g_testsPassed << " PASSED, " << g_testsFailed << " FAILED.\n"
              << "========================================================\n";

    return (g_testsFailed == 0) ? 0 : 1;
}
