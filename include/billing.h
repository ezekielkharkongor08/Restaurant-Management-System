#ifndef RMS_BILLING_H
#define RMS_BILLING_H

#include <string>
#include <ostream>
#include "order.h"

namespace restaurant {

class Bill {
private:
    int orderId;
    int waiterId;
    double subtotal;
    double discountPercent;
    double gstPercent;
    double tip;
    double total;
public:
    Bill();
    Bill(int oid, int wid, double sub, double disc, double gst, double t);

    int getOrderId() const;
    int getWaiterId() const;
    double getSubtotal() const;
    double getDiscountPercent() const;
    double getGSTPercent() const;
    double getTip() const;
    double getTotal() const;

    friend std::ostream& operator<<(std::ostream &os, const Bill &b);  // friend usage
};

}

#endif
