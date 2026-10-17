#ifndef RMS_ORDER_H
#define RMS_ORDER_H

#include <string>
#include <vector>
#include <ostream>

namespace restaurant {

struct OrderItem {
    int dishId;
    std::string name;
    double price;
    int qty;
};

class Order {
private:
    int id;
    int tableNo;
    int waiterId;
    std::vector<OrderItem> items;
    enum State { Placed, Cooking, Ready, Served, Paid };
    State state;
    static int nextOrderId;                    // static member: auto-increasing order id
public:
    Order();
    Order(int t, int w);
    ~Order();

    int getId() const;
    int getTableNo() const;
    int getWaiterId() const;
    State getState() const;
    const std::vector<OrderItem>& getItems() const;
    double subtotal() const;

    // Operator overloading: add an item to an order with +=
    void operator+=(const OrderItem &item);
    void advanceState();
    void markPaid();

    bool isPlaced() const;
    bool isCooking() const;
    bool isReady() const;
    bool isServed() const;
    bool isPaid() const;

    friend std::ostream& operator<<(std::ostream &os, const Order &o);
};

}

#endif
