#include "ui/Menus.h"
#include "ui/RestaurantSystem.h"
#include "ui/Console.h"
#include "staff/Manager.h"
#include "staff/Chef.h"
#include "staff/Waiter.h"
#include "staff/Cashier.h"
#include "staff/Cleaner.h"
#include "staff/HeadChef.h"
#include "staff/ShiftSupervisor.h"
#include "core/IdGenerator.h"
#include "core/Exceptions.h"
#include "core/Tags.h"
#include "storage/Algorithms.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>

namespace restaurant {
namespace ui {

namespace {

// ---------------------------------------------------------------- staff ----

void listStaff(RestaurantSystem& sys) {
    std::cout << "\n--- Staff (" << sys.staff().size() << ") ---\n";
    for (std::size_t i = 0; i < sys.staff().size(); ++i) {
        staff::Staff* s = sys.staff().at(i);
        std::cout << *s << "  pay=" << s->calculatePay() << "\n";
    }
}

staff::Staff* buildStaff(RestaurantSystem& sys) {
    std::cout << "\nRole: 1 Manager  2 Chef  3 Waiter  4 Cashier  5 Cleaner"
                 "  6 HeadChef  7 ShiftSupervisor\n";
    int role = readInt("Role number: ");
    std::string name = readLine("Name: ");
    std::string phone = readLine("Phone: ");
    std::string address = readLine("Address: ");
    core::Money base = readMoney("Base salary (Rs): ");
    std::string password = readLine("Login password: ");

    int id = core::IdGenerator::nextStaffId();
    core::Date join = sys.today();

    if (role == 1) {
        core::Money allowance = readMoney("Fixed allowance (Rs): ");
        staff::Manager* s = new staff::Manager(id, name, phone, address, join, base, allowance);
        s->setPassword(password);
        return s;
    } else if (role == 2) {
        core::Money rate = readMoney("Rate per dish cooked (Rs): ");
        staff::Chef* s = new staff::Chef(id, name, phone, address, join, base, rate);
        s->setPassword(password);
        return s;
    } else if (role == 3) {
        staff::Waiter* s = new staff::Waiter(id, name, phone, address, join, base);
        s->setPassword(password);
        return s;
    } else if (role == 4) {
        core::Money overtime = readMoney("Overtime rate per hour (Rs): ");
        staff::Cashier* s = new staff::Cashier(id, name, phone, address, join, base, overtime);
        s->setPassword(password);
        return s;
    } else if (role == 5) {
        core::Money wage = readMoney("Hourly wage (Rs): ");
        staff::Cleaner* s = new staff::Cleaner(id, name, phone, address, join, base, wage);
        s->setPassword(password);
        return s;
    } else if (role == 6) {
        core::Money allowance = readMoney("Fixed allowance (Rs): ");
        core::Money rate = readMoney("Rate per dish cooked (Rs): ");
        staff::HeadChef* s = new staff::HeadChef(id, name, phone, address, join, base, allowance, rate);
        s->setPassword(password);
        return s;
    } else if (role == 7) {
        core::Money overtime = readMoney("Overtime rate per hour (Rs): ");
        staff::ShiftSupervisor* s = new staff::ShiftSupervisor(id, name, phone, address, join, base, overtime);
        s->setPassword(password);
        return s;
    }

    throw core::InvalidInputException("Unknown role number");
}

void hireStaff(RestaurantSystem& sys) {
    staff::Staff* s = buildStaff(sys);
    sys.staff().add(s);
    sys.saveAll();
    std::cout << "  Hired " << s->name() << " as " << s->roleName()
              << " (id " << s->id() << ").\n";
}

void fireStaff(RestaurantSystem& sys, staff::Staff& user) {
    listStaff(sys);
    int id = readInt("ID of staff to remove: ");
    if (id == user.id()) throw core::UnauthorizedAccessException("You cannot fire yourself");
    if (!sys.staff().remove(id)) throw core::StaffNotFoundException("No staff with that ID");
    sys.saveAll();
    std::cout << "  Staff removed.\n";
}

void markAttendance(RestaurantSystem& sys) {
    listStaff(sys);
    int id = readInt("Staff ID: ");
    staff::Staff* s = sys.staff().find(id);
    std::cout << "1 Present  2 Absent  3 Leave  4 Add overtime\n";
    int choice = readInt("Choice: ");
    core::Date today = sys.today();
    if (choice == 1) {
        s->attendance().markPresent(today);
        std::cout << "  Marked present.\n";
    } else if (choice == 2) {
        s->attendance().markAbsent(today);
        std::cout << "  Marked absent.\n";
    } else if (choice == 3) {
        s->attendance().markLeave(today);
        std::cout << "  Marked leave.\n";
    } else if (choice == 4) {
        double hours = readDouble("Overtime hours: ");
        s->attendance().addOvertime(hours);
        std::cout << "  Overtime added.\n";
    } else {
        throw core::InvalidInputException("Unknown attendance choice");
    }
    sys.saveAll();
}

// ----------------------------------------------------------------- menu ----

void listDishes(RestaurantSystem& sys) {
    std::cout << "\n--- Menu (" << sys.dishes().size() << ") ---\n";
    for (std::size_t i = 0; i < sys.dishes().size(); ++i) {
        menu::Dish* d = sys.dishes().at(i);
        std::cout << *d;
        if (!d->isAvailable(sys.inventory())) std::cout << "  [not available]";
        std::cout << "\n";
    }
}

void listIngredients(RestaurantSystem& sys) {
    std::cout << "\n--- Ingredients (" << sys.ingredients().size() << ") ---\n";
    sys.inventory().listAll(std::cout);
}

void addDish(RestaurantSystem& sys) {
    std::string name = readLine("Dish name: ");
    core::Money price = readMoney("Price (Rs): ");
    std::string category = readLine("Category (Main/Beverage/Dessert): ");
    menu::Dish* d = new menu::Dish(core::IdGenerator::nextDishId(), name, price, category);

    listIngredients(sys);
    while (true) {
        int ingId = readInt("Add ingredient id (0 when done): ");
        if (ingId == 0) break;
        if (!sys.inventory().findIngredient(ingId)) {
            std::cout << "  ! No ingredient with that id.\n";
            continue;
        }
        double qty = readDouble("Quantity used per dish: ");
        d->addIngredient(ingId, qty);
    }
    sys.dishes().add(d);
    sys.saveAll();
    std::cout << "  Dish added (id " << d->id() << ").\n";
}

void updateDish(RestaurantSystem& sys) {
    listDishes(sys);
    int id = readInt("Dish id to update: ");
    menu::Dish* d = sys.dishes().find(id);
    std::cout << "1 Name  2 Price  3 Category\n";
    int choice = readInt("Choice: ");
    if (choice == 1) d->setName(readLine("New name: "));
    else if (choice == 2) d->setPrice(readMoney("New price (Rs): "));
    else if (choice == 3) d->setCategory(readLine("New category: "));
    else throw core::InvalidInputException("Unknown update choice");
    sys.saveAll();
    std::cout << "  Dish updated.\n";
}

void deleteDish(RestaurantSystem& sys) {
    listDishes(sys);
    int id = readInt("Dish id to delete: ");
    sys.dishes().find(id);  // throws if it does not exist
    sys.dishes().remove(id);
    sys.saveAll();
    std::cout << "  Dish deleted.\n";
}

void addIngredient(RestaurantSystem& sys) {
    std::string name = readLine("Ingredient name: ");
    std::string unit = readLine("Unit (kg/litre/piece): ");
    double qty = readDouble("Quantity in stock: ");
    double minLevel = readDouble("Minimum level: ");
    menu::Ingredient* ing =
        new menu::Ingredient(core::IdGenerator::nextIngredientId(), name, unit, qty, minLevel);
    sys.ingredients().add(ing);
    sys.saveAll();
    std::cout << "  Ingredient added (id " << ing->id() << ").\n";
}

void restockIngredient(RestaurantSystem& sys) {
    listIngredients(sys);
    int id = readInt("Ingredient id: ");
    menu::Ingredient* ing = sys.inventory().findIngredient(id);
    if (!ing) throw core::InvalidInputException("No ingredient with that id");
    double amount = readDouble("Amount to add: ");
    ing->restock(amount);
    sys.saveAll();
    std::cout << "  Restocked.\n";
}

// ---------------------------------------------------------------- orders ----

void listTables(RestaurantSystem& sys) {
    std::cout << "\n--- Tables (" << sys.tables().size() << ") ---\n";
    for (std::size_t i = 0; i < sys.tables().size(); ++i) {
        std::cout << *sys.tables().at(i) << "\n";
    }
}

void listOrders(RestaurantSystem& sys) {
    std::cout << "\n--- Orders (" << sys.orders().size() << ") ---\n";
    for (std::size_t i = 0; i < sys.orders().size(); ++i) {
        std::cout << *sys.orders().at(i) << "\n\n";
    }
}

void placeOrder(RestaurantSystem& sys, staff::Staff& waiter) {
    std::cout << "\nChoose a table:\n";
    listTables(sys);
    int tableNo = readInt("Table number: ");
    orders::Table* table = sys.tables().findOrNull(tableNo);
    if (!table) throw core::InvalidInputException("No such table");
    if (!table->isFree()) throw core::InvalidInputException("Table is not free");

    orders::Order* order = new orders::Order(core::IdGenerator::nextOrderId(),
                                             tableNo, waiter.id(), sys.today());

    while (true) {
        listDishes(sys);
        int dishId = readInt("Add dish id (0 to finish): ");
        if (dishId == 0) break;
        menu::Dish* dish = sys.dishes().findOrNull(dishId);
        if (!dish) {
            std::cout << "  ! No dish with that id.\n";
            continue;
        }
        int qty = readInt("Quantity: ");
        if (qty < 1) {
            std::cout << "  ! Quantity must be at least 1.\n";
            continue;
        }
        try {
            sys.inventory().checkDish(*dish, qty);
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
            continue;
        }
        *order += orders::OrderItem(dish->id(), dish->name(), qty, dish->price());
        std::cout << "  Added " << qty << " x " << dish->name() << "\n";
    }

    if (order->empty()) {
        delete order;
        std::cout << "  Empty order cancelled.\n";
        return;
    }

    // Now really take the ingredients out of stock.
    for (std::size_t i = 0; i < order->itemCount(); ++i) {
        const orders::OrderItem& item = (*order)[i];
        menu::Dish* dish = sys.dishes().find(item.dishId());
        sys.inventory().consumeDish(*dish, item.quantity());
    }

    table->setStatus(orders::TABLE_OCCUPIED);
    table->setWaiterId(waiter.id());
    sys.orders().add(order);
    waiter.attendance().addSales(order->total());
    if (staff::Waiter* w = dynamic_cast<staff::Waiter*>(&waiter)) w->addOrderTaken();
    sys.saveAll();
    std::cout << "\nOrder #" << order->id() << " placed for table " << tableNo
              << " (total " << order->total() << ").\n";
}

void listOpenOrders(RestaurantSystem& sys) {
    std::cout << "\n--- Open (unpaid) orders ---\n";
    for (std::size_t i = 0; i < sys.orders().size(); ++i) {
        orders::Order* o = sys.orders().at(i);
        if (o->status() != orders::ORDER_PAID) std::cout << *o << "\n\n";
    }
}

void createBill(RestaurantSystem& sys, staff::Staff& cashier) {
    listOpenOrders(sys);
    int orderId = readInt("Order id to bill: ");
    orders::Order* order = sys.orders().find(orderId);
    if (order->status() == orders::ORDER_PAID)
        throw core::InvalidInputException("That order is already paid");

    orders::Bill* bill = new orders::Bill(core::IdGenerator::nextBillId(), *order);
    bill->setCashierId(cashier.id());
    double discount = readDouble("Discount percent (0 for none): ");
    core::Money tip = readMoney("Tip (0 for none): ");
    bill->setDiscountPercent(discount);
    bill->setTip(tip);

    // The tip is passed to the waiter's pay data.
    if (order->waiterId() != 0) {
        staff::Staff* w = sys.staff().findOrNull(order->waiterId());
        if (w) w->attendance().addTips(tip);
    }

    bill->markPaid();
    order->setStatus(orders::ORDER_PAID);
    orders::Table* table = sys.tables().findOrNull(order->tableNumber());
    if (table) {
        table->setStatus(orders::TABLE_FREE);
        table->setWaiterId(0);
    }
    sys.bills().add(bill);
    if (staff::Cashier* c = dynamic_cast<staff::Cashier*>(&cashier)) c->addBillIssued();
    sys.saveAll();

    std::cout << "\n" << *bill;
}

// -------------------------------------------------------------- payroll ----

void runPayroll(RestaurantSystem& sys) {
    if (sys.staff().size() == 0) {
        std::cout << "  No staff to pay.\n";
        return;
    }
    core::Money bonus = readMoney("Bonus per staff (0 for none): ");
    core::Date month = sys.today();
    core::Date firstOfMonth(1, month.month(), month.year());

    std::cout << "\n--- Payroll run for " << firstOfMonth << " ---\n";
    for (std::size_t i = 0; i < sys.staff().size(); ++i) {
        payroll::Payslip slip = sys.payroll().runMonthly(*sys.staff().at(i), firstOfMonth, bonus);
        sys.payslips().add(slip.clone());
        std::cout << slip;
    }
    sys.saveAll();
    std::cout << "Total net expense: " << sys.payroll().totalExpense() << "\n";
}

void viewPayslips(RestaurantSystem& sys) {
    std::cout << "\n--- Payslip history (" << sys.payslips().size() << ") ---\n";
    for (std::size_t i = 0; i < sys.payslips().size(); ++i)
        std::cout << *sys.payslips().at(i);
}

// -------------------------------------------------------------- reports ----

void reports(RestaurantSystem& sys) {
    core::Date today = sys.today();

    // Daily sales
    core::Money daily(0);
    for (std::size_t i = 0; i < sys.bills().size(); ++i)
        if (sys.bills().at(i)->date() == today) daily += sys.bills().at(i)->total();
    std::cout << "\nDaily sales (" << today << "): " << daily << "\n";

    // Top dish by quantity sold
    std::map<int, int> dishCount;
    std::map<int, std::string> dishName;
    for (std::size_t i = 0; i < sys.orders().size(); ++i) {
        orders::Order* o = sys.orders().at(i);
        for (std::size_t j = 0; j < o->itemCount(); ++j) {
            dishCount[(*o)[j].dishId()] += (*o)[j].quantity();
            dishName[(*o)[j].dishId()] = (*o)[j].dishName();
        }
    }
    int bestDish = -1, bestQty = 0;
    for (std::map<int, int>::iterator it = dishCount.begin(); it != dishCount.end(); ++it) {
        if (it->second > bestQty) { bestQty = it->second; bestDish = it->first; }
    }
    if (bestDish >= 0)
        std::cout << "Top dish: " << dishName[bestDish] << " (" << bestQty << " sold)\n";
    else
        std::cout << "Top dish: none yet\n";

    // Best waiter by orders taken
    std::map<int, int> waiterOrders;
    for (std::size_t i = 0; i < sys.orders().size(); ++i)
        waiterOrders[sys.orders().at(i)->waiterId()]++;
    int bestWaiter = -1, bestCount = 0;
    for (std::map<int, int>::iterator it = waiterOrders.begin(); it != waiterOrders.end(); ++it) {
        if (it->second > bestCount) { bestCount = it->second; bestWaiter = it->first; }
    }
    if (bestWaiter > 0) {
        staff::Staff* w = sys.staff().findOrNull(bestWaiter);
        std::cout << "Best waiter: " << (w ? w->name() : "?") << " (" << bestCount << " orders)\n";
    } else {
        std::cout << "Best waiter: none yet\n";
    }

    // Monthly salary expense (sum of the persisted payslips for this month)
    core::Money monthly(0);
    for (std::size_t i = 0; i < sys.payslips().size(); ++i) {
        const payroll::Payslip* p = sys.payslips().at(i);
        if (p->month().month() == today.month() && p->month().year() == today.year())
            monthly += p->net();
    }
    std::cout << "Monthly salary expense (" << today.month() << "/" << today.year()
              << "): " << monthly << "\n";
}

// --------------------------------------------------------------- menus -----

void showMenuHeader(const std::string& title, staff::Staff& user) {
    std::cout << "\n=== " << title << " menu - " << user.name() << " ===\n";
}

} // anonymous namespace

// ============================================================ Manager ======

void ManagerMenu::run(RestaurantSystem& sys, staff::Staff& user) {
    bool done = false;
    while (!done) {
        showMenuHeader("MANAGER", user);
        std::cout << " 1. Hire staff\n"
                     " 2. Fire staff\n"
                     " 3. View all staff\n"
                     " 4. Mark attendance\n"
                     " 5. Add dish\n"
                     " 6. Update dish\n"
                     " 7. Delete dish\n"
                     " 8. Add ingredient\n"
                     " 9. Restock ingredient\n"
                     "10. View inventory / low stock\n"
                     "11. View all orders\n"
                     "12. View all tables\n"
                     "13. Run payroll\n"
                     "14. View payslips\n"
                     "15. Reports\n"
                     "16. Save now\n"
                     " 0. Logout\n";
        int choice = readInt("Choice: ");
        try {
            switch (choice) {
                case 1: hireStaff(sys); break;
                case 2: fireStaff(sys, user); break;
                case 3: listStaff(sys); break;
                case 4: markAttendance(sys); break;
                case 5: addDish(sys); break;
                case 6: updateDish(sys); break;
                case 7: deleteDish(sys); break;
                case 8: addIngredient(sys); break;
                case 9: restockIngredient(sys); break;
                case 10: {
                    listIngredients(sys);
                    std::vector<menu::Ingredient*> low = sys.inventory().lowStock();
                    std::cout << "Low stock items: " << low.size() << "\n";
                    for (std::size_t i = 0; i < low.size(); ++i)
                        std::cout << "  " << *low[i] << "\n";
                    break;
                }
                case 11: listOrders(sys); break;
                case 12: listTables(sys); break;
                case 13: runPayroll(sys); break;
                case 14: viewPayslips(sys); break;
                case 15: reports(sys); break;
                case 16: sys.saveAll(); std::cout << "  Saved.\n"; break;
                case 0: done = true; break;
                default: std::cout << "  ! Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
        if (!done) pressEnter();
    }
}

// ============================================================ Cashier ======

void CashierMenu::run(RestaurantSystem& sys, staff::Staff& user) {
    bool done = false;
    while (!done) {
        showMenuHeader("CASHIER", user);
        std::cout << " 1. View open orders\n"
                     " 2. Create bill / take payment\n"
                     " 3. View all bills\n"
                     " 4. View tables\n"
                     " 5. My salary info\n"
                     " 0. Logout\n";
        int choice = readInt("Choice: ");
        try {
            switch (choice) {
                case 1: listOpenOrders(sys); break;
                case 2: createBill(sys, user); break;
                case 3: {
                    std::cout << "\n--- Bills (" << sys.bills().size() << ") ---\n";
                    for (std::size_t i = 0; i < sys.bills().size(); ++i)
                        std::cout << *sys.bills().at(i);
                    break;
                }
                case 4: listTables(sys); break;
                case 5:
                    std::cout << "Base " << user.baseSalary()
                              << "  current pay " << user.calculatePay() << "\n";
                    break;
                case 0: done = true; break;
                default: std::cout << "  ! Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
        if (!done) pressEnter();
    }
}

// ============================================================= Waiter ======

void WaiterMenu::run(RestaurantSystem& sys, staff::Staff& user) {
    bool done = false;
    while (!done) {
        showMenuHeader("WAITER", user);
        std::cout << " 1. View free tables\n"
                     " 2. Take an order\n"
                     " 3. View my orders\n"
                     " 4. View my tips\n"
                     " 5. Mark myself present\n"
                     " 0. Logout\n";
        int choice = readInt("Choice: ");
        try {
            switch (choice) {
                case 1: {
                    std::cout << "\n--- Free tables ---\n";
                    for (std::size_t i = 0; i < sys.tables().size(); ++i)
                        if (sys.tables().at(i)->isFree()) std::cout << *sys.tables().at(i) << "\n";
                    break;
                }
                case 2: placeOrder(sys, user); break;
                case 3: {
                    std::cout << "\n--- My orders ---\n";
                    for (std::size_t i = 0; i < sys.orders().size(); ++i)
                        if (sys.orders().at(i)->waiterId() == user.id())
                            std::cout << *sys.orders().at(i) << "\n\n";
                    break;
                }
                case 4:
                    std::cout << "Tips earned: " << user.attendance().tipsEarned()
                              << "  current pay " << user.calculatePay() << "\n";
                    break;
                case 5:
                    user.attendance().markPresent(sys.today());
                    sys.saveAll();
                    std::cout << "  Marked present for today.\n";
                    break;
                case 0: done = true; break;
                default: std::cout << "  ! Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
        if (!done) pressEnter();
    }
}

// ============================================================== Chef ======

void ChefMenu::run(RestaurantSystem& sys, staff::Staff& user) {
    bool done = false;
    while (!done) {
        showMenuHeader("CHEF", user);
        std::cout << " 1. View cooking queue\n"
                     " 2. Start cooking an order\n"
                     " 3. Mark an order ready\n"
                     " 4. View all orders\n"
                     " 5. Mark myself present\n"
                     " 0. Logout\n";
        int choice = readInt("Choice: ");
        try {
            switch (choice) {
                case 1: {
                    std::cout << "\n--- Cooking queue (open orders) ---\n";
                    for (std::size_t i = 0; i < sys.orders().size(); ++i) {
                        orders::Order* o = sys.orders().at(i);
                        if (o->status() == orders::ORDER_PLACED ||
                            o->status() == orders::ORDER_COOKING ||
                            o->status() == orders::ORDER_READY)
                            std::cout << *o << "\n\n";
                    }
                    break;
                }
                case 2: {
                    listOpenOrders(sys);
                    int id = readInt("Order id to start cooking: ");
                    orders::Order* o = sys.orders().find(id);
                    if (o->status() != orders::ORDER_PLACED)
                        throw core::InvalidInputException("Order is not in Placed state");
                    o->setStatus(orders::ORDER_COOKING);
                    o->setChefId(user.id());
                    sys.saveAll();
                    std::cout << "  Order #" << id << " is now Cooking.\n";
                    break;
                }
                case 3: {
                    listOpenOrders(sys);
                    int id = readInt("Order id that is ready: ");
                    orders::Order* o = sys.orders().find(id);
                    if (o->status() != orders::ORDER_COOKING)
                        throw core::InvalidInputException("Order is not Cooking");
                    o->setStatus(orders::ORDER_READY);
                    if (staff::Chef* c = dynamic_cast<staff::Chef*>(&user)) c->addDishCooked();
                    sys.saveAll();
                    std::cout << "  Order #" << id << " is now Ready.\n";
                    break;
                }
                case 4: listOrders(sys); break;
                case 5:
                    user.attendance().markPresent(sys.today());
                    sys.saveAll();
                    std::cout << "  Marked present for today.\n";
                    break;
                case 0: done = true; break;
                default: std::cout << "  ! Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
        if (!done) pressEnter();
    }
}

// =========================================================== Cleaner ======

void CleanerMenu::run(RestaurantSystem& sys, staff::Staff& user) {
    bool done = false;
    while (!done) {
        showMenuHeader("CLEANER", user);
        std::cout << " 1. View my cleaning schedule\n"
                     " 2. Add a cleaning task\n"
                     " 3. Mark myself present\n"
                     " 4. View my attendance\n"
                     " 0. Logout\n";
        int choice = readInt("Choice: ");
        try {
            switch (choice) {
                case 1: {
                    staff::Cleaner* c = dynamic_cast<staff::Cleaner*>(&user);
                    std::cout << "\n--- Schedule ---\n";
                    if (c) {
                        const std::vector<std::string>& s = c->schedule();
                        for (std::size_t i = 0; i < s.size(); ++i)
                            std::cout << "  " << (i + 1) << ". " << s[i] << "\n";
                    }
                    break;
                }
                case 2: {
                    staff::Cleaner* c = dynamic_cast<staff::Cleaner*>(&user);
                    if (c) {
                        c->addSchedule(readLine("New cleaning task: "));
                        sys.saveAll();
                        std::cout << "  Task added.\n";
                    }
                    break;
                }
                case 3:
                    user.attendance().markPresent(sys.today());
                    sys.saveAll();
                    std::cout << "  Marked present for today.\n";
                    break;
                case 4:
                    std::cout << "Attendance: " << user.attendance() << "\n";
                    break;
                case 0: done = true; break;
                default: std::cout << "  ! Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  ! " << e.what() << "\n";
        }
        if (!done) pressEnter();
    }
}

} // namespace ui
} // namespace restaurant
