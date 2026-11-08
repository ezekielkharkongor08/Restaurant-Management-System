#include "menu.h"

#include <cstring>
#include <iostream>

using namespace std;

int Dish::nextId = 1;

Dish::Dish() {
    id = 0;
    memset(name, 0, sizeof(name));
    price = 0.0;
    stock = 0;
    active = 1;
}

Dish::Dish(const std::string &dishName, double dishPrice, int dishStock) {
    id = nextId;
    nextId = nextId + 1;
    memset(name, 0, sizeof(name));
    strncpy(name, dishName.c_str(), sizeof(name) - 1);
    price = dishPrice;
    stock = dishStock;
    active = 1;
}

int Dish::nextIdValue() {
    return nextId;
}

void Dish::setNextIdValue(int value) {
    nextId = value;
}

int Dish::getId() const {
    return id;
}

std::string Dish::getName() const {
    return name;
}

double Dish::getPrice() const {
    return price;
}

int Dish::getStock() const {
    return stock;
}

bool Dish::isActive() const {
    return active != 0;
}

void Dish::setPrice(double dishPrice) {
    price = dishPrice;
}

void Dish::setStock(int dishStock) {
    stock = dishStock;
}

void Dish::setActive(bool status) {
    active = status ? 1 : 0;
}

bool Dish::reduceStock(int quantity) {
    if (quantity <= 0 || stock < quantity) {
        return false;
    }
    stock = stock - quantity;
    return true;
}

bool Dish::operator<(const Dish &other) const {
    return price < other.price;
}

std::ostream& operator<<(std::ostream &output, const Dish &dish) {
    output << "ID: " << dish.id
           << " | " << dish.name
           << " | Price: Rs." << dish.price
           << " | Stock: " << dish.stock;
    return output;
}
