#pragma once
#include <vector>
#include <string>
#include "menu/Ingredient.h"
#include "menu/Dish.h"
#include "storage/Repository.h"

namespace restaurant {
namespace menu {

// Inventory holds every ingredient.  When an order is placed it checks and
// reduces stock for every dish.  It uses a Repository<Ingredient>.
class Inventory {
public:
    explicit Inventory(storage::Repository<Ingredient>* repository);

    storage::Repository<Ingredient>& ingredients();

    Ingredient* findIngredient(int id) const;
    Ingredient* findByName(const std::string& name) const;

    void addIngredient(Ingredient* ingredient);

    // Throws InsufficientStockException if any ingredient of the dish is short.
    // "times" is how many of the dish are being prepared.
    void checkDish(const Dish& dish, int times = 1) const;
    // Consumes the ingredients needed by the dish.
    void consumeDish(const Dish& dish, int times = 1);

    std::vector<Ingredient*> lowStock() const;
    void listAll(std::ostream& os) const;

private:
    storage::Repository<Ingredient>* repo_;
};

} // namespace menu
} // namespace restaurant
