#include "ShoppingCart.h"

void addItemQuantity(const Product& product, double quantity);

std::vector<ProductQuantity> ShoppingCart::getItems() const {
    return items;
}

std::map<Product, double> ShoppingCart::getProductQuantities() const {
    return productQuantities;
}

void ShoppingCart::addItem(const Product& product) {
    addItemQuantity(product, DEFAULT_QUANTITY);
}

void ShoppingCart::addItemQuantity(const Product& product, double quantity) {
    items.emplace_back(product, quantity);
    if (productQuantities.find(product) != productQuantities.end()) {
        productQuantities[product] += quantity;
    } else {
        productQuantities[product] = quantity;
    }
}

void ShoppingCart::applyBundles(std::vector <std::vector <std::string> > bundles_list) {
    bundles = bundles_list;
}

double ShoppingCart::computeAmountDiscount(double amount, double quantity, double unitPrice, 
        int count) {

    int quantityAsInt = (int) quantity;
    int numberOfXs = quantityAsInt / count;
    double total = (numberOfXs * amount) + quantityAsInt % count * unitPrice;
    double discountAmount = unitPrice * quantity - total;
    return -discountAmount;
}

void ShoppingCart::checkThreeForTwoDiscount(Receipt& receipt, Offer& offer, Product& product, 
        double unitPrice, double quantity) {

    if (offer.getOfferType() == SpecialOfferType::ThreeForTwo && (int) quantity >= 3) {
        double amount = 2 * unitPrice;
        receipt.addDiscount(product, "3 for 2", computeAmountDiscount(amount, quantity, unitPrice, 3));
    }
}

void ShoppingCart::checkAmountDiscount(Receipt& receipt, Product& product, double amount, 
        double unitPrice, double quantity, int count) {

    if ((int) quantity >= count) {
        double discountAmount = computeAmountDiscount(amount, quantity, unitPrice, count);
        std::string description = std::to_string(count) + " for " + std::to_string(amount);
        receipt.addDiscount(product, description, discountAmount);
    }
}

void ShoppingCart::checkTwoForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, 
        double unitPrice, double quantity) {

    int quantityAsInt = (int) quantity;
    if (offer.getOfferType() == SpecialOfferType::TwoForAmount) {
        checkAmountDiscount(receipt, product, offer.getArgument(), unitPrice, quantity, 2);
    }
}

void ShoppingCart::checkFiveForAmountDiscount(Receipt& receipt, Offer& offer, Product& product, 
        double unitPrice, double quantity) {

    if (offer.getOfferType() == SpecialOfferType::FiveForAmount) {
        checkAmountDiscount(receipt, product, offer.getArgument(), unitPrice, quantity, 5);
    }
}

void ShoppingCart::checkTenPercentDiscount(Receipt& receipt, Offer& offer, Product& product, 
        double unitPrice, double quantity) {

    if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
        double discountAmount = quantity * unitPrice * offer.getArgument() / 100.0;
        std::string description = std::to_string(offer.getArgument()) + "% off";
        receipt.addDiscount(product, description, -discountAmount);
    }
}

void ShoppingCart::checkBundleDiscount(Receipt& receipt, Offer& offer, Product& product, 
        double unitPrice, double quantity) {
	double productQuantity = quantity;
    if (offer.getOfferType() == SpecialOfferType::Bundle) {
		for(int i=0;i<bundles.size();i++){
            std::vector <std::string>::iterator it;
			int bundleIndex = -1;
            for(int k=0;k<bundles[i].size();k++)
				if(bundles[i][k] == product.getName()){
					bundleIndex = k;
				}

			if((bundleIndex != -1) && (productQuantity>0)){
				double bundlesQuantity = productQuantity;
                bool unmatchedBundle = false;
                for(int j=0;j<bundles[i].size();j++){
                    double itemQuantity = receipt.getItemQuantity(bundles[i][j]);
					if(itemQuantity == -1) {
                        unmatchedBundle = true;
                        break;
                    }    
                    bundlesQuantity = std::min(bundlesQuantity, itemQuantity);
                }
                if(unmatchedBundle == false)
                    productQuantity -= bundlesQuantity;
            }   
        }
        double discountAmount = (quantity - productQuantity) * unitPrice * offer.getArgument() / 100.0;
		
		std::string description = std::to_string(offer.getArgument()) + "% off";
		receipt.addDiscount(product, description, -discountAmount);
	}
}

void ShoppingCart::handleOffers(Receipt& receipt, std::map<Product, Offer> offers, 
        SupermarketCatalog* catalog) {

    for (const auto& productQuantity : productQuantities) {
        Product product = productQuantity.first;
        double quantity = productQuantity.second;
        if (offers.find(product) != offers.end()) {
            auto offer = offers[product];           
            checkThreeForTwoDiscount(receipt, offer, product, catalog->getUnitPrice(product), quantity);
            checkTwoForAmountDiscount(receipt, offer, product, catalog->getUnitPrice(product), quantity);
            checkFiveForAmountDiscount(receipt, offer, product, catalog->getUnitPrice(product), quantity);
            checkTenPercentDiscount(receipt, offer, product, catalog->getUnitPrice(product), quantity);
			checkBundleDiscount(receipt, offer, product, catalog->getUnitPrice(product), quantity);
        }
    }
}
