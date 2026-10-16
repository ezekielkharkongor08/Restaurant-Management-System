// ============================================================================
// Restaurant Management System - single entry point.
// Everything is started from here: the console object is created and its run()
// method shows the login screen and then the correct role menu.
// ============================================================================
#include "ui/RestaurantSystem.h"
#include <iostream>
#include <exception>

int main() {
    try {
        restaurant::ui::RestaurantSystem system("data");
        system.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
