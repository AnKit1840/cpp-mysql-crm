#include "../../include/services/ProductService.hpp"

using namespace std;

ProductService::ProductService(ProductRepository repository) : repo(repository) {}

Product ProductService::addProduct(int categoryId, const string& name, double price,
                                   int initialStock, const string& description) {
    if (name.empty()) {
        throw invalid_argument("Product name cannot be empty.");
    }
    if (price < 0.0) {
        throw invalid_argument("Product price cannot be negative.");
    }
    if (initialStock < 0) {
        throw invalid_argument("Initial stock quantity cannot be negative.");
    }

    Product prod(0, categoryId, name, price, initialStock, description);
    if (!repo.addProduct(prod)) {
        throw runtime_error("Failed to insert product record into database.");
    }
    return prod;
}

bool ProductService::updateProductDetails(int id, int categoryId, const string& name,
                                          double price, int stock, const string& description) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw invalid_argument("Product ID #" + to_string(id) + " not found.");
    }
    if (price < 0.0) {
        throw invalid_argument("Product price cannot be negative.");
    }
    if (stock < 0) {
        throw invalid_argument("Stock quantity cannot be negative.");
    }

    Product updated(id, categoryId, name, price, stock, description);
    return repo.updateProduct(updated);
}

bool ProductService::updatePrice(int id, double newPrice) {
    if (newPrice < 0.0) {
        throw invalid_argument("Product price cannot be negative.");
    }
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw invalid_argument("Product ID #" + to_string(id) + " not found.");
    }
    return repo.updatePrice(id, newPrice);
}

bool ProductService::updateInventory(int id, int newStock) {
    if (newStock < 0) {
        throw invalid_argument("Stock quantity cannot be negative.");
    }
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw invalid_argument("Product ID #" + to_string(id) + " not found.");
    }
    return repo.updateStock(id, newStock);
}

bool ProductService::deleteProduct(int id) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw invalid_argument("Product ID #" + to_string(id) + " not found.");
    }

    if (repo.isReferencedInOrders(id)) {
        throw runtime_error("Cannot delete product: It is referenced in past customer order history.");
    }

    return repo.deleteProduct(id);
}

optional<Product> ProductService::getProductById(int id) {
    return repo.getById(id);
}

vector<Product> ProductService::searchProducts(const string& query) {
    return repo.search(query);
}

vector<Product> ProductService::getAllProducts() {
    return repo.getAll();
}

vector<Product> ProductService::getLowStockAlerts(int threshold) {
    return repo.getLowStock(threshold);
}
