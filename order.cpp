#include "order.h"
#include <iostream>

namespace restaurant {

int Order::nextOrderId = 100;

Order::Order() : id(0), tableNo(0), waiterId(0), state(Placed) {}
Order::Order(int t, int w) : id(nextOrderId++), tableNo(t), waiterId(w), state(Placed) {}
Order::~Order() {}

int Order::getId() const { return id; }
int Order::getTableNo() const { return tableNo; }
int Order::getWaiterId() const { return waiterId; }
Order::State Order::getState() const { return state; }
const std::vector<OrderItem>& Order::getItems() const { return items; }

double Order::subtotal() const {
    double s = 0.0;
    for (size_t i = 0; i < items.size(); i++) {
        s = s + (items[i].price * items[i].qty);
    }
    return s;
}

void Order::operator+=(const OrderItem &item) {
    // Merge if same dish already in order
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].dishId == item.dishId) {
            items[i].qty = items[i].qty + item.qty;
            return;
        }
    }
    items.push_back(item);
}

void Order::advanceState() {
    switch (state) {
        case Placed:   state = Cooking; break;
        case Cooking:  state = Ready; break;
        case Ready:    state = Served; break;
        case Served:   state = Paid; break;
        case Paid:     break;
    }
}

void Order::markPaid() { state = Paid; }

bool Order::isPlaced() const { return state == Placed; }
bool Order::isCooking() const { return state == Cooking; }
bool Order::isReady() const { return state == Ready; }
bool Order::isServed() const { return state == Served; }
bool Order::isPaid() const { return state == Paid; }

std::ostream& operator<<(std::ostream &os, const Order &o) {
    os << "Order ID: " << o.id << " | Table: " << o.tableNo
       << " | Waiter ID: " << o.waiterId << " | State: ";
    switch (o.state) {
        case Order::Placed:   os << "Placed"; break;
        case Order::Cooking:  os << "Cooking"; break;
        case Order::Ready:    os << "Ready"; break;
        case Order::Served:   os << "Served"; break;
        case Order::Paid:     os << "Paid"; break;
    }
    os << " | Subtotal: Rs." << o.subtotal();
    return os;
}

}
