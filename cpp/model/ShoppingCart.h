#ifndef CPP_SHOPPINGCART_H
#define CPP_SHOPPINGCART_H


#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "ProductQuantity.h"
#include "Offer.h"
#include "Receipt.h"
#include "SupermarketCatalog.h"

#define DEFAULT_QUANTITY 1.0

class ShoppingCart {
public:
    std::vector<ProductQuantity> getItems() const;

    std::map<Product, double> getProductQuantities() const;

    void addItem(const Product& product);

    void addItemQuantity(const Product& product, double quantity);

    void handleOffers(Receipt& receipt, std::map<Product, Offer> offers, 
        SupermarketCatalog* catalog);

    void checkThreeForTwoDiscount(Receipt& receipt, Offer& offer, Product& product, 
            double unitPrice, double quantity);

    void checkTwoForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, 
            double unitPrice, double quantity);

    void checkFiveForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, 
            double unitPrice, double quantity);

    void checkTenPercentDiscount(Receipt& receipt, Offer& offer, Product& product, 
            double unitPrice, double quantity);

    void checkAmountDiscount(Receipt& receipt, Product& product, double amount, 
            double unitPrice, double quantity, int count);

    double computeAmountDiscount(double amount, double quantity, double unitPrice, 
            int count);
			
	void checkBundleDiscount(Receipt& receipt, Offer& offer, Product& product, 
			double unitPrice, double quantity);
			
	void applyBundles(std::vector <std::vector <std::string> > bundles_list);



private:
    std::vector<ProductQuantity> items;
    std::map<Product, double> productQuantities;
	std::vector <std::vector <std::string> > bundles;

};


#endif //CPP_SHOPPINGCART_H
