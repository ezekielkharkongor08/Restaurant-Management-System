#include "restaurant.h"

#include <cstring>
#include <iostream>
#include <vector>
#include "menus.h"

using namespace std;

const string STAFF_FILE = "data/staff.dat";
const string MENU_FILE = "data/menu.dat";
const string ORDER_FILE = "data/orders.dat";

const string DEFAULT_MANAGER_NAME = "Default Manager";
const double DEFAULT_MANAGER_SALARY = 50000.0;

Restaurant::Restaurant()
    : staffStore(STAFF_FILE),
      menuStore(MENU_FILE),
      orderStore(ORDER_FILE),
      loggedInId(0),
      loggedInRole(0) {
    loadAllStaff();

    if (staffList.empty()) {
        Manager *manager = new Manager(DEFAULT_MANAGER_NAME, DEFAULT_MANAGER_SALARY);
        staffList.push_back(manager);
        staffStore.append(manager->toData());
    }

    loadAllMenu();
    loadAllOrders();
}

Restaurant::~Restaurant() {
    for (int i = 0; i < (int)staffList.size(); i++) {
        if (staffList[i] != NULL) {
            delete staffList[i];
        }
    }
}

int Restaurant::findStaffIndexById(int id) const {
    for (int i = 0; i < (int)staffList.size(); i++) {
        if (staffList[i] != NULL && staffList[i]->getId() == id) {
            return i;
        }
    }
    return -1;
}

int Restaurant::findDishIndexById(int id) const {
    for (int i = 0; i < menu.size(); i++) {
        if (menu[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

Order* Restaurant::findOrderById(int id) {
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getId() == id) {
            return &orders[i];
        }
    }
    return NULL;
}

void Restaurant::saveStaff(const Staff &staffMember) {
    int position = staffStore.findPosition(staffMember.getId());
    if (position == -1) {
        throw RestaurantException("Staff record not found in the data file.");
    }
    if (!staffStore.updateAt(position, staffMember.toData())) {
        throw RestaurantException("Cannot update the staff data file.");
    }
}

void Restaurant::saveDish(const Dish &dish) {
    int position = menuStore.findPosition(dish.getId());
    if (position == -1) {
        throw RestaurantException("Dish record not found in the data file.");
    }
    if (!menuStore.updateAt(position, dish)) {
        throw RestaurantException("Cannot update the menu data file.");
    }
}

void Restaurant::saveOrder(const Order &order) {
    int position = orderStore.findPosition(order.getId());
    if (position == -1) {
        throw RestaurantException("Order record not found in the data file.");
    }
    if (!orderStore.updateAt(position, order)) {
        throw RestaurantException("Cannot update the order data file.");
    }
}

void Restaurant::loadAllStaff() {
    vector<StaffData> records = staffStore.readAll();
    int highestId = 0;

    for (int i = 0; i < (int)records.size(); i++) {
        const StaffData &record = records[i];

        if (record.id > highestId) {
            highestId = record.id;
        }
        if (record.active == 0) {
            continue;
        }

        Staff *person = NULL;
        if (record.role == MANAGER) {
            person = new Manager();
        } else if (record.role == CHEF) {
            person = new Chef();
        } else if (record.role == WAITER) {
            person = new Waiter();
        } else if (record.role == HEAD_CHEF) {
            person = new HeadChef();
        }

        if (person != NULL) {
            person->loadFromData(record);
            staffList.push_back(person);
        }
    }

    if (highestId + 1 > Staff::nextIdValue()) {
        Staff::setNextIdValue(highestId + 1);
    }
}

void Restaurant::loadAllMenu() {
    vector<Dish> records = menuStore.readAll();
    int highestId = 0;

    for (int i = 0; i < (int)records.size(); i++) {
        const Dish &dish = records[i];

        if (dish.getId() > highestId) {
            highestId = dish.getId();
        }
        if (!dish.isActive()) {
            continue;
        }

        menu.add(dish);
    }

    if (highestId + 1 > Dish::nextIdValue()) {
        Dish::setNextIdValue(highestId + 1);
    }
}

void Restaurant::loadAllOrders() {
    vector<Order> records = orderStore.readAll();
    int highestId = 0;

    for (int i = 0; i < (int)records.size(); i++) {
        const Order &order = records[i];

        if (order.getId() > highestId) {
            highestId = order.getId();
        }

        orders.push_back(order);
    }

    if (highestId + 1 > Order::nextIdValue()) {
        Order::setNextIdValue(highestId + 1);
    }
}

bool Restaurant::login(int id) {
    int index = findStaffIndexById(id);
    if (index == -1) {
        return false;
    }

    loggedInId = staffList[index]->getId();
    loggedInRole = staffList[index]->getRole();
    return true;
}

void Restaurant::logout() {
    loggedInId = 0;
    loggedInRole = 0;
}

int Restaurant::getLoggedInId() const {
    return loggedInId;
}

int Restaurant::getLoggedInRole() const {
    return loggedInRole;
}

bool Restaurant::isLoggedIn() const {
    return loggedInId != 0;
}

// ---------------- Staff Management ----------------

void Restaurant::addStaff(int role, const std::string &name, double salary) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can add staff.");
    }
    if (name.empty()) {
        throw RestaurantException("Staff name cannot be empty.");
    }
    if (salary < 0.0) {
        throw RestaurantException("Salary cannot be negative.");
    }
    if (role != MANAGER && role != CHEF && role != WAITER && role != HEAD_CHEF) {
        throw RestaurantException("Invalid staff role.");
    }

    Staff *person = NULL;
    if (role == MANAGER) {
        person = new Manager(name, salary);
    } else if (role == CHEF) {
        person = new Chef(name, salary);
    } else if (role == WAITER) {
        person = new Waiter(name, salary);
    } else {
        person = new HeadChef(name, salary);
    }

    staffStore.append(person->toData());
    staffList.push_back(person);
    cout << "Staff added. ID: " << person->getId() << endl;
}

void Restaurant::viewStaff() const {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can view staff.");
    }

    cout << "\n========== STAFF LIST ==========\n";
    if (staffList.empty()) {
        cout << "No staff records.\n";
    } else {
        for (int i = 0; i < (int)staffList.size(); i++) {
            staffList[i]->display();
            cout << endl;
        }
    }
    cout << "================================\n";
}

void Restaurant::updateStaff(int id, const std::string &name, double salary) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can update staff.");
    }
    if (name.empty()) {
        throw RestaurantException("Staff name cannot be empty.");
    }
    if (salary < 0.0) {
        throw RestaurantException("Salary cannot be negative.");
    }

    int index = findStaffIndexById(id);
    if (index == -1) {
        throw RestaurantException("Staff not found.");
    }

    staffList[index]->setName(name);
    staffList[index]->setBaseSalary(salary);
    saveStaff(*staffList[index]);
    cout << "Staff updated.\n";
}

void Restaurant::deleteStaff(int id) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can delete staff.");
    }
    if (id == loggedInId) {
        throw RestaurantException("You cannot delete the logged-in staff member.");
    }

    int index = findStaffIndexById(id);
    if (index == -1) {
        throw RestaurantException("Staff not found.");
    }

    Staff *person = staffList[index];
    int position = staffStore.findPosition(id);
    if (position == -1) {
        throw RestaurantException("Staff record not found in the data file.");
    }

    StaffData data = person->toData();
    data.active = 0;
    if (!staffStore.updateAt(position, data)) {
        throw RestaurantException("Cannot update the staff data file.");
    }

    delete person;
    staffList.erase(staffList.begin() + index);
    cout << "Staff deleted.\n";
}

// ---------------- Menu Management ----------------

void Restaurant::addDish(const std::string &name, double price) {
    addDish(name, price, 0);
}

void Restaurant::addDish(const std::string &name, double price, int stock) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can add dishes.");
    }
    if (name.empty()) {
        throw RestaurantException("Dish name cannot be empty.");
    }
    if (price < 0.0) {
        throw RestaurantException("Price cannot be negative.");
    }
    if (stock < 0) {
        throw RestaurantException("Stock cannot be negative.");
    }

    Dish dish(name, price, stock);
    menu.add(dish);
    menuStore.append(dish);
    cout << "Dish added. ID: " << dish.getId() << endl;
}

void Restaurant::viewMenu() const {
    cout << "\n========== MENU ==========\n";

    vector<Dish> activeDishes;
    for (int i = 0; i < menu.size(); i++) {
        activeDishes.push_back(menu[i]);
    }

    if (activeDishes.empty()) {
        cout << "Menu is empty.\n";
    } else {
        sortItems(&activeDishes[0], (int)activeDishes.size());
        for (int i = 0; i < (int)activeDishes.size(); i++) {
            cout << activeDishes[i] << endl;
        }
    }

    cout << "==========================\n";
}

void Restaurant::updateDish(int id, double price, int stock) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can update dishes.");
    }
    if (price < 0.0) {
        throw RestaurantException("Price cannot be negative.");
    }
    if (stock < 0) {
        throw RestaurantException("Stock cannot be negative.");
    }

    int index = findDishIndexById(id);
    if (index == -1) {
        throw RestaurantException("Dish not found.");
    }

    menu[index].setPrice(price);
    menu[index].setStock(stock);
    saveDish(menu[index]);
    cout << "Dish updated.\n";
}

void Restaurant::deleteDish(int id) {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can delete dishes.");
    }

    int index = findDishIndexById(id);
    if (index == -1) {
        throw RestaurantException("Dish not found.");
    }

    int position = menuStore.findPosition(id);
    if (position == -1) {
        throw RestaurantException("Dish record not found in the data file.");
    }

    Dish dish = menu[index];
    dish.setActive(false);
    if (!menuStore.updateAt(position, dish)) {
        throw RestaurantException("Cannot update the menu data file.");
    }

    menu.removeAt(index);
    cout << "Dish deleted.\n";
}

// ---------------- Orders and Billing ----------------

void Restaurant::placeOrder(int tableNo) {
    if (loggedInRole != WAITER) {
        throw RestaurantException("Only a Waiter can place orders.");
    }
    if (tableNo <= 0) {
        throw RestaurantException("Table number must be positive.");
    }

    viewMenu();

    vector<OrderItem> cart;

    while (true) {
        int dishId = readInt("Dish ID (0 to finish): ");
        if (dishId == 0) {
            break;
        }

        int index = findDishIndexById(dishId);
        if (index == -1) {
            cout << "Dish not found.\n";
            continue;
        }

        int quantity = readInt("Quantity: ");
        if (quantity <= 0) {
            cout << "Quantity must be positive.\n";
            continue;
        }

        bool found = false;
        for (int i = 0; i < (int)cart.size(); i++) {
            if (cart[i].dishId == dishId) {
                cart[i].qty = cart[i].qty + quantity;
                found = true;
                break;
            }
        }

        if (!found) {
            OrderItem item;
            item.dishId = dishId;
            strncpy(item.name, menu[index].getName().c_str(), sizeof(item.name) - 1);
            item.name[sizeof(item.name) - 1] = '\0';
            item.price = menu[index].getPrice();
            item.qty = quantity;
            cart.push_back(item);
        }
    }

    if (cart.empty()) {
        cout << "Order cancelled. No stock was changed.\n";
        return;
    }
    if ((int)cart.size() > MAX_ITEMS) {
        throw RestaurantException("Too many different dishes in one order.");
    }

    for (int i = 0; i < (int)cart.size(); i++) {
        int index = findDishIndexById(cart[i].dishId);
        if (index == -1 || menu[index].getStock() < cart[i].qty) {
            throw RestaurantException("Not enough stock for " + string(cart[i].name) + ".");
        }
    }

    Order order(tableNo, loggedInId);
    for (int i = 0; i < (int)cart.size(); i++) {
        order += cart[i];
        int index = findDishIndexById(cart[i].dishId);
        menu[index].reduceStock(cart[i].qty);
        saveDish(menu[index]);
    }

    orders.push_back(order);
    orderStore.append(order);
    cout << "Order placed.\n";
    cout << order << endl;
}

void Restaurant::viewOrders() const {
    if (loggedInRole != WAITER) {
        throw RestaurantException("Only a Waiter can view orders.");
    }

    cout << "\n========== ORDERS ==========\n";
    if (orders.empty()) {
        cout << "No orders yet.\n";
    } else {
        for (int i = 0; i < (int)orders.size(); i++) {
            cout << orders[i] << endl;
        }
    }
    cout << "============================\n";
}

void Restaurant::markServed(int orderId) {
    if (loggedInRole != WAITER) {
        throw RestaurantException("Only a Waiter can mark an order served.");
    }

    Order *order = findOrderById(orderId);
    if (order == NULL || !order->isPlaced()) {
        throw RestaurantException("Order not placed or not found.");
    }

    order->advanceState();
    saveOrder(*order);
    cout << "Order marked served.\n";
}

void Restaurant::makeBill(int orderId) {
    if (loggedInRole != WAITER) {
        throw RestaurantException("Only a Waiter can make bills.");
    }

    Order *order = findOrderById(orderId);
    if (order == NULL || !order->isServed()) {
        throw RestaurantException("Order not served or not found.");
    }

    double discount = readDouble("Discount % (0-100): ");
    if (discount < 0.0 || discount > 100.0) {
        throw RestaurantException("Discount must be between 0 and 100.");
    }

    Bill bill(order->getId(), order->getWaiterId(), order->subtotal(), discount);
    cout << bill;

    double payment = readDouble("Payment amount: ");
    if (payment < bill.getTotal()) {
        throw RestaurantException("Payment is less than the total.");
    }

    order->markPaid(bill.getTotal());
    saveOrder(*order);

    cout << "Change to return: Rs." << (payment - bill.getTotal()) << "\n";
    cout << "Order paid and closed.\n";
}

// ---------------- Payroll ----------------

void Restaurant::runPayroll() {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can run payroll.");
    }
    payroll.runPayroll(staffList);
}

void Restaurant::viewPayslipHistory() const {
    if (loggedInRole != MANAGER && loggedInRole != HEAD_CHEF) {
        throw RestaurantException("Only a Manager or HeadChef can view payslip history.");
    }
    payroll.showHistory();
}
