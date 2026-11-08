#include "menus.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include "restaurant.h"

using namespace std;

int readInt(const std::string &prompt) {
    int value;
    cout << prompt;
    cin >> value;

    if (cin.eof()) {
        throw runtime_error("End of input.");
    }
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        throw runtime_error("Invalid integer input.");
    }

    return value;
}

double readDouble(const std::string &prompt) {
    double value;
    cout << prompt;
    cin >> value;

    if (cin.eof()) {
        throw runtime_error("End of input.");
    }
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        throw runtime_error("Invalid number input.");
    }

    return value;
}

std::string readString(const std::string &prompt) {
    string value;
    cout << prompt;
    cin >> value;

    if (cin.eof()) {
        throw runtime_error("End of input.");
    }
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        throw runtime_error("Invalid text input.");
    }

    return value;
}

void staffMenu(Restaurant &restaurant) {
    bool running = true;

    while (running) {
        cout << "\n-- STAFF MENU --\n";
        cout << "1. Add Staff\n";
        cout << "2. View Staff\n";
        cout << "3. Update Staff\n";
        cout << "4. Delete Staff\n";
        cout << "5. Back\n";

        try {
            int choice = readInt("Choice: ");

            if (choice == 1) {
                cout << "Roles: 1=Manager 2=Chef 3=Waiter 4=HeadChef\n";
                int role = readInt("Role: ");
                string name = readString("Name: ");
                double salary = readDouble("Base salary: ");
                restaurant.addStaff(role, name, salary);
            } else if (choice == 2) {
                restaurant.viewStaff();
            } else if (choice == 3) {
                int id = readInt("Staff ID: ");
                string name = readString("New name: ");
                double salary = readDouble("New base salary: ");
                restaurant.updateStaff(id, name, salary);
            } else if (choice == 4) {
                int id = readInt("Staff ID: ");
                restaurant.deleteStaff(id);
            } else if (choice == 5) {
                running = false;
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (const exception &error) {
            if (cin.eof()) {
                throw;
            }
            cout << "Error: " << error.what() << endl;
        }
    }
}

void menuMenu(Restaurant &restaurant) {
    bool running = true;

    while (running) {
        cout << "\n-- MENU MANAGEMENT --\n";
        cout << "1. Add Dish\n";
        cout << "2. View Menu\n";
        cout << "3. Update Dish\n";
        cout << "4. Delete Dish\n";
        cout << "5. Back\n";

        try {
            int choice = readInt("Choice: ");

            if (choice == 1) {
                string name = readString("Dish name: ");
                double price = readDouble("Price: ");
                int stock = readInt("Stock (0 if none): ");

                if (stock == 0) {
                    restaurant.addDish(name, price);
                } else {
                    restaurant.addDish(name, price, stock);
                }
            } else if (choice == 2) {
                restaurant.viewMenu();
            } else if (choice == 3) {
                int id = readInt("Dish ID: ");
                double price = readDouble("New price: ");
                int stock = readInt("New stock: ");
                restaurant.updateDish(id, price, stock);
            } else if (choice == 4) {
                int id = readInt("Dish ID: ");
                restaurant.deleteDish(id);
            } else if (choice == 5) {
                running = false;
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (const exception &error) {
            if (cin.eof()) {
                throw;
            }
            cout << "Error: " << error.what() << endl;
        }
    }
}
