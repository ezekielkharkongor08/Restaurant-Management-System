#pragma once
#include "staff/Staff.h"

namespace restaurant {
namespace ui {

class RestaurantSystem;

// Each role sees only the actions it is allowed to perform.
class ManagerMenu {
public:
    static void run(RestaurantSystem& system, staff::Staff& user);
};

class CashierMenu {
public:
    static void run(RestaurantSystem& system, staff::Staff& user);
};

class WaiterMenu {
public:
    static void run(RestaurantSystem& system, staff::Staff& user);
};

class ChefMenu {
public:
    static void run(RestaurantSystem& system, staff::Staff& user);
};

class CleanerMenu {
public:
    static void run(RestaurantSystem& system, staff::Staff& user);
};

} // namespace ui
} // namespace restaurant
