#ifndef MENU_H
#define MENU_H

#include <ostream>
#include <string>

class Dish {
private:
    int id;
    char name[32];
    double price;
    int stock;
    int active;

    static int nextId;

public:
    Dish();
    Dish(const std::string &dishName, double dishPrice, int dishStock = 0);

    static int nextIdValue();
    static void setNextIdValue(int value);

    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getStock() const;
    bool isActive() const;

    void setPrice(double dishPrice);
    void setStock(int dishStock);
    void setActive(bool status);

    bool reduceStock(int quantity);

    bool operator<(const Dish &other) const;

    friend std::ostream& operator<<(std::ostream &output, const Dish &dish);
};

#endif
