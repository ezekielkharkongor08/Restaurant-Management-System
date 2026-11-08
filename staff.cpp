#include "staff.h"

#include <cstring>
#include <iostream>

using namespace std;

const double MANAGER_ALLOWANCE = 5000.0;

int Staff::nextId = 1;

Staff::Staff() {
    id = 0;
    name = "";
    baseSalary = 0.0;
}

Staff::Staff(const std::string &staffName, double salary) {
    id = nextId;
    nextId = nextId + 1;
    name = staffName;
    baseSalary = salary;
}

Staff::~Staff() {
}

int Staff::nextIdValue() {
    return nextId;
}

void Staff::setNextIdValue(int value) {
    nextId = value;
}

int Staff::getId() const {
    return id;
}

std::string Staff::getName() const {
    return name;
}

void Staff::setName(const std::string &staffName) {
    name = staffName;
}

void Staff::setBaseSalary(double salary) {
    baseSalary = salary;
}

StaffData Staff::toData() const {
    StaffData data;
    memset(&data, 0, sizeof(data));

    data.id = id;
    strncpy(data.name, name.c_str(), sizeof(data.name) - 1);
    data.name[sizeof(data.name) - 1] = '\0';
    data.role = getRole();
    data.baseSalary = baseSalary;
    data.active = 1;

    return data;
}

void Staff::loadFromData(const StaffData &data) {
    id = data.id;
    name = data.name;
    baseSalary = data.baseSalary;
}

// Chef
Chef::Chef() : Staff() {
}

Chef::Chef(const std::string &staffName, double salary)
    : Staff(staffName, salary) {
}

double Chef::calculateSalary() const {
    return baseSalary;
}

void Chef::display() const {
    cout << "ID: " << id << " | Name: " << name
         << " | Role: Chef | Salary: Rs." << calculateSalary();
}

Role Chef::getRole() const {
    return CHEF;
}

// Waiter
Waiter::Waiter() : Staff() {
}

Waiter::Waiter(const std::string &staffName, double salary)
    : Staff(staffName, salary) {
}

double Waiter::calculateSalary() const {
    return baseSalary;
}

void Waiter::display() const {
    cout << "ID: " << id << " | Name: " << name
         << " | Role: Waiter | Salary: Rs." << calculateSalary();
}

Role Waiter::getRole() const {
    return WAITER;
}

// Manager
Manager::Manager() : Staff() {
    allowance = MANAGER_ALLOWANCE;
}

Manager::Manager(const std::string &staffName, double salary)
    : Staff(staffName, salary) {
    allowance = MANAGER_ALLOWANCE;
}

double Manager::calculateSalary() const {
    return baseSalary + allowance;
}

void Manager::display() const {
    cout << "ID: " << id << " | Name: " << name
         << " | Role: Manager | Allowance: Rs." << allowance
         << " | Salary: Rs." << calculateSalary();
}

Role Manager::getRole() const {
    return MANAGER;
}

// HeadChef
HeadChef::HeadChef() : Staff(), Chef(), Manager() {
}

HeadChef::HeadChef(const std::string &staffName, double salary)
    : Staff(staffName, salary), Chef(staffName, salary), Manager(staffName, salary) {
}

double HeadChef::calculateSalary() const {
    return baseSalary + allowance;
}

void HeadChef::display() const {
    cout << "ID: " << id << " | Name: " << name
         << " | Role: HeadChef | Allowance: Rs." << allowance
         << " | Salary: Rs." << calculateSalary();
}

Role HeadChef::getRole() const {
    return HEAD_CHEF;
}
