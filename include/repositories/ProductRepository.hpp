#ifndef PRODUCT_REPOSITORY_HPP
#define PRODUCT_REPOSITORY_HPP

#include <vector>
#include <optional>
#include "../models/Product.hpp"
#include "../database/DatabaseManager.hpp"

using namespace std;

class ProductRepository {
private:
    DatabaseManager& db;

public:
    explicit ProductRepository(DatabaseManager& dbManager = DatabaseManager::getInstance());

    bool addProduct(Product& product);
    bool updateProduct(const Product& product);
    bool deleteProduct(int productId);

    optional<Product> getById(int productId);
    vector<Product> search(const string& searchTerm);
    vector<Product> getAll();
    vector<Product> getLowStock(int threshold = 10);

    bool updatePrice(int productId, double newPrice);
    bool updateStock(int productId, int newStock);
    bool deductStock(int productId, int quantity);
    bool restoreStock(int productId, int quantity);

    bool isReferencedInOrders(int productId);
};

#endif // PRODUCT_REPOSITORY_HPP
