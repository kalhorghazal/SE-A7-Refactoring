#ifndef CPP_RECEIPTPRINTER_H
#define CPP_RECEIPTPRINTER_H


#include "Receipt.h"

#include <iomanip>
#include <sstream>

#define COLUMN_NUM 40
#define NEW_LINE "\n"
#define SPACE " "

class ReceiptPrinter
{

public:
    ReceiptPrinter() : ReceiptPrinter(COLUMN_NUM) {}

    ReceiptPrinter(int columns) : columns(columns) {}

    std::string printReceipt(const Receipt &receipt);

    std::string presentReceiptItem(const ReceiptItem &item) const;

    std::string presentDiscount(const Discount &discount) const;

    std::string presentTotal(const Receipt &receipt) const;

    std::string formatLineWithWhitespace(const std::string &name, const std::string &value) const;

    std::string presentPrice(double price) const;


    static std::string presentQuantity(const ReceiptItem &item) 
    {
        if (item.hasEachProductUnit()) {
            return getFormattedNumberAsString(item.getQuantity(), 0);
        }

        return getFormattedNumberAsString(item.getQuantity(), 3);
    }

private:

    static std::string getFormattedNumberAsString(double number, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << number;
        return stream.str();
    }

    const int columns;


};


#endif //CPP_RECEIPTPRINTER_H
