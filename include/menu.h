#ifndef RMS_MENU_H
#define RMS_MENU_H

#include <string>
#include <ostream>
#include "records.h"

namespace restaurant {

class Dish {
private:
    int id;
    std::string name;
    double price;
    int stock;
    bool active;
    static int nextDishId;                     // static member: auto-increasing dish id
public:
    Dish();
    Dish(const std::string &n, double p, int s);
    Dish(const Dish &other);                   // copy constructor
    Dish& operator=(const Dish &other);        // assignment operator
    ~Dish();

    static int nextDishIdValue() { return nextDishId; }
    static void setNextDishIdValue(int v) { nextDishId = v; }

    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    int getStock() const;
    bool isActive() const;

    void setName(const std::string &n);
    void setPrice(double p);
    void setStock(int s);
    void setActive(bool a);

    // Reduce stock when an order is placed; out of stock -> error handled by caller
    bool reduceStock(int qty);

    // Operator overloading: compare by price (used by sortItems template)
    bool operator<(const Dish &other) const;

    DishRecord toRecord() const;
    void fromRecord(const DishRecord &r);

    friend std::ostream& operator<<(std::ostream &os, const Dish &d);  // friend usage
};

}

#endif
