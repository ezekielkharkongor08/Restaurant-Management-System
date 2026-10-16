#include "orders/OrderItem.h"
#include "core/Serialization.h"

namespace restaurant {
namespace orders {

OrderItem::OrderItem()
    : dishId_(0), dishName_(""), quantity_(0), unitPrice_(0) {}

OrderItem::OrderItem(int dishId, const std::string& dishName, int quantity,
                     const core::Money& unitPrice)
    : dishId_(dishId), dishName_(dishName), quantity_(quantity), unitPrice_(unitPrice) {}

OrderItem::OrderItem(const OrderItem& other)
    : dishId_(other.dishId_), dishName_(other.dishName_),
      quantity_(other.quantity_), unitPrice_(other.unitPrice_) {}

OrderItem& OrderItem::operator=(const OrderItem& other) {
    if (this != &other) {
        dishId_ = other.dishId_;
        dishName_ = other.dishName_;
        quantity_ = other.quantity_;
        unitPrice_ = other.unitPrice_;
    }
    return *this;
}

OrderItem::~OrderItem() {}

int OrderItem::dishId() const { return dishId_; }
const std::string& OrderItem::dishName() const { return dishName_; }
int OrderItem::quantity() const { return quantity_; }
core::Money OrderItem::unitPrice() const { return unitPrice_; }

void OrderItem::setQuantity(int quantity) { quantity_ = quantity; }
void OrderItem::setUnitPrice(const core::Money& price) { unitPrice_ = price; }

core::Money OrderItem::lineTotal() const {
    return unitPrice_ * static_cast<double>(quantity_);
}

void OrderItem::write(std::ostream& os) const {
    core::writeValue(os, dishId_);
    core::writeString(os, dishName_);
    core::writeValue(os, quantity_);
    unitPrice_.write(os);
}

void OrderItem::read(std::istream& is) {
    core::readValue(is, dishId_);
    core::readString(is, dishName_);
    core::readValue(is, quantity_);
    unitPrice_.read(is);
}

std::ostream& operator<<(std::ostream& os, const OrderItem& item) {
    os << item.quantity() << " x " << item.dishName() << " @ " << item.unitPrice()
       << " = " << item.lineTotal();
    return os;
}

} // namespace orders
} // namespace restaurant
