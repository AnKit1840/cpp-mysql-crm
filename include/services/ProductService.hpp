#ifndef PRODUCT_SERVICE_HPP
#define PRODUCT_SERVICE_HPP

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include "../models/Product.hpp"
#include "../repositories/ProductRepository.hpp"

class ProductService {
private:
    ProductRepository repo;

public:
    explicit ProductService(ProductRepository repository = ProductRepository());

    Product addProduct(int categoryId, const std::string& name, double price,
                       int initialStock, const std::string& description = "");
    bool updateProductDetails(int id, int categoryId, const std::string& name,
                              double price, int stock, const std::string& description);
    bool updatePrice(int id, double newPrice);
    bool updateInventory(int id, int newStock);
    bool deleteProduct(int id);

    std::optional<Product> getProductById(int id);
    std::vector<Product> searchProducts(const std::string& query);
    std::vector<Product> getAllProducts();
    std::vector<Product> getLowStockAlerts(int threshold = 10);
};

#endif // PRODUCT_SERVICE_HPP
