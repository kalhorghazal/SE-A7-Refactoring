#ifndef CPP_RECEIPTPRINTER_H
#define CPP_RECEIPTPRINTER_H


#include "Receipt.h"

#include <iomanip>
#include <sstream>

#define COLUMN_NUM 40
#define NEW_LINE "\n"
#define SPACE " "

std::string operator*(std::string const &s, std::size_t n)
{
  std::string r;
  r.reserve(n * s.size());
  for (std::size_t i = 0; i < n; ++i)
    r += s;
  return r;
}

class ReceiptPrinter
{

public:
    ReceiptPrinter() : ReceiptPrinter(COLUMN_NUM)
    {
    }

    ReceiptPrinter(int columns) : columns(columns)
    {}

    std::string printReceipt(const Receipt &receipt)
    {
        std::string result;
        for (const auto &item : receipt.getItems())
        {
            result.append(presentReceiptItem(item));
        }
        for (const auto &discount : receipt.getDiscounts())
        {
            result.append(presentDiscount(discount));
        }
        result.append(NEW_LINE);
        result.append(presentTotal(receipt));
        return result;
    }

    std::string presentReceiptItem(const ReceiptItem &item) const
    {
        std::string price = getFormattedNumberAsString(item.getTotalPrice(), 2);
        std::string name = item.getProductName();

        std::string line = formatLineWithWhitespace(name, price);

        if (item.getQuantity() != 1)
        {
            line += (std::string(SPACE) * 2) + getFormattedNumberAsString(item.getPrice(), 2) + " * " + presentQuantity(item) + NEW_LINE;
        }
        return line;
    }

    std::string presentDiscount(const Discount &discount) const
    {
        std::string name = discount.getDescription() + "(" + discount.getProductName() + ")";
        std::string pricePresentation = getFormattedNumberAsString(discount.getDiscountAmount(), 2);
        return formatLineWithWhitespace(name, pricePresentation);
    }

    std::string presentTotal(const Receipt &receipt) const
    {
        std::string total = "Total: ";
        std::string pricePresentation = presentPrice(receipt.getTotalPrice());
        return formatLineWithWhitespace(total, pricePresentation);
    }

    std::string formatLineWithWhitespace(const std::string &name, const std::string &value) const
    {
        int whitespaceSize = columns - name.length() - value.length();
        std::string formatted = name + (std::string(SPACE) * whitespaceSize) + value + NEW_LINE;
        return formatted;
    }
    std::string presentPrice(double price) const
    { return getFormattedNumberAsString(price, 2); }


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
