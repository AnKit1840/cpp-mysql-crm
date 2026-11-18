#include "../../include/services/ProductService.hpp"

ProductService::ProductService(ProductRepository repository) : repo(repository) {}

Product ProductService::addProduct(int categoryId, const std::string& name, double price,
                                   int initialStock, const std::string& description) {
    if (name.empty()) {
        throw std::invalid_argument("Product name cannot be empty.");
    }
    if (price < 0.0) {
        throw std::invalid_argument("Product price cannot be negative.");
    }
    if (initialStock < 0) {
        throw std::invalid_argument("Initial stock quantity cannot be negative.");
    }

    Product prod(0, categoryId, name, price, initialStock, description);
    if (!repo.addProduct(prod)) {
        throw std::runtime_error("Failed to insert product record into database.");
    }
    return prod;
}

bool ProductService::updateProductDetails(int id, int categoryId, const std::string& name,
                                          double price, int stock, const std::string& description) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Product ID #" + std::to_string(id) + " not found.");
    }
    if (price < 0.0) {
        throw std::invalid_argument("Product price cannot be negative.");
    }
    if (stock < 0) {
        throw std::invalid_argument("Stock quantity cannot be negative.");
    }

    Product updated(id, categoryId, name, price, stock, description);
    return repo.updateProduct(updated);
}

bool ProductService::updatePrice(int id, double newPrice) {
    if (newPrice < 0.0) {
        throw std::invalid_argument("Product price cannot be negative.");
    }
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Product ID #" + std::to_string(id) + " not found.");
    }
    return repo.updatePrice(id, newPrice);
}

bool ProductService::updateInventory(int id, int newStock) {
    if (newStock < 0) {
        throw std::invalid_argument("Stock quantity cannot be negative.");
    }
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Product ID #" + std::to_string(id) + " not found.");
    }
    return repo.updateStock(id, newStock);
}

bool ProductService::deleteProduct(int id) {
    auto existing = repo.getById(id);
    if (!existing.has_value()) {
        throw std::invalid_argument("Product ID #" + std::to_string(id) + " not found.");
    }

    if (repo.isReferencedInOrders(id)) {
        throw std::runtime_error("Cannot delete product: It is referenced in past customer order history.");
    }

    return repo.deleteProduct(id);
}

std::optional<Product> ProductService::getProductById(int id) {
    return repo.getById(id);
}

std::vector<Product> ProductService::searchProducts(const std::string& query) {
    return repo.search(query);
}

std::vector<Product> ProductService::getAllProducts() {
    return repo.getAll();
}

std::vector<Product> ProductService::getLowStockAlerts(int threshold) {
    return repo.getLowStock(threshold);
}
