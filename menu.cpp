#include "menu.h"
#include "utils.h"
#include <iostream>
#include <cstring>

namespace restaurant {

int Dish::nextDishId = 1;

Dish::Dish() : id(0), name(""), price(0.0), stock(0), active(false) {}
Dish::Dish(const std::string &n, double p, int s)
    : id(nextDishId++), name(n), price(p), stock(s), active(true) {}
Dish::Dish(const Dish &other) {
    id = other.id;
    name = other.name;
    price = other.price;
    stock = other.stock;
    active = other.active;
}
Dish& Dish::operator=(const Dish &other) {
    if (this == &other) return *this;
    id = other.id;
    name = other.name;
    price = other.price;
    stock = other.stock;
    active = other.active;
    return *this;
}
Dish::~Dish() {}

int Dish::getId() const { return id; }
const std::string& Dish::getName() const { return name; }
double Dish::getPrice() const { return price; }
int Dish::getStock() const { return stock; }
bool Dish::isActive() const { return active; }

void Dish::setName(const std::string &n) { name = n; }
void Dish::setPrice(double p) { price = p; }
void Dish::setStock(int s) { stock = s; }
void Dish::setActive(bool a) { active = a; }

bool Dish::reduceStock(int qty) {
    if (stock < qty) return false;
    stock -= qty;
    return true;
}

bool Dish::operator<(const Dish &other) const {
    return price < other.price;
}

DishRecord Dish::toRecord() const {
    DishRecord r;
    clearDishRecord(r);
    r.id = id;
    copyToCharArray(name, r.name, 32);
    r.price = price;
    r.stock = stock;
    r.active = active;
    return r;
}
void Dish::fromRecord(const DishRecord &r) {
    id = r.id;
    name = r.name;
    price = r.price;
    stock = r.stock;
    active = r.active;
}

std::ostream& operator<<(std::ostream &os, const Dish &d) {
    os << "ID: " << d.id << " | " << d.name
       << " | Price: Rs." << d.price
       << " | Stock: " << d.stock
       << " | " << (d.active ? "Active" : "Inactive");
    return os;
}

}
