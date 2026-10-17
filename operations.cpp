#include "restaurant.h"
#include "exceptions.h"
#include <iostream>
#include <string>

namespace restaurant {

// ---------- Staff sub-menu (Manager / Head Chef) ----------
void staffMenu(Restaurant &r) {
    while (true) {
        std::cout << "\n-- STAFF MENU --\n";
        std::cout << "1. Add Staff\n2. View Staff\n3. Update Staff\n";
        std::cout << "4. Delete Staff\n5. Promote/Change Role\n6. Mark Attendance\n7. Back\n";
        int choice = readInt("Choice: ");
        try {
            if (choice == 1) {
                std::cout << "Roles: 1=Manager 2=Chef 3=Waiter 4=Cashier 5=Cleaner 6=HeadChef\n";
                int role = readInt("Role: ");
                std::string name;
                std::cout << "Name: ";
                std::getline(std::cin, name);
                name = trim(name);
                double base = readDouble("Base salary: ");
                r.addStaff(role, name, base);
            } else if (choice == 2) {
                r.viewStaff();
            } else if (choice == 3) {
                int id = readInt("Staff ID: ");
                r.updateStaff(id);
            } else if (choice == 4) {
                int id = readInt("Staff ID: ");
                r.deleteStaff(id);
            } else if (choice == 5) {
                int id = readInt("Staff ID: ");
                std::cout << "New role (1-6): ";
                int nr = readInt("");
                r.promoteStaff(id, nr);
            } else if (choice == 6) {
                int id = readInt("Staff ID: ");
                int days = readInt("Days present: ");
                double ot = readDouble("Overtime hours: ");
                r.markAttendance(id, days, ot);
            } else if (choice == 7) {
                break;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const RestaurantException &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- Menu sub-menu ----------
void menuMenu(Restaurant &r) {
    while (true) {
        std::cout << "\n-- MENU MENU --\n";
        std::cout << "1. Add Dish\n2. View Menu\n3. Update Dish\n4. Delete Dish\n5. Back\n";
        int choice = readInt("Choice: ");
        try {
            if (choice == 1) {
                std::string name;
                std::cout << "Dish name: ";
                std::getline(std::cin, name);
                name = trim(name);
                double price = readDouble("Price: ");
                int stock = readInt("Stock: ");
                r.addDish(name, price, stock);
            } else if (choice == 2) {
                r.viewMenu();
            } else if (choice == 3) {
                int id = readInt("Dish ID: ");
                r.updateDish(id);
            } else if (choice == 4) {
                int id = readInt("Dish ID: ");
                r.deleteDish(id);
            } else if (choice == 5) {
                break;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const RestaurantException &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- Order sub-menu ----------
void orderMenu(Restaurant &r) {
    while (true) {
        std::cout << "\n-- ORDER MENU --\n";
        std::cout << "1. Place Order (Waiter)\n2. View Cooking Queue\n";
        std::cout << "3. Mark Cooking (Chef)\n4. Mark Ready (Chef)\n5. Mark Served (Waiter)\n6. Back\n";
        int choice = readInt("Choice: ");
        try {
            if (choice == 1) {
                int table = readInt("Table number: ");
                r.placeOrder(table);
            } else if (choice == 2) {
                r.viewCookingQueue();
            } else if (choice == 3) {
                int id = readInt("Order ID: ");
                r.markCooking(id);
            } else if (choice == 4) {
                int id = readInt("Order ID: ");
                r.markReady(id);
            } else if (choice == 5) {
                int id = readInt("Order ID: ");
                r.markServed(id);
            } else if (choice == 6) {
                break;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const RestaurantException &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- Billing sub-menu (Cashier) ----------
void billingMenu(Restaurant &r) {
    while (true) {
        std::cout << "\n-- BILLING MENU --\n";
        std::cout << "1. Make Bill\n2. Back\n";
        int choice = readInt("Choice: ");
        try {
            if (choice == 1) {
                int id = readInt("Order ID: ");
                double disc = readDouble("Discount % (0-100): ");
                double tip = readDouble("Tip amount: ");
                r.makeBill(id, disc, tip);
            } else if (choice == 2) {
                break;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const RestaurantException &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- Payroll sub-menu ----------
void payrollMenu(Restaurant &r) {
    while (true) {
        std::cout << "\n-- PAYROLL MENU --\n";
        std::cout << "1. Run Payroll\n2. View Payslip History\n3. Back\n";
        int choice = readInt("Choice: ");
        try {
            if (choice == 1) {
                r.runPayroll();
            } else if (choice == 2) {
                r.viewPayslipHistory();
            } else if (choice == 3) {
                break;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const RestaurantException &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- Role-based main menu ----------
void runApp(Restaurant &r) {
    while (true) {
        if (!r.isLoggedIn()) {
            std::cout << "\n========== RESTAURANT MANAGEMENT SYSTEM ==========\n";
            std::cout << "1. Login\n2. Exit\n";
            int choice = readInt("Choice: ");
            try {
                if (choice == 1) {
                    int id = readInt("Enter Staff ID: ");
                    if (r.login(id)) {
                        std::cout << "Login successful. Welcome!\n";
                    } else {
                        std::cout << "Login failed: invalid Staff ID.\n";
                    }
                } else if (choice == 2) {
                    break;
                } else {
                    std::cout << "Invalid choice.\n";
                }
            } catch (const RestaurantException &e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else {
            int role = r.getLoggedInRole();
            std::cout << "\n-- MAIN MENU (Logged in as ID " << r.getLoggedInId() << ") --\n";
            if (role == 1 || role == 6) {   // Manager / Head Chef
                std::cout << "1. Staff\n2. Menu\n3. Order\n4. Billing\n5. Payroll\n6. Reports\n7. My Profile\n8. Logout\n";
            } else if (role == 2 || role == 6) {  // Chef / Head Chef
                std::cout << "1. Order\n2. My Profile\n3. Logout\n";
            } else if (role == 3) {  // Waiter
                std::cout << "1. Order\n2. Billing View\n3. My Profile\n4. Logout\n";
            } else if (role == 4) {  // Cashier
                std::cout << "1. Billing\n2. My Profile\n3. Logout\n";
            } else if (role == 5) {  // Cleaner
                std::cout << "1. My Profile\n2. Logout\n";
            }
            int choice = readInt("Choice: ");
            try {
                if (role == 1 || role == 6) {
                    if (choice == 1) staffMenu(r);
                    else if (choice == 2) menuMenu(r);
                    else if (choice == 3) orderMenu(r);
                    else if (choice == 4) billingMenu(r);
                    else if (choice == 5) payrollMenu(r);
                    else if (choice == 6) r.showReports();
                    else if (choice == 7) r.viewMyProfile();
                    else if (choice == 8) { r.logout(); }  // logout
                    else std::cout << "Invalid choice.\n";
                } else if (role == 2 || role == 6) {
                    if (choice == 1) orderMenu(r);
                    else if (choice == 2) r.viewMyProfile();
                    else if (choice == 3) { r.logout(); }
                    else std::cout << "Invalid choice.\n";
                } else if (role == 3) {
                    if (choice == 1) orderMenu(r);
                    else if (choice == 2) r.viewCookingQueue();
                    else if (choice == 3) r.viewMyProfile();
                    else if (choice == 4) { r.logout(); }
                    else std::cout << "Invalid choice.\n";
                } else if (role == 4) {
                    if (choice == 1) billingMenu(r);
                    else if (choice == 2) r.viewMyProfile();
                    else if (choice == 3) { r.logout(); }
                    else std::cout << "Invalid choice.\n";
                } else if (role == 5) {
                    if (choice == 1) r.viewMyProfile();
                    else if (choice == 2) { r.logout(); }
                    else std::cout << "Invalid choice.\n";
                }
            } catch (const RestaurantException &e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
}

}
