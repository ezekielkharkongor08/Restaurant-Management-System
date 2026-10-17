#include "manager.h"
#include "utils.h"
#include <iostream>
#include <cstring>

namespace restaurant {

const double Manager::ALLOWANCE = 5000.0;

Manager::Manager() : Staff("Manager", 0.0) {}
Manager::Manager(const std::string &n, double base) : Staff(n, base) {}
Manager::~Manager() {}

double Manager::getAllowance() { return ALLOWANCE; }

double Manager::calculatePay() const {
    return baseSalary + ALLOWANCE;
}
std::string Manager::duties() const { return "Runs the restaurant: hires/fires staff, payroll, menu, reports."; }
void Manager::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Manager"
       << " | Base: Rs." << baseSalary << " | Allowance: Rs." << ALLOWANCE
       << " | Gross Pay: Rs." << calculatePay();
}
int Manager::role() const { return 1; }
std::string Manager::roleName() const { return "Manager"; }
StaffRecord Manager::toRecord() const {
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
void Manager::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
}

// ---------------- HeadChef (Diamond Problem) ----------------
HeadChef::HeadChef() : Staff("HeadChef", 0.0), Chef("HeadChef", 0.0), Manager("HeadChef", 0.0) {}
HeadChef::HeadChef(const std::string &n, double base)
    : Staff(n, base), Chef(n, base), Manager(n, base) {}
HeadChef::~HeadChef() {}

double HeadChef::calculatePay() const {
    // Pay combines both parts: base counted once (virtual base) + allowance + per-dish rate
    return baseSalary + Manager::ALLOWANCE + (dishesCooked * 40.0);
}
std::string HeadChef::duties() const {
    return "Cooks orders and manages the kitchen (Chef + Manager).";
}
void HeadChef::print(std::ostream &os) const {
    os << "ID: " << id << " | Name: " << name << " | Role: Head Chef (Diamond)"
       << " | Base: Rs." << baseSalary << " | Allowance: Rs." << Manager::ALLOWANCE
       << " | Dishes Cooked: " << dishesCooked
       << " | Gross Pay: Rs." << calculatePay();
}
int HeadChef::role() const { return 6; }
std::string HeadChef::roleName() const { return "HeadChef"; }
StaffRecord HeadChef::toRecord() const {
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
void HeadChef::fromRecord(const StaffRecord &r) {
    id = r.id;
    name = r.name;
    baseSalary = r.baseSalary;
    daysPresent = r.daysPresent;
    overtimeHours = r.overtimeHours;
    dishesCooked = r.dishesCooked;
}

}
