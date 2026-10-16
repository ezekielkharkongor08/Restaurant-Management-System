#include "menu/Ingredient.h"
#include "core/Exceptions.h"
#include "core/Serialization.h"

namespace restaurant {
namespace menu {

Ingredient::Ingredient()
    : id_(0), name_(""), unit_("kg"), quantity_(0.0), minLevel_(0.0) {}

Ingredient::Ingredient(int id, const std::string& name, const std::string& unit,
                       double quantity, double minLevel)
    : id_(id), name_(name), unit_(unit), quantity_(quantity), minLevel_(minLevel) {
    if (quantity < 0 || minLevel < 0)
        throw core::InvalidInputException("Ingredient quantity cannot be negative");
}

int Ingredient::id() const { return id_; }
const std::string& Ingredient::name() const { return name_; }
const std::string& Ingredient::unit() const { return unit_; }
double Ingredient::quantity() const { return quantity_; }
double Ingredient::minLevel() const { return minLevel_; }

void Ingredient::setName(const std::string& name) { name_ = name; }
void Ingredient::setUnit(const std::string& unit) { unit_ = unit; }
void Ingredient::setQuantity(double quantity) { quantity_ = quantity; }
void Ingredient::setMinLevel(double minLevel) { minLevel_ = minLevel; }

void Ingredient::restock(double amount) {
    if (amount < 0)
        throw core::InvalidInputException("Restock amount cannot be negative");
    quantity_ += amount;
}

void Ingredient::consume(double amount) {
    if (amount < 0)
        throw core::InvalidInputException("Consume amount cannot be negative");
    if (quantity_ < amount)
        throw core::InsufficientStockException(
            "Not enough " + name_ + " in stock (need " +
            std::to_string(amount) + ", have " + std::to_string(quantity_) + ")");
    quantity_ -= amount;
}

bool Ingredient::isLow() const { return quantity_ <= minLevel_; }

Ingredient* Ingredient::clone() const { return new Ingredient(*this); }

void Ingredient::print(std::ostream& os) const {
    os << "#" << id_ << "  " << name_ << "  stock=" << quantity_ << " " << unit_
       << "  min=" << minLevel_ << (isLow() ? "  [LOW]" : "");
}

std::ostream& operator<<(std::ostream& os, const Ingredient& ingredient) {
    ingredient.print(os);
    return os;
}

void Ingredient::write(std::ostream& os) const {
    core::writeValue(os, id_);
    core::writeString(os, name_);
    core::writeString(os, unit_);
    core::writeValue(os, quantity_);
    core::writeValue(os, minLevel_);
}

void Ingredient::read(std::istream& is) {
    core::readValue(is, id_);
    core::readString(is, name_);
    core::readString(is, unit_);
    core::readValue(is, quantity_);
    core::readValue(is, minLevel_);
}

} // namespace menu
} // namespace restaurant
