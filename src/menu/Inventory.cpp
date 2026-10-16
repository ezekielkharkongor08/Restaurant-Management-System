#include "menu/Inventory.h"
#include "core/Exceptions.h"

namespace restaurant {
namespace menu {

Inventory::Inventory(storage::Repository<Ingredient>* repository)
    : repo_(repository) {}

storage::Repository<Ingredient>& Inventory::ingredients() { return *repo_; }

Ingredient* Inventory::findIngredient(int id) const {
    return repo_->findOrNull(id);
}

Ingredient* Inventory::findByName(const std::string& name) const {
    for (std::size_t i = 0; i < repo_->size(); ++i)
        if (repo_->at(i)->name() == name) return repo_->at(i);
    return 0;
}

void Inventory::addIngredient(Ingredient* ingredient) { repo_->add(ingredient); }

void Inventory::checkDish(const Dish& dish, int times) const {
    if (times < 1) return;
    const std::vector<RecipeLine>& recipe = dish.recipe();
    for (std::size_t i = 0; i < recipe.size(); ++i) {
        Ingredient* ing = findIngredient(recipe[i].ingredientId);
        if (!ing)
            throw core::InsufficientStockException(
                "Dish " + dish.name() + " needs an ingredient that is missing");
        if (ing->quantity() < recipe[i].quantity * times)
            throw core::InsufficientStockException(
                "Not enough " + ing->name() + " for " + dish.name());
    }
}

void Inventory::consumeDish(const Dish& dish, int times) {
    if (times < 1) return;
    checkDish(dish, times);  // throws if anything is short, so stock is never half-used
    const std::vector<RecipeLine>& recipe = dish.recipe();
    for (std::size_t i = 0; i < recipe.size(); ++i)
        findIngredient(recipe[i].ingredientId)->consume(recipe[i].quantity * times);
}

std::vector<Ingredient*> Inventory::lowStock() const {
    std::vector<Ingredient*> out;
    for (std::size_t i = 0; i < repo_->size(); ++i)
        if (repo_->at(i)->isLow()) out.push_back(repo_->at(i));
    return out;
}

void Inventory::listAll(std::ostream& os) const {
    for (std::size_t i = 0; i < repo_->size(); ++i) {
        repo_->at(i)->print(os);
        os << "\n";
    }
}

} // namespace menu
} // namespace restaurant
