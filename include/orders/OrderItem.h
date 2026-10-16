#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "core/Money.h"

namespace restaurant {
namespace orders {

// OrderItem is one line in an order: which dish, how many and the unit price.
// It has an explicit copy constructor (a requirement of the syllabus).
class OrderItem {
public:
    OrderItem();
    OrderItem(int dishId, const std::string& dishName, int quantity,
              const core::Money& unitPrice);
    OrderItem(const OrderItem& other);              // copy constructor
    OrderItem& operator=(const OrderItem& other);   // assignment
    ~OrderItem();

    int dishId() const;
    const std::string& dishName() const;
    int quantity() const;
    core::Money unitPrice() const;

    void setQuantity(int quantity);
    void setUnitPrice(const core::Money& price);

    core::Money lineTotal() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int dishId_;
    std::string dishName_;
    int quantity_;
    core::Money unitPrice_;
};

std::ostream& operator<<(std::ostream& os, const OrderItem& item);

} // namespace orders
} // namespace restaurant
