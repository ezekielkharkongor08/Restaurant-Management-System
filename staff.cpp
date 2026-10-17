#include "staff.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <cstring>

namespace restaurant {

int Staff::nextId = 1;

Staff::Staff() : id(0), name(""), baseSalary(0.0), daysPresent(0), overtimeHours(0.0) {}

Staff::Staff(const std::string &n, double base)
    : id(nextId++), name(n), baseSalary(base), daysPresent(0), overtimeHours(0.0) {}

Staff::Staff(const Staff &other) {
    id = other.id;
    name = other.name;
    baseSalary = other.baseSalary;
    daysPresent = other.daysPresent;
    overtimeHours = other.overtimeHours;
}

Staff& Staff::operator=(const Staff &other) {
    if (this == &other) return *this;
    id = other.id;
    name = other.name;
    baseSalary = other.baseSalary;
    daysPresent = other.daysPresent;
    overtimeHours = other.overtimeHours;
    return *this;
}

Staff::~Staff() {}

int Staff::getId() const { return id; }
const std::string& Staff::getName() const { return name; }
double Staff::getBaseSalary() const { return baseSalary; }
int Staff::getDaysPresent() const { return daysPresent; }
double Staff::getOvertimeHours() const { return overtimeHours; }
bool Staff::isActive() const { return true; }   // base: active by default

void Staff::setName(const std::string &n) { name = n; }
void Staff::setBaseSalary(double b) { baseSalary = b; }
void Staff::setDaysPresent(int d) { daysPresent = d; }
void Staff::setOvertimeHours(double o) { overtimeHours = o; }
void Staff::setActive(bool a) { (void)a; }

// ---------------- Chef ----------------
Chef::Chef() : Staff("Chef", 0.0), dishesCooked(0) {}
Chef::Chef(const std::string &n, double base) : Staff(n, base), dishesCooked(0) {}
Chef::~Chef() {}

int Chef::getDishesCooked() const { return dishesCooked; }
void Chef::setDishesCooked(int d) { dishesCooked = d; }

double Chef::calculatePay() const {
    return baseSalary + (dishesCooked * 40.0);   // rate per dish cooked
}
std::string Chef::duties() const { return "Cooks orders and updates the cooking queue."; }
void Chef::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Chef"
       << " | Base: Rs." << baseSalary << " | Dishes Cooked: " << dishesCooked
       << " | Gross Pay: Rs." << calculatePay();
}
int Chef::role() const { return 2; }
std::string Chef::roleName() const { return "Chef"; }
StaffRecord Chef::toRecord() const {
    StaffRecord r;
    clearStaffRecord(r);
    r.id = id;
    copyToCharArray(name, r.name, 32);
    r.role = role();
    r.baseSalary = baseSalary;
    r.daysPresent = daysPresent;
    r.overtimeHours = overtimeHours;
    r.tips = 0.0;
    r.dishesCooked = dishesCooked;
    r.active = true;
    return r;
}
void Chef::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
    dishesCooked = r.dishesCooked;
}

// ---------------- Waiter ----------------
Waiter::Waiter() : Staff("Waiter", 0.0), tips(0.0) {}
Waiter::Waiter(const std::string &n, double base) : Staff(n, base), tips(0.0) {}
Waiter::~Waiter() {}

double Waiter::getTips() const { return tips; }
void Waiter::addTips(double t) { tips += t; }

double Waiter::calculatePay() const {
    return baseSalary + tips;
}
std::string Waiter::duties() const { return "Takes orders at tables, serves food, receives tips."; }
void Waiter::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Waiter"
       << " | Base: Rs." << baseSalary << " | Tips: Rs." << tips
       << " | Gross Pay: Rs." << calculatePay();
}
int Waiter::role() const { return 3; }
std::string Waiter::roleName() const { return "Waiter"; }
StaffRecord Waiter::toRecord() const {
    StaffRecord r;
    clearStaffRecord(r);
    r.id = id;
    copyToCharArray(name, r.name, 32);
    r.role = role();
    r.baseSalary = baseSalary;
    r.daysPresent = daysPresent;
    r.overtimeHours = overtimeHours;
    r.tips = tips;
    r.dishesCooked = 0;
    r.active = true;
    return r;
}
void Waiter::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
    tips = r.tips;
}

// ---------------- Cashier ----------------
Cashier::Cashier() : Staff("Cashier", 0.0) {}
Cashier::Cashier(const std::string &n, double base) : Staff(n, base) {}
Cashier::~Cashier() {}

double Cashier::calculatePay() const {
    return baseSalary + (overtimeHours * 30.0);   // overtime pay
}
std::string Cashier::duties() const { return "Creates bills, takes payment, closes orders."; }
void Cashier::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Cashier"
       << " | Base: Rs." << baseSalary << " | Overtime Hrs: " << overtimeHours
       << " | Gross Pay: Rs." << calculatePay();
}
int Cashier::role() const { return 4; }
std::string Cashier::roleName() const { return "Cashier"; }
StaffRecord Cashier::toRecord() const {
    StaffRecord r;
    clearStaffRecord(r);
    r.id = id;
    copyToCharArray(name, r.name, 32);
    r.role = role();
    r.baseSalary = baseSalary;
    r.daysPresent = daysPresent;
    r.overtimeHours = overtimeHours;
    r.tips = 0.0;
    r.dishesCooked = 0;
    r.active = true;
    return r;
}
void Cashier::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
}

// ---------------- Cleaner ----------------
Cleaner::Cleaner() : Staff("Cleaner", 0.0) {}
Cleaner::Cleaner(const std::string &n, double base) : Staff(n, base) {}
Cleaner::~Cleaner() {}

double Cleaner::calculatePay() const {
    return baseSalary * daysPresent / 26.0;   // based on days present
}
std::string Cleaner::duties() const { return "Cleans tables and kitchen, follows cleaning schedule."; }
void Cleaner::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Cleaner"
       << " | Base: Rs." << baseSalary << " | Days Present: " << daysPresent
       << " | Gross Pay: Rs." << calculatePay();
}
int Cleaner::role() const { return 5; }
std::string Cleaner::roleName() const { return "Cleaner"; }
StaffRecord Cleaner::toRecord() const {
    StaffRecord r;
    clearStaffRecord(r);
    r.id = id;
    copyToCharArray(name, r.name, 32);
    r.role = role();
    r.baseSalary = baseSalary;
    r.daysPresent = daysPresent;
    r.overtimeHours = overtimeHours;
    r.tips = 0.0;
    r.dishesCooked = 0;
    r.active = true;
    return r;
}
void Cleaner::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
}

}
