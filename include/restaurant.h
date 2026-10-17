#ifndef RMS_RESTAURANT_H
#define RMS_RESTAURANT_H

#include <string>
#include <vector>
#include "staff.h"
#include "manager.h"
#include "menu.h"
#include "order.h"
#include "payroll.h"
#include "billing.h"
#include "templates.h"
#include "exceptions.h"
#include "utils.h"

namespace restaurant {

class Restaurant {
private:
    std::vector<Staff*> staffList;             // run-time polymorphism through base-class pointers
    Repo<Dish> menuRepo;                       // class template container
    std::vector<Order> orders;
    Repo<Payslip> payslipHistory;              // class template container
    FileStore<StaffRecord> staffStore;         // class template binary storage (random access)
    FileStore<DishRecord> menuStore;           // class template binary storage (random access)
    int loggedInId;
    int loggedInRole;

    Staff* findStaffById(int id) const;
    int findStaffIndexById(int id) const;
    Dish* findDishById(int id);
    int findDishIndexById(int id) const;
    Order* findOrderById(int id);

    void saveAllStaff();
    void saveAllMenu();
    void loadAllStaff();
    void loadAllMenu();
    void seedDefaultManager();                 // first run: create one default manager

public:
    Restaurant();
    ~Restaurant();

    bool login(int id);
    void logout();
    int getLoggedInId() const;
    int getLoggedInRole() const;
    bool isLoggedIn() const;

    // Staff operations
    void addStaff(int role, const std::string &name, double base);
    void viewStaff() const;
    void updateStaff(int id);
    void deleteStaff(int id);
    void promoteStaff(int id, int newRole);
    void markAttendance(int id, int days, double ot);
    void viewMyProfile() const;

    // Menu operations
    void addDish(const std::string &name, double price, int stock);
    void viewMenu() const;
    void updateDish(int id);
    void deleteDish(int id);

    // Order operations
    void placeOrder(int tableNo);
    void viewCookingQueue() const;
    void markCooking(int orderId);
    void markReady(int orderId);
    void markServed(int orderId);

    // Billing
    void makeBill(int orderId, double discountPercent, double tip);

    // Payroll
    void runPayroll();
    void viewPayslipHistory() const;

    // Reports
    void showReports() const;
};

}

#endif
