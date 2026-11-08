#ifndef ORDER_H
#define ORDER_H

#include <ostream>
#include <string>

const int MAX_ITEMS = 20;

struct OrderItem {
    int dishId;
    char name[32];
    double price;
    int qty;
};

class Order {
private:
    int id;
    int tableNo;
    int waiterId;
    int state;
    double billTotal;
    int itemCount;
    OrderItem items[MAX_ITEMS];

    static int nextId;

public:
    enum State {
        PLACED = 0,
        SERVED = 1,
        PAID = 2
    };

    Order();
    Order(int table, int waiter);

    int getId() const;
    int getWaiterId() const;
    int getTableNo() const;
    int getState() const;
    double subtotal() const;

    Order& operator+=(const OrderItem &item);

    void advanceState();
    void markPaid(double total);

    bool isPlaced() const;
    bool isServed() const;
    bool isPaid() const;

    int getItemCount() const;
    OrderItem getItem(int index) const;

    static int nextIdValue();
    static void setNextIdValue(int value);

    friend class Restaurant;
    friend std::ostream& operator<<(std::ostream &output, const Order &order);
};

class Bill {
private:
    int orderId;
    int waiterId;
    double subtotal;
    double discountPercent;
    double total;

public:
    Bill();
    Bill(int order, int waiter, double subTotal, double discount);

    double getTotal() const;

    friend std::ostream& operator<<(std::ostream &output, const Bill &bill);
};

#endif
