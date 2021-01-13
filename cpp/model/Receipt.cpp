#include "Receipt.h"

std::vector<ReceiptItem> Receipt::getItems() const {
    return items;
}

void Receipt::addDiscount(const Product& product, std::string description, double discountAmount) {
    Discount* discount = new Discount(description, discountAmount, product);
    discounts.push_back(*discount);
}

void Receipt::addProduct(const Product& product, double quantity, double price, double totalPrice) {
    items.push_back(ReceiptItem(product, quantity, price, totalPrice));
}

std::vector<Discount> Receipt::getDiscounts() const {
    return discounts;
}

double Receipt::getTotalPrice() const {
    double total = std::accumulate(begin(items), end(items), 0.0,
            [](double price, const auto &item) { return price + item.getTotalPrice(); });

    total = std::accumulate(begin(discounts), end(discounts), total,
            [](double val, const auto &discount) { return val + discount.getDiscountAmount(); });

    return total;
}
