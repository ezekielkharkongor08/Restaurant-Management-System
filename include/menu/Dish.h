#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "interfaces/Interfaces.h"

namespace restaurant {

namespace menu {
class Inventory;
}

namespace menu {

// One line of a dish recipe: which ingredient and how much of it is used.
struct RecipeLine {
    int ingredientId;
    double quantity;
    RecipeLine() : ingredientId(0), quantity(0.0) {}
    RecipeLine(int id, double qty) : ingredientId(id), quantity(qty) {}
};

// Dish is one item on the menu: name, price, category and the ingredients it
// needs.  isAvailable() checks the inventory before an order is accepted.
class Dish : public interfaces::Printable, public interfaces::Serializable {
public:
    Dish();
    Dish(int id, const std::string& name, const core::Money& price,
         const std::string& category);

    int id() const;
    const std::string& name() const;
    core::Money price() const;
    const std::string& category() const;

    void setName(const std::string& name);
    void setPrice(const core::Money& price);
    void setCategory(const std::string& category);

    void addIngredient(int ingredientId, double quantity);
    const std::vector<RecipeLine>& recipe() const;

    bool isAvailable(const Inventory& inventory) const;

    Dish* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int id_;
    std::string name_;
    core::Money price_;
    std::string category_;
    std::vector<RecipeLine> recipe_;
};

std::ostream& operator<<(std::ostream& os, const Dish& dish);

} // namespace menu
} // namespace restaurant
