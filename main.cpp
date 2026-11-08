#include "menus.h"

#include <exception>
#include <iostream>
#include "restaurant.h"

using namespace std;

void runApp(Restaurant &restaurant) {
    while (true) {
        try {
            if (!restaurant.isLoggedIn()) {
                cout << "\n===== RESTAURANT MANAGEMENT SYSTEM =====\n";
                cout << "1. Login\n";
                cout << "2. Exit\n";

                int choice = readInt("Choice: ");
                if (choice == 1) {
                    int id = readInt("Staff ID: ");
                    if (restaurant.login(id)) {
                        cout << "Login successful.\n";
                    } else {
                        cout << "Invalid staff ID.\n";
                    }
                } else if (choice == 2) {
                    break;
                } else {
                    cout << "Invalid choice.\n";
                }
            } else {
                int role = restaurant.getLoggedInRole();
                cout << "\n--- MAIN MENU (ID " << restaurant.getLoggedInId() << ") ---\n";

                if (role == MANAGER || role == HEAD_CHEF) {
                    cout << "1. Manage Staff\n";
                    cout << "2. Manage Menu\n";
                    cout << "3. Run Payroll\n";
                    cout << "4. Payslip History\n";
                    cout << "5. Logout\n";

                    int choice = readInt("Choice: ");
                    if (choice == 1) {
                        staffMenu(restaurant);
                    } else if (choice == 2) {
                        menuMenu(restaurant);
                    } else if (choice == 3) {
                        restaurant.runPayroll();
                    } else if (choice == 4) {
                        restaurant.viewPayslipHistory();
                    } else if (choice == 5) {
                        restaurant.logout();
                    } else {
                        cout << "Invalid choice.\n";
                    }
                } else if (role == CHEF) {
                    cout << "1. View Menu\n";
                    cout << "2. Logout\n";

                    int choice = readInt("Choice: ");
                    if (choice == 1) {
                        restaurant.viewMenu();
                    } else if (choice == 2) {
                        restaurant.logout();
                    } else {
                        cout << "Invalid choice.\n";
                    }
                } else if (role == WAITER) {
                    cout << "1. Place Order\n";
                    cout << "2. View Orders\n";
                    cout << "3. Mark Served\n";
                    cout << "4. Make Bill\n";
                    cout << "5. Logout\n";

                    int choice = readInt("Choice: ");
                    if (choice == 1) {
                        int table = readInt("Table number: ");
                        restaurant.placeOrder(table);
                    } else if (choice == 2) {
                        restaurant.viewOrders();
                    } else if (choice == 3) {
                        int id = readInt("Order ID: ");
                        restaurant.markServed(id);
                    } else if (choice == 4) {
                        int id = readInt("Order ID: ");
                        restaurant.makeBill(id);
                    } else if (choice == 5) {
                        restaurant.logout();
                    } else {
                        cout << "Invalid choice.\n";
                    }
                } else {
                    cout << "Unknown role. Logging out.\n";
                    restaurant.logout();
                }
            }
        } catch (const exception &error) {
            if (cin.eof()) {
                throw;
            }
            cout << "Error: " << error.what() << endl;
        }
    }
}

int main() {
    try {
        Restaurant restaurant;
        runApp(restaurant);
    } catch (const exception &error) {
        if (cin.eof()) {
            return 0;
        }
        cerr << "Fatal error: " << error.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown fatal error.\n";
        return 1;
    }

    return 0;
}
