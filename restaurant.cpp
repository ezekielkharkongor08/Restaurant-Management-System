#include "restaurant.h"
#include <iostream>
#include <algorithm>

namespace restaurant {

Restaurant::Restaurant() : staffStore("staff.dat"), menuStore("menu.dat"),
    loggedInId(0), loggedInRole(0) {
    loadAllStaff();
    if (staffList.empty()) {
        seedDefaultManager();
    }
    loadAllMenu();
}

Restaurant::~Restaurant() {
    for (size_t i = 0; i < staffList.size(); i++) {
        delete staffList[i];
        staffList[i] = NULL;
    }
}

Staff* Restaurant::findStaffById(int id) const {
    for (size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i] && staffList[i]->getId() == id) return staffList[i];
    }
    return NULL;
}

int Restaurant::findStaffIndexById(int id) const {
    for (int i = 0; i < (int)staffList.size(); i++) {
        if (staffList[i] && staffList[i]->getId() == id) return i;
    }
    return -1;
}

Dish* Restaurant::findDishById(int id) {
    for (int i = 0; i < menuRepo.size(); i++) {
        if (menuRepo[i].getId() == id) return &menuRepo[i];
    }
    return NULL;
}

int Restaurant::findDishIndexById(int id) const {
    for (int i = 0; i < menuRepo.size(); i++) {
        if (menuRepo[i].getId() == id) return i;
    }
    return -1;
}

Order* Restaurant::findOrderById(int id) {
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].getId() == id) return &orders[i];
    }
    return NULL;
}

void Restaurant::saveAllStaff() {
    // Rewrite entire staff file (simple approach for correctness)
    FILE *f = std::fopen("staff.dat", "wb");
    if (f == NULL) throw RestaurantException("Cannot save staff data.");
    for (size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i]) {
            StaffRecord r = staffList[i]->toRecord();
            std::fwrite(&r, sizeof(StaffRecord), 1, f);
        }
    }
    std::fclose(f);
}

void Restaurant::saveAllMenu() {
    FILE *f = std::fopen("menu.dat", "wb");
    if (f == NULL) throw RestaurantException("Cannot save menu data.");
    for (int i = 0; i < menuRepo.size(); i++) {
        DishRecord r = menuRepo[i].toRecord();
        std::fwrite(&r, sizeof(DishRecord), 1, f);
    }
    std::fclose(f);
}

void Restaurant::loadAllStaff() {
    std::vector<StaffRecord> recs = staffStore.readAll();
    for (size_t i = 0; i < recs.size(); i++) {
        const StaffRecord &r = recs[i];
        if (!r.active) continue;
        Staff *s = NULL;
        if (r.role == 1)      s = new Manager();
        else if (r.role == 2) s = new Chef();
        else if (r.role == 3) s = new Waiter();
        else if (r.role == 4) s = new Cashier();
        else if (r.role == 5) s = new Cleaner();
        else if (r.role == 6) s = new HeadChef();
        if (s) {
            s->fromRecord(r);
            staffList.push_back(s);
            if (r.id >= Staff::nextIdValue()) Staff::setNextIdValue(r.id + 1);
        }
    }
}

void Restaurant::loadAllMenu() {
    std::vector<DishRecord> recs = menuStore.readAll();
    for (size_t i = 0; i < recs.size(); i++) {
        const DishRecord &r = recs[i];
        if (!r.active) continue;
        Dish d;
        d.fromRecord(r);
        menuRepo.add(d);
        if (r.id >= Dish::nextDishIdValue()) Dish::setNextDishIdValue(r.id + 1);
    }
}

void Restaurant::seedDefaultManager() {
    Manager *m = new Manager("Default Manager", 50000.0);
    staffList.push_back(m);
    StaffRecord r = m->toRecord();
    staffStore.append(r);
}

bool Restaurant::login(int id) {
    Staff *s = findStaffById(id);
    if (!s) return false;
    loggedInId = id;
    loggedInRole = s->role();
    return true;
}

void Restaurant::logout() {
    loggedInId = 0;
    loggedInRole = 0;
}

int Restaurant::getLoggedInId() const { return loggedInId; }
int Restaurant::getLoggedInRole() const { return loggedInRole; }
bool Restaurant::isLoggedIn() const { return loggedInId != 0; }

// ---------------- Staff operations ----------------
void Restaurant::addStaff(int role, const std::string &name, double base) {
    Staff *s = NULL;
    if (role == 1)      s = new Manager(name, base);
    else if (role == 2) s = new Chef(name, base);
    else if (role == 3) s = new Waiter(name, base);
    else if (role == 4) s = new Cashier(name, base);
    else if (role == 5) s = new Cleaner(name, base);
    else if (role == 6) s = new HeadChef(name, base);
    else throw RestaurantException("Invalid staff role.");

    staffList.push_back(s);
    saveAllStaff();   // persist (rewrite file)
    std::cout << "Staff added. ID: " << s->getId() << "\n";
}

void Restaurant::viewStaff() const {
    std::cout << "\n========== STAFF LIST ==========\n";
    bool any = false;
    for (size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i]) {
            staffList[i]->print(std::cout);
            std::cout << " | Duties: " << staffList[i]->duties() << "\n";
            any = true;
        }
    }
    if (!any) std::cout << "No staff records.\n";
    std::cout << "================================\n";
}

void Restaurant::updateStaff(int id) {
    int idx = findStaffIndexById(id);
    if (idx == -1) throw RestaurantException("Staff not found.");
    Staff *s = staffList[idx];
    std::string name;
    std::cout << "Enter new name (current " << s->getName() << "): ";
    std::getline(std::cin, name);
    if (!name.empty()) s->setName(name);
    double base = readDouble("Enter new base salary: ");
    s->setBaseSalary(base);
    saveAllStaff();
    std::cout << "Staff updated.\n";
}

void Restaurant::deleteStaff(int id) {
    int idx = findStaffIndexById(id);
    if (idx == -1) throw RestaurantException("Staff not found.");
    // Soft delete: mark inactive and rewrite file
    staffList[idx]->setActive(false);
    saveAllStaff();
    std::cout << "Staff marked inactive (soft delete).\n";
}

void Restaurant::promoteStaff(int id, int newRole) {
    int idx = findStaffIndexById(id);
    if (idx == -1) throw RestaurantException("Staff not found.");
    Staff *old = staffList[idx];
    double base = old->getBaseSalary();
    std::string name = old->getName();
    int days = old->getDaysPresent();
    double ot = old->getOvertimeHours();

    Staff *nw = NULL;
    if (newRole == 1)      nw = new Manager(name, base);
    else if (newRole == 2) nw = new Chef(name, base);
    else if (newRole == 3) nw = new Waiter(name, base);
    else if (newRole == 4) nw = new Cashier(name, base);
    else if (newRole == 5) nw = new Cleaner(name, base);
    else if (newRole == 6) nw = new HeadChef(name, base);
    else throw RestaurantException("Invalid new role.");

    nw->setDaysPresent(days);
    nw->setOvertimeHours(ot);
    delete staffList[idx];
    staffList[idx] = nw;
    saveAllStaff();
    std::cout << "Staff promoted/role changed to " << nw->roleName() << "\n";
}

void Restaurant::markAttendance(int id, int days, double ot) {
    int idx = findStaffIndexById(id);
    if (idx == -1) throw RestaurantException("Staff not found.");
    staffList[idx]->setDaysPresent(days);
    staffList[idx]->setOvertimeHours(ot);
    saveAllStaff();
    std::cout << "Attendance updated.\n";
}

void Restaurant::viewMyProfile() const {
    Staff *s = findStaffById(loggedInId);
    if (!s) throw RestaurantException("Logged-in staff not found.");
    s->print(std::cout);
    std::cout << " | Duties: " << s->duties() << "\n";
}

// ---------------- Menu operations ----------------
void Restaurant::addDish(const std::string &name, double price, int stock) {
    Dish d(name, price, stock);
    menuRepo.add(d);
    saveAllMenu();
    std::cout << "Dish added. ID: " << d.getId() << "\n";
}

void Restaurant::viewMenu() const {
    std::cout << "\n========== MENU ==========\n";
    if (menuRepo.size() == 0) {
        std::cout << "Menu is empty.\n";
    } else {
        // Show sorted copy by price (uses sortItems function template)
        Dish *copy = new Dish[menuRepo.size()];
        for (int i = 0; i < menuRepo.size(); i++) copy[i] = menuRepo[i];
        sortItems(copy, menuRepo.size());   // function template
        for (int i = 0; i < menuRepo.size(); i++) {
            std::cout << copy[i] << "\n";   // operator<<
        }
        delete[] copy;
    }
    std::cout << "==========================\n";
}

void Restaurant::updateDish(int id) {
    Dish *d = findDishById(id);
    if (!d) throw RestaurantException("Dish not found.");
    double price = readDouble("Enter new price: ");
    int stock = readInt("Enter new stock: ");
    d->setPrice(price);
    d->setStock(stock);
    saveAllMenu();
    std::cout << "Dish updated.\n";
}

void Restaurant::deleteDish(int id) {
    int idx = findDishIndexById(id);
    if (idx == -1) throw RestaurantException("Dish not found.");
    menuRepo[idx].setActive(false);
    saveAllMenu();
    std::cout << "Dish marked inactive.\n";
}

// ---------------- Order operations ----------------
void Restaurant::placeOrder(int tableNo) {
    if (loggedInRole != 3) throw RestaurantException("Only waiters can place orders.");
    Order order(tableNo, loggedInId);
    viewMenu();
    while (true) {
        int dishId = readInt("Enter dish ID to add (0 to finish): ");
        if (dishId == 0) break;
        Dish *d = findDishById(dishId);
        if (!d || !d->isActive()) { std::cout << "Dish not found or inactive.\n"; continue; }
        int qty = readInt("Enter quantity: ");
        if (!d->reduceStock(qty)) throw RestaurantException("Not enough stock!");
        OrderItem item;
        item.dishId = d->getId();
        item.name = d->getName();
        item.price = d->getPrice();
        item.qty = qty;
        order += item;   // operator overloading
        saveAllMenu();
    }
    orders.push_back(order);
    std::cout << "Order placed: " << order << "\n";
}

void Restaurant::viewCookingQueue() const {
    std::cout << "\n========== COOKING QUEUE ==========\n";
    bool any = false;
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].isPlaced() || orders[i].isCooking() || orders[i].isReady()) {
            std::cout << orders[i] << "\n";
            any = true;
        }
    }
    if (!any) std::cout << "Queue is empty.\n";
    std::cout << "===================================\n";
}

void Restaurant::markCooking(int orderId) {
    if (loggedInRole != 2 && loggedInRole != 6) throw RestaurantException("Only chef/head chef can mark cooking.");
    Order *o = findOrderById(orderId);
    if (!o || !o->isPlaced()) throw RestaurantException("Order not placed or not found.");
    o->advanceState();
    std::cout << "Order marked Cooking.\n";
}

void Restaurant::markReady(int orderId) {
    if (loggedInRole != 2 && loggedInRole != 6) throw RestaurantException("Only chef/head chef can mark ready.");
    Order *o = findOrderById(orderId);
    if (!o || !o->isCooking()) throw RestaurantException("Order not cooking or not found.");
    o->advanceState();
    // Chef/HeadChef dishesCooked increases
    Staff *s = findStaffById(loggedInId);
    Chef *c = dynamic_cast<Chef*>(s);
    if (c) {
        int totalQty = 0;
        const std::vector<OrderItem> &items = o->getItems();
        for (size_t i = 0; i < items.size(); i++) totalQty = totalQty + items[i].qty;
        c->setDishesCooked(c->getDishesCooked() + totalQty);
        saveAllStaff();
    }
    std::cout << "Order marked Ready.\n";
}

void Restaurant::markServed(int orderId) {
    if (loggedInRole != 3) throw RestaurantException("Only waiter can mark served.");
    Order *o = findOrderById(orderId);
    if (!o || !o->isReady()) throw RestaurantException("Order not ready or not found.");
    o->advanceState();
    std::cout << "Order marked Served.\n";
}

// ---------------- Billing ----------------
void Restaurant::makeBill(int orderId, double discountPercent, double tip) {
    if (loggedInRole != 4) throw RestaurantException("Only cashier can make bills.");
    Order *o = findOrderById(orderId);
    if (!o || !o->isServed()) throw RestaurantException("Order not served or not found.");
    if (discountPercent < 0 || discountPercent > 100) throw RestaurantException("Discount must be 0-100.");
    if (tip < 0) throw RestaurantException("Tip cannot be negative.");

    Bill bill(o->getId(), o->getWaiterId(), o->subtotal(), discountPercent, 5.0, tip);
    std::cout << bill;   // operator<<

    double payment = readDouble("Enter payment amount: ");
    if (payment < bill.getTotal()) throw RestaurantException("Payment less than total.");

    // Tip goes to waiter's pay data
    Staff *waiter = findStaffById(o->getWaiterId());
    if (waiter) {
        Waiter *w = dynamic_cast<Waiter*>(waiter);
        if (w) {
            w->addTips(tip);
            saveAllStaff();
        }
    }

    o->markPaid();
    std::cout << "Order paid and closed.\n";
}

// ---------------- Payroll ----------------
void Restaurant::runPayroll() {
    if (loggedInRole != 1 && loggedInRole != 6) throw RestaurantException("Only manager/head chef can run payroll.");
    Payroll payroll;
    payroll.runPayroll(staffList);   // run-time polymorphism via Staff*
    payslipHistory.clear();
    // store payslips into history repo (class template)
    // re-run to capture into history
    for (size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i]) {
            Payslip p = payroll.computePayslip(*staffList[i]);
            payslipHistory.add(p);
        }
    }
}

void Restaurant::viewPayslipHistory() const {
    std::cout << "\n========== PAYSLIP HISTORY ==========\n";
    if (payslipHistory.size() == 0) {
        std::cout << "No payslips yet.\n";
    } else {
        for (int i = 0; i < payslipHistory.size(); i++) {
            std::cout << payslipHistory[i];
        }
    }
    std::cout << "====================================\n";
}

// ---------------- Reports ----------------
void Restaurant::showReports() const {
    if (loggedInRole != 1 && loggedInRole != 6) throw RestaurantException("Only manager/head chef can see reports.");
    double totalPayroll = 0.0;
    double revenue = 0.0;
    for (size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i]) totalPayroll = totalPayroll + staffList[i]->calculatePay();
    }
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].isPaid()) revenue = revenue + orders[i].subtotal();
    }
    std::cout << "\n========== REPORTS ==========\n";
    std::cout << "Total Staff Count: " << staffList.size() << "\n";
    std::cout << "Estimated Total Payroll: Rs." << totalPayroll << "\n";
    std::cout << "Total Revenue (Paid Orders): Rs." << revenue << "\n";
    std::cout << "=============================\n";
}

}
