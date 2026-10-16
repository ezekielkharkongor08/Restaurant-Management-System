#include "menu/Dish.h"
#include "menu/Inventory.h"
#include "core/Exceptions.h"
#include "core/Serialization.h"

namespace restaurant {
namespace menu {

Dish::Dish() : id_(0), name_(""), price_(0), category_("") {}

Dish::Dish(int id, const std::string& name, const core::Money& price,
           const std::string& category)
    : id_(id), name_(name), price_(price), category_(category) {
    if (price < core::Money(0))
        throw core::InvalidInputException("Dish price cannot be negative");
}

int Dish::id() const { return id_; }
const std::string& Dish::name() const { return name_; }
core::Money Dish::price() const { return price_; }
const std::string& Dish::category() const { return category_; }

void Dish::setName(const std::string& name) { name_ = name; }
void Dish::setPrice(const core::Money& price) { price_ = price; }
void Dish::setCategory(const std::string& category) { category_ = category; }

void Dish::addIngredient(int ingredientId, double quantity) {
    recipe_.push_back(RecipeLine(ingredientId, quantity));
}

const std::vector<RecipeLine>& Dish::recipe() const { return recipe_; }

bool Dish::isAvailable(const Inventory& inventory) const {
    for (std::size_t i = 0; i < recipe_.size(); ++i) {
        const Ingredient* ing =
            inventory.findIngredient(recipe_[i].ingredientId);
        if (!ing) return false;
        if (ing->quantity() < recipe_[i].quantity) return false;
    }
    return true;
}

Dish* Dish::clone() const { return new Dish(*this); }

void Dish::print(std::ostream& os) const {
    os << "#" << id_ << "  " << name_ << "  [" << category_ << "]  "
       << price_ << "  (needs " << recipe_.size() << " ingredient(s))";
}

std::ostream& operator<<(std::ostream& os, const Dish& dish) {
    dish.print(os);
    return os;
}

void Dish::write(std::ostream& os) const {
    core::writeValue(os, id_);
    core::writeString(os, name_);
    price_.write(os);
    core::writeString(os, category_);
    std::uint64_t n = recipe_.size();
    core::writeValue(os, n);
    for (std::size_t i = 0; i < recipe_.size(); ++i) {
        core::writeValue(os, recipe_[i].ingredientId);
        core::writeValue(os, recipe_[i].quantity);
    }
}

void Dish::read(std::istream& is) {
    core::readValue(is, id_);
    core::readString(is, name_);
    price_.read(is);
    core::readString(is, category_);
    std::uint64_t n = 0;
    core::readValue(is, n);
    recipe_.clear();
    for (std::uint64_t i = 0; i < n; ++i) {
        RecipeLine line;
        core::readValue(is, line.ingredientId);
        core::readValue(is, line.quantity);
        recipe_.push_back(line);
    }
}

} // namespace menu
} // namespace restaurant
