#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>
#include "exceptions.h"
#include "menu.h"
#include "order.h"
#include "payroll.h"
#include "staff.h"
#include "templates.h"

class Restaurant {
private:
    std::vector<Staff*> staffList;
    Repo<Dish> menu; // dynamic array template used for catalog management
    std::vector<Order> orders;

    FileStore<StaffData> staffStore;
    FileStore<Dish> menuStore;
    FileStore<Order> orderStore;

    Payroll payroll;

    int loggedInId;
    int loggedInRole;

    int findStaffIndexById(int id) const;
    int findDishIndexById(int id) const;
    Order* findOrderById(int id);

    void saveStaff(const Staff &staffMember);
    void saveDish(const Dish &dish);
    void saveOrder(const Order &order);

    void loadAllStaff();
    void loadAllMenu();
    void loadAllOrders();

public:
    Restaurant();
    ~Restaurant();

    bool login(int id);
    void logout();
    int getLoggedInId() const;
    int getLoggedInRole() const;
    bool isLoggedIn() const;

    // Staff management
    void addStaff(int role, const std::string &name, double salary);
    void viewStaff() const;
    void updateStaff(int id, const std::string &name, double salary);
    void deleteStaff(int id);

    // Menu management
    void addDish(const std::string &name, double price);
    void addDish(const std::string &name, double price, int stock);
    void viewMenu() const;
    void updateDish(int id, double price, int stock);
    void deleteDish(int id);

    // Orders and billing
    void placeOrder(int tableNo);
    void viewOrders() const;
    void markServed(int orderId);
    void makeBill(int orderId);

    // Payroll
    void runPayroll();
    void viewPayslipHistory() const;
};

#endif
