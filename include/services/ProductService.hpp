#ifndef PRODUCT_SERVICE_HPP
#define PRODUCT_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Product.hpp"
#include "../repositories/ProductRepository.hpp"

using namespace std;

class ProductService {
private:
    ProductRepository repo;

public:
    explicit ProductService(ProductRepository repository = ProductRepository());

    Product addProduct(int categoryId, const string& name, double price,
                       int initialStock, const string& description = "");
    bool updateProductDetails(int id, int categoryId, const string& name,
                              double price, int stock, const string& description);
    bool updatePrice(int id, double newPrice);
    bool updateInventory(int id, int newStock);
    bool deleteProduct(int id);

    optional<Product> getProductById(int id);
    vector<Product> searchProducts(const string& query);
    vector<Product> getAllProducts();
    vector<Product> getLowStockAlerts(int threshold = 10);
};

#endif // PRODUCT_SERVICE_HPP
