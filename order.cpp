#include "order.h"

#include <cstring>
#include <iostream>
#include "exceptions.h"

using namespace std;

const double PERCENT_DIVISOR = 100.0;

int Order::nextId = 100;

Order::Order() {
    id = 0;
    tableNo = 0;
    waiterId = 0;
    state = PLACED;
    billTotal = 0.0;
    itemCount = 0;
    memset(items, 0, sizeof(items));
}

Order::Order(int table, int waiter) {
    id = nextId;
    nextId = nextId + 1;
    tableNo = table;
    waiterId = waiter;
    state = PLACED;
    billTotal = 0.0;
    itemCount = 0;
    memset(items, 0, sizeof(items));
}

int Order::getId() const {
    return id;
}

int Order::getWaiterId() const {
    return waiterId;
}

int Order::getTableNo() const {
    return tableNo;
}

int Order::getState() const {
    return state;
}

double Order::subtotal() const {
    double sum = 0.0;
    for (int i = 0; i < itemCount; i++) {
        sum = sum + items[i].price * items[i].qty;
    }
    return sum;
}

Order& Order::operator+=(const OrderItem &item) {
    for (int i = 0; i < itemCount; i++) {
        if (items[i].dishId == item.dishId) {
            items[i].qty = items[i].qty + item.qty;
            return *this;
        }
    }

    if (itemCount >= MAX_ITEMS) {
        throw RestaurantException("Order item limit reached.");
    }

    items[itemCount] = item;
    itemCount = itemCount + 1;
    return *this;
}

void Order::advanceState() {
    if (state == PLACED) {
        state = SERVED;
    }
}

void Order::markPaid(double total) {
    state = PAID;
    billTotal = total;
}

bool Order::isPlaced() const {
    return state == PLACED;
}

bool Order::isServed() const {
    return state == SERVED;
}

bool Order::isPaid() const {
    return state == PAID;
}

int Order::getItemCount() const {
    return itemCount;
}

OrderItem Order::getItem(int index) const {
    if (index >= 0 && index < itemCount) {
        return items[index];
    }
    OrderItem emptyItem;
    memset(&emptyItem, 0, sizeof(emptyItem));
    return emptyItem;
}

int Order::nextIdValue() {
    return nextId;
}

void Order::setNextIdValue(int value) {
    nextId = value;
}

std::ostream& operator<<(std::ostream &output, const Order &order) {
    output << "Order ID: " << order.id
           << " | Table: " << order.tableNo
           << " | Waiter ID: " << order.waiterId
           << " | Status: ";

    if (order.state == Order::PLACED) {
        output << "Placed";
    } else if (order.state == Order::SERVED) {
        output << "Served";
    } else {
        output << "Paid";
    }

    output << " | Subtotal: Rs." << order.subtotal();
    if (order.state == Order::PAID) {
        output << " | Bill: Rs." << order.billTotal;
    }

    return output;
}

// Bill
Bill::Bill() {
    orderId = 0;
    waiterId = 0;
    subtotal = 0.0;
    discountPercent = 0.0;
    total = 0.0;
}

Bill::Bill(int order, int waiter, double subTotal, double discount) {
    orderId = order;
    waiterId = waiter;
    subtotal = subTotal;
    discountPercent = discount;

    double discountAmount = subtotal * discountPercent / PERCENT_DIVISOR;
    total = subtotal - discountAmount;
}

double Bill::getTotal() const {
    return total;
}

std::ostream& operator<<(std::ostream &output, const Bill &bill) {
    double discountAmount = bill.subtotal * bill.discountPercent / PERCENT_DIVISOR;

    output << "\n===== BILL =====\n";
    output << "Order ID: " << bill.orderId
           << " | Waiter ID: " << bill.waiterId << "\n";
    output << "Subtotal:        Rs." << bill.subtotal << "\n";
    output << "Discount (" << bill.discountPercent << "%): -Rs."
           << discountAmount << "\n";
    output << "----------------\n";
    output << "TOTAL:           Rs." << bill.total << "\n";
    output << "================\n";

    return output;
}
