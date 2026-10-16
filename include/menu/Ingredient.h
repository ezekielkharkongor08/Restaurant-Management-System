#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "interfaces/Interfaces.h"

namespace restaurant {
namespace menu {

// Ingredient is one raw material kept in stock (rice, oil, chicken ...).
// consume() throws InsufficientStockException when the stock is too low.
class Ingredient : public interfaces::Printable, public interfaces::Serializable {
public:
    Ingredient();
    Ingredient(int id, const std::string& name, const std::string& unit,
               double quantity, double minLevel);

    int id() const;
    const std::string& name() const;
    const std::string& unit() const;
    double quantity() const;
    double minLevel() const;

    void setName(const std::string& name);
    void setUnit(const std::string& unit);
    void setQuantity(double quantity);
    void setMinLevel(double minLevel);

    void restock(double amount);
    void consume(double amount);   // throws InsufficientStockException

    bool isLow() const;

    Ingredient* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int id_;
    std::string name_;
    std::string unit_;
    double quantity_;
    double minLevel_;
};

std::ostream& operator<<(std::ostream& os, const Ingredient& ingredient);

} // namespace menu
} // namespace restaurant
