#include "ui/RestaurantSystem.h"
#include "ui/Auth.h"
#include "ui/Console.h"
#include "ui/Menus.h"
#include "staff/Manager.h"
#include "staff/Chef.h"
#include "staff/Waiter.h"
#include "staff/Cashier.h"
#include "staff/Cleaner.h"
#include "staff/HeadChef.h"
#include "staff/ShiftSupervisor.h"
#include "storage/StaffFactory.h"
#include "core/IdGenerator.h"
#include "core/Tags.h"
#include "core/Exceptions.h"

#include <iostream>
#include <vector>
#include <ctime>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

namespace restaurant {
namespace ui {

RestaurantSystem::RestaurantSystem(const std::string& dataDir)
    : dataDir_(dataDir),
      staffRepo_(), dishRepo_(), ingredientRepo_(), orderRepo_(), tableRepo_(),
      billRepo_(), payslipRepo_(),
      inventory_(&ingredientRepo_),
      payroll_(),
      staffStore_(dataDir + "/staff.dat",
                  storage::FileStore<staff::Staff>::TagFn(
                      [](const staff::Staff& s) { return s.typeTag(); }),
                  storage::FileStore<staff::Staff>::LoadFn(
                      [](int tag) { return storage::StaffFactory::create(tag); })),
      dishStore_(dataDir + "/dishes.dat",
                 storage::FileStore<menu::Dish>::TagFn(),
                 storage::FileStore<menu::Dish>::LoadFn(
                     [](int) { return new menu::Dish(); })),
      ingredientStore_(dataDir + "/ingredients.dat",
                 storage::FileStore<menu::Ingredient>::TagFn(),
                 storage::FileStore<menu::Ingredient>::LoadFn(
                     [](int) { return new menu::Ingredient(); })),
      orderStore_(dataDir + "/orders.dat",
                 storage::FileStore<orders::Order>::TagFn(),
                 storage::FileStore<orders::Order>::LoadFn(
                     [](int) { return new orders::Order(); })),
      tableStore_(dataDir + "/tables.dat",
                 storage::FileStore<orders::Table>::TagFn(),
                 storage::FileStore<orders::Table>::LoadFn(
                     [](int) { return new orders::Table(); })),
      billStore_(dataDir + "/bills.dat",
                 storage::FileStore<orders::Bill>::TagFn(),
                 storage::FileStore<orders::Bill>::LoadFn(
                     [](int) { return new orders::Bill(); })),
      payslipStore_(dataDir + "/payslips.dat",
                 storage::FileStore<payroll::Payslip>::TagFn(),
                 storage::FileStore<payroll::Payslip>::LoadFn(
                     [](int) { return new payroll::Payslip(); })) {
    ensureDataDir();
}

RestaurantSystem::~RestaurantSystem() {}

std::string RestaurantSystem::path(const std::string& file) const {
    return dataDir_ + "/" + file;
}

void RestaurantSystem::ensureDataDir() const {
#ifdef _WIN32
    _mkdir(dataDir_.c_str());
#else
    mkdir(dataDir_.c_str(), 0755);
#endif
}

storage::Repository<staff::Staff>& RestaurantSystem::staff() { return staffRepo_; }
storage::Repository<menu::Dish>& RestaurantSystem::dishes() { return dishRepo_; }
storage::Repository<menu::Ingredient>& RestaurantSystem::ingredients() { return ingredientRepo_; }
storage::Repository<orders::Order>& RestaurantSystem::orders() { return orderRepo_; }
storage::Repository<orders::Table>& RestaurantSystem::tables() { return tableRepo_; }
storage::Repository<orders::Bill>& RestaurantSystem::bills() { return billRepo_; }
storage::Repository<payroll::Payslip>& RestaurantSystem::payslips() { return payslipRepo_; }

menu::Inventory& RestaurantSystem::inventory() { return inventory_; }
payroll::Payroll& RestaurantSystem::payroll() { return payroll_; }

core::Date RestaurantSystem::today() const {
    std::time_t t = std::time(0);
    std::tm* local = std::localtime(&t);
    if (!local) return core::Date(1, 1, 2000);
    return core::Date(local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

void RestaurantSystem::loadAll() {
    staffStore_.rebuildIndex();
    dishStore_.rebuildIndex();
    ingredientStore_.rebuildIndex();
    orderStore_.rebuildIndex();
    tableStore_.rebuildIndex();
    billStore_.rebuildIndex();
    payslipStore_.rebuildIndex();

    staffRepo_.clear();
    dishRepo_.clear();
    ingredientRepo_.clear();
    orderRepo_.clear();
    tableRepo_.clear();
    billRepo_.clear();
    payslipRepo_.clear();

    std::vector<staff::Staff*> st = staffStore_.loadAll();
    for (std::size_t i = 0; i < st.size(); ++i) staffRepo_.add(st[i]);

    std::vector<menu::Dish*> ds = dishStore_.loadAll();
    for (std::size_t i = 0; i < ds.size(); ++i) dishRepo_.add(ds[i]);

    std::vector<menu::Ingredient*> ig = ingredientStore_.loadAll();
    for (std::size_t i = 0; i < ig.size(); ++i) ingredientRepo_.add(ig[i]);

    std::vector<orders::Order*> ors = orderStore_.loadAll();
    for (std::size_t i = 0; i < ors.size(); ++i) orderRepo_.add(ors[i]);

    std::vector<orders::Table*> tb = tableStore_.loadAll();
    for (std::size_t i = 0; i < tb.size(); ++i) tableRepo_.add(tb[i]);

    std::vector<orders::Bill*> bl = billStore_.loadAll();
    for (std::size_t i = 0; i < bl.size(); ++i) billRepo_.add(bl[i]);

    std::vector<payroll::Payslip*> ps = payslipStore_.loadAll();
    for (std::size_t i = 0; i < ps.size(); ++i) payslipRepo_.add(ps[i]);

    // Make sure freshly generated IDs never clash with loaded ones.
    int maxId = 0;
    for (std::size_t i = 0; i < staffRepo_.size(); ++i) if (staffRepo_.at(i)->id() > maxId) maxId = staffRepo_.at(i)->id();
    core::IdGenerator::seedStaff(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < dishRepo_.size(); ++i) if (dishRepo_.at(i)->id() > maxId) maxId = dishRepo_.at(i)->id();
    core::IdGenerator::seedDish(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < ingredientRepo_.size(); ++i) if (ingredientRepo_.at(i)->id() > maxId) maxId = ingredientRepo_.at(i)->id();
    core::IdGenerator::seedIngredient(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < orderRepo_.size(); ++i) if (orderRepo_.at(i)->id() > maxId) maxId = orderRepo_.at(i)->id();
    core::IdGenerator::seedOrder(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < tableRepo_.size(); ++i) if (tableRepo_.at(i)->id() > maxId) maxId = tableRepo_.at(i)->id();
    core::IdGenerator::seedTable(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < billRepo_.size(); ++i) if (billRepo_.at(i)->id() > maxId) maxId = billRepo_.at(i)->id();
    core::IdGenerator::seedBill(maxId + 1);
    maxId = 0;
    for (std::size_t i = 0; i < payslipRepo_.size(); ++i) if (payslipRepo_.at(i)->id() > maxId) maxId = payslipRepo_.at(i)->id();
    core::IdGenerator::seedPayslip(maxId + 1);
}

void RestaurantSystem::saveAll() {
    staffStore_.reset();
    for (std::size_t i = 0; i < staffRepo_.size(); ++i) staffStore_.save(*staffRepo_.at(i));

    dishStore_.reset();
    for (std::size_t i = 0; i < dishRepo_.size(); ++i) dishStore_.save(*dishRepo_.at(i));

    ingredientStore_.reset();
    for (std::size_t i = 0; i < ingredientRepo_.size(); ++i) ingredientStore_.save(*ingredientRepo_.at(i));

    orderStore_.reset();
    for (std::size_t i = 0; i < orderRepo_.size(); ++i) orderStore_.save(*orderRepo_.at(i));

    tableStore_.reset();
    for (std::size_t i = 0; i < tableRepo_.size(); ++i) tableStore_.save(*tableRepo_.at(i));

    billStore_.reset();
    for (std::size_t i = 0; i < billRepo_.size(); ++i) billStore_.save(*billRepo_.at(i));

    payslipStore_.reset();
    for (std::size_t i = 0; i < payslipRepo_.size(); ++i) payslipStore_.save(*payslipRepo_.at(i));
}

void RestaurantSystem::seedIfEmpty() {
    if (staffRepo_.size() > 0) return;

    core::Date join(1, 1, 2024);

    staff::Manager* mgr = new staff::Manager(core::IdGenerator::nextStaffId(),
        "Alice Manager", "9000000001", "Head Office", join,
        core::Money::fromRupees(60000), core::Money::fromRupees(5000));
    mgr->setPassword("manager");
    staffRepo_.add(mgr);

    staff::Cashier* cash = new staff::Cashier(core::IdGenerator::nextStaffId(),
        "Carl Cashier", "9000000002", "Counter", join,
        core::Money::fromRupees(30000), core::Money::fromRupees(150));
    cash->setPassword("cashier");
    staffRepo_.add(cash);

    staff::Waiter* wait = new staff::Waiter(core::IdGenerator::nextStaffId(),
        "Wendy Waiter", "9000000003", "Floor", join,
        core::Money::fromRupees(20000));
    wait->setPassword("waiter");
    staffRepo_.add(wait);

    staff::Chef* chef = new staff::Chef(core::IdGenerator::nextStaffId(),
        "Charlie Chef", "9000000004", "Kitchen", join,
        core::Money::fromRupees(35000), core::Money::fromRupees(25));
    chef->setPassword("chef");
    staffRepo_.add(chef);

    staff::Cleaner* clean = new staff::Cleaner(core::IdGenerator::nextStaffId(),
        "Cindy Cleaner", "9000000005", "Floor", join,
        core::Money(0), core::Money::fromRupees(100));
    clean->setPassword("cleaner");
    clean->addSchedule("Clean dining tables (morning)");
    clean->addSchedule("Clean kitchen (evening)");
    staffRepo_.add(clean);

    staff::HeadChef* head = new staff::HeadChef(core::IdGenerator::nextStaffId(),
        "Henry HeadChef", "9000000006", "Kitchen", join,
        core::Money::fromRupees(50000), core::Money::fromRupees(8000),
        core::Money::fromRupees(30));
    head->setPassword("headchef");
    staffRepo_.add(head);

    staff::ShiftSupervisor* sup = new staff::ShiftSupervisor(core::IdGenerator::nextStaffId(),
        "Sam Supervisor", "9000000007", "Floor", join,
        core::Money::fromRupees(28000), core::Money::fromRupees(170));
    sup->setPassword("supervisor");
    staffRepo_.add(sup);

    menu::Ingredient* rice = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Rice", "kg", 50, 10);
    menu::Ingredient* chicken = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Chicken", "kg", 30, 8);
    menu::Ingredient* tomato = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Tomato", "kg", 20, 5);
    menu::Ingredient* oil = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Oil", "litre", 15, 4);
    menu::Ingredient* paneer = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Paneer", "kg", 10, 3);
    menu::Ingredient* milk = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Milk", "litre", 20, 5);
    menu::Ingredient* sugar = new menu::Ingredient(core::IdGenerator::nextIngredientId(), "Sugar", "kg", 25, 5);
    ingredientRepo_.add(rice);
    ingredientRepo_.add(chicken);
    ingredientRepo_.add(tomato);
    ingredientRepo_.add(oil);
    ingredientRepo_.add(paneer);
    ingredientRepo_.add(milk);
    ingredientRepo_.add(sugar);

    menu::Dish* d1 = new menu::Dish(core::IdGenerator::nextDishId(), "Veg Fried Rice",
        core::Money::fromRupees(180), "Main");
    d1->addIngredient(rice->id(), 0.30);
    d1->addIngredient(oil->id(), 0.05);
    d1->addIngredient(tomato->id(), 0.10);
    dishRepo_.add(d1);

    menu::Dish* d2 = new menu::Dish(core::IdGenerator::nextDishId(), "Chicken Curry",
        core::Money::fromRupees(320), "Main");
    d2->addIngredient(chicken->id(), 0.40);
    d2->addIngredient(tomato->id(), 0.15);
    d2->addIngredient(oil->id(), 0.05);
    dishRepo_.add(d2);

    menu::Dish* d3 = new menu::Dish(core::IdGenerator::nextDishId(), "Paneer Butter Masala",
        core::Money::fromRupees(300), "Main");
    d3->addIngredient(paneer->id(), 0.25);
    d3->addIngredient(tomato->id(), 0.20);
    d3->addIngredient(oil->id(), 0.06);
    dishRepo_.add(d3);

    menu::Dish* d4 = new menu::Dish(core::IdGenerator::nextDishId(), "Masala Chai",
        core::Money::fromRupees(40), "Beverage");
    d4->addIngredient(milk->id(), 0.15);
    d4->addIngredient(sugar->id(), 0.02);
    dishRepo_.add(d4);

    menu::Dish* d5 = new menu::Dish(core::IdGenerator::nextDishId(), "Gulab Jamun",
        core::Money::fromRupees(90), "Dessert");
    d5->addIngredient(milk->id(), 0.20);
    d5->addIngredient(sugar->id(), 0.15);
    dishRepo_.add(d5);

    for (int i = 1; i <= 6; ++i)
        tableRepo_.add(new orders::Table(i, 4));

    saveAll();
    std::cout << "[setup] Seeded demo staff, menu, ingredients and tables.\n";
}

void RestaurantSystem::run() {
    loadAll();
    seedIfEmpty();

    std::cout << "\n";
    std::cout << "=================================================\n";
    std::cout << "        RESTAURANT MANAGEMENT SYSTEM\n";
    std::cout << "     (C++ OOP project - Modules 1 to 4)\n";
    std::cout << "=================================================\n";
    std::cout << "Default logins (id / password):\n";
    std::cout << "  1 manager   2 cashier   3 waiter\n";
    std::cout << "  4 chef      5 cleaner   6 headchef   7 supervisor\n";

    while (true) {
        std::cout << "\n-------------------------------------------------\n";
        int id = readInt("Login ID (0 to quit): ");
        if (id == 0) break;
        std::string password = readLine("Password: ");
        try {
            staff::Staff* user = Auth::login(staffRepo_, id, password);
            std::cout << "\nWelcome " << user->name() << " [" << user->roleName() << "]\n";
            switch (user->typeTag()) {
                case core::TAG_MANAGER:
                case core::TAG_HEAD_CHEF:
                    ManagerMenu::run(*this, *user);
                    break;
                case core::TAG_CASHIER:
                case core::TAG_SHIFT_SUPERVISOR:
                    CashierMenu::run(*this, *user);
                    break;
                case core::TAG_WAITER:
                    WaiterMenu::run(*this, *user);
                    break;
                case core::TAG_CHEF:
                    ChefMenu::run(*this, *user);
                    break;
                case core::TAG_CLEANER:
                    CleanerMenu::run(*this, *user);
                    break;
                default:
                    std::cout << "This role has no menu yet.\n";
                    break;
            }
            saveAll();
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
    }

    saveAll();
    std::cout << "\nData saved. Good bye!\n";
}

} // namespace ui
} // namespace restaurant
