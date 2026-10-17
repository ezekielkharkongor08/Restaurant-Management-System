#include "billing.h"
#include <iostream>
#include <iomanip>

namespace restaurant {

Bill::Bill() : orderId(0), waiterId(0), subtotal(0.0), discountPercent(0.0), gstPercent(5.0), tip(0.0), total(0.0) {}
Bill::Bill(int oid, int wid, double sub, double disc, double gst, double t)
    : orderId(oid), waiterId(wid), subtotal(sub), discountPercent(disc), gstPercent(gst), tip(t) {
    double afterDisc = subtotal - (subtotal * discountPercent / 100.0);
    double gstAmt = afterDisc * gstPercent / 100.0;
    total = afterDisc + gstAmt + tip;
}

int Bill::getOrderId() const { return orderId; }
int Bill::getWaiterId() const { return waiterId; }
double Bill::getSubtotal() const { return subtotal; }
double Bill::getDiscountPercent() const { return discountPercent; }
double Bill::getGSTPercent() const { return gstPercent; }
double Bill::getTip() const { return tip; }
double Bill::getTotal() const { return total; }

std::ostream& operator<<(std::ostream &os, const Bill &b) {
    double afterDisc = b.subtotal - (b.subtotal * b.discountPercent / 100.0);
    double gstAmt = afterDisc * b.gstPercent / 100.0;
    os << "\n===== BILL =====\n";
    os << "Order ID: " << b.orderId << " | Waiter ID: " << b.waiterId << "\n";
    os << "Subtotal:        Rs." << b.subtotal << "\n";
    os << "Discount (" << b.discountPercent << "%): -Rs." << (b.subtotal * b.discountPercent / 100.0) << "\n";
    os << "GST (5%):        +Rs." << gstAmt << "\n";
    os << "Tip:             +Rs." << b.tip << "\n";
    os << "----------------\n";
    os << "TOTAL:           Rs." << b.total << "\n";
    os << "================\n";
    return os;
}

}
