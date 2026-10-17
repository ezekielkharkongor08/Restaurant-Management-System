#ifndef RMS_RECORDS_H
#define RMS_RECORDS_H

#include <cstring>

namespace restaurant {

// Fixed-size binary record for staff (no std::string inside -> fixed sizeof)
struct StaffRecord {
    int id;
    char name[32];
    int role;          // 1=Manager, 2=Chef, 3=Waiter, 4=Cashier, 5=Cleaner, 6=HeadChef
    double baseSalary;
    int daysPresent;
    double overtimeHours;
    double tips;
    int dishesCooked;
    bool active;
};

// Fixed-size binary record for menu dish
struct DishRecord {
    int id;
    char name[32];
    double price;
    int stock;
    bool active;
};

inline void clearStaffRecord(StaffRecord &r) {
    r.id = 0;
    std::memset(r.name, 0, sizeof(r.name));
    r.role = 0;
    r.baseSalary = 0.0;
    r.daysPresent = 0;
    r.overtimeHours = 0.0;
    r.tips = 0.0;
    r.dishesCooked = 0;
    r.active = false;
}

inline void clearDishRecord(DishRecord &r) {
    r.id = 0;
    std::memset(r.name, 0, sizeof(r.name));
    r.price = 0.0;
    r.stock = 0;
    r.active = false;
}

}

#endif
