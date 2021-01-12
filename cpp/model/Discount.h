#ifndef CPP_DISCOUNT_H
#define CPP_DISCOUNT_H


#include <string>
#include "Product.h"

class Discount {
public:
    Discount(const std::string& description, double discountAmount, const Product& product);

    std::string getDescription() const;

    double getDiscountAmount() const;

    Product getProduct() const;

    std::string getProductName() const;

private:
    std::string description;
    double discountAmount;
    Product product;
};


#endif //CPP_DISCOUNT_H
