#pragma once
#include <string>
#include "core/Date.h"
#include "core/Money.h"
#include "menu/Dish.h"
#include "menu/Ingredient.h"
#include "menu/Inventory.h"
#include "orders/Bill.h"
#include "orders/Order.h"
#include "orders/Table.h"
#include "payroll/Payroll.h"
#include "payroll/Payslip.h"
#include "staff/Staff.h"
#include "storage/FileStore.h"
#include "storage/Repository.h"

namespace restaurant {
namespace ui {

// RestaurantSystem is the glue of the whole program.  It owns every repository
// (in memory) plus the file stores (on disk) and the shared Inventory/Payroll
// objects.  It is the single object the menus talk to.
class RestaurantSystem {
public:
    explicit RestaurantSystem(const std::string& dataDir);
    ~RestaurantSystem();

    void run();  // login screen + role menu loop

    storage::Repository<staff::Staff>& staff();
    storage::Repository<menu::Dish>& dishes();
    storage::Repository<menu::Ingredient>& ingredients();
    storage::Repository<orders::Order>& orders();
    storage::Repository<orders::Table>& tables();
    storage::Repository<orders::Bill>& bills();
    storage::Repository<payroll::Payslip>& payslips();

    menu::Inventory& inventory();
    payroll::Payroll& payroll();

    core::Date today() const;

    // Persistence
    void loadAll();
    void saveAll();
    void seedIfEmpty();

private:
    std::string path(const std::string& file) const;
    void ensureDataDir() const;

    std::string dataDir_;

    storage::Repository<staff::Staff> staffRepo_;
    storage::Repository<menu::Dish> dishRepo_;
    storage::Repository<menu::Ingredient> ingredientRepo_;
    storage::Repository<orders::Order> orderRepo_;
    storage::Repository<orders::Table> tableRepo_;
    storage::Repository<orders::Bill> billRepo_;
    storage::Repository<payroll::Payslip> payslipRepo_;

    menu::Inventory inventory_;
    payroll::Payroll payroll_;

    storage::FileStore<staff::Staff> staffStore_;
    storage::FileStore<menu::Dish> dishStore_;
    storage::FileStore<menu::Ingredient> ingredientStore_;
    storage::FileStore<orders::Order> orderStore_;
    storage::FileStore<orders::Table> tableStore_;
    storage::FileStore<orders::Bill> billStore_;
    storage::FileStore<payroll::Payslip> payslipStore_;
};

} // namespace ui
} // namespace restaurant
