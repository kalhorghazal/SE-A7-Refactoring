#include "ShoppingCart.h"

void addItemQuantity(const Product& product, double quantity);

std::vector<ProductQuantity> ShoppingCart::getItems() const {
    return items;
}

std::map<Product, double> ShoppingCart::getProductQuantities() const {
    return productQuantities;
}

void ShoppingCart::addItem(const Product& product) {
    addItemQuantity(product, 1.0);
}

void ShoppingCart::addItemQuantity(const Product& product, double quantity) {
    items.emplace_back(product, quantity);
    if (productQuantities.find(product) != productQuantities.end()) {
        productQuantities[product] += quantity;
    } else {
        productQuantities[product] = quantity;
    }
}

void ShoppingCart::checkThreeForTwoDiscount(Receipt& receipt, Offer& offer, Product& product, double unitPrice, double quantity) {
    int quantityAsInt = (int) quantity;
    int numberOfXs = quantityAsInt / 3;
    if (offer.getOfferType() == SpecialOfferType::ThreeForTwo && quantityAsInt > 2) {
        double total = (numberOfXs * 2 * unitPrice) + quantityAsInt % 3 * unitPrice;
        double discountAmount = unitPrice * quantity - total;
        Discount* discount = new Discount("3 for 2", -discountAmount, product);
        receipt.addDiscount(*discount);
    }
}

void ShoppingCart::checkTwoForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, double unitPrice, double quantity) {
    int quantityAsInt = (int) quantity;
    if (offer.getOfferType() == SpecialOfferType::TwoForAmount && quantityAsInt >= 2) {
        double total = offer.getArgument() * (quantityAsInt / 2) + quantityAsInt % 2 * unitPrice;
        double discountAmount = unitPrice * quantity - total;
        std::string description = "2 for " + std::to_string(offer.getArgument());
        Discount* discount = new Discount(description, -discountAmount, product);
        receipt.addDiscount(*discount);
    }
}

void ShoppingCart::checkFiveForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, double unitPrice, double quantity) {
    int quantityAsInt = (int) quantity;
    int numberOfXs = quantityAsInt / 5;
    if (offer.getOfferType() == SpecialOfferType::FiveForAmount && quantityAsInt >= 5) {
        double total = offer.getArgument() * numberOfXs + quantityAsInt % 5 * unitPrice;
        double discountAmount = unitPrice * quantity - total;
        std::string description = "5 for " + std::to_string(offer.getArgument());
        Discount* discount = new Discount(description, -discountAmount, product);
        receipt.addDiscount(*discount);
    }
}

void ShoppingCart::checkTenPercentDiscount(Receipt& receipt, Offer& offer, Product& product, double unitPrice, double quantity) {
    if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
        double discountAmount = quantity * unitPrice * offer.getArgument() / 100.0;
        std::string description = std::to_string(offer.getArgument()) + "% off";
        Discount* discount = new Discount(description, -discountAmount, product);
        receipt.addDiscount(*discount);
    }
}

void ShoppingCart::handleOffers(Receipt& receipt, std::map<Product, Offer> offers, SupermarketCatalog* catalog) {
    for (const auto& productQuantity : productQuantities) {
        Product product = productQuantity.first;
        double quantity = productQuantity.second;
        if (offers.find(product) != offers.end()) {
            auto offer = offers[product];
            double unitPrice = catalog->getUnitPrice(product);

            checkThreeForTwoDiscount(receipt, offer, product, unitPrice, quantity);
            checkTwoForAmountDiscount(receipt, offer, product, unitPrice, quantity);
            checkFiveForAmountDiscount(receipt, offer, product, unitPrice, quantity);
            checkTenPercentDiscount(receipt, offer, product, unitPrice, quantity);
        }
    }
}
