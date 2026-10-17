#include <iostream>
#include "restaurant.h"
#include "exceptions.h"

namespace restaurant {
    void runApp(Restaurant &r);   // defined in operations.cpp
}

int main() {
    try {
        restaurant::Restaurant rms;
        restaurant::runApp(rms);
    } catch (const restaurant::RestaurantException &e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error.\n";
        return 1;
    }
    return 0;
}
