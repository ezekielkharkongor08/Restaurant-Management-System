#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "core/Date.h"
#include "interfaces/Interfaces.h"
#include "orders/OrderItem.h"
#include "orders/Order.h"

namespace restaurant {
namespace orders {

// Bill is made from an order.  It adds GST, applies a discount, adds a tip and
// gives the final amount.  It is printable and is saved to file.
class Bill : public interfaces::Printable, public interfaces::Serializable {
public:
    Bill();
    Bill(int id, const Order& order);

    int id() const;
    int orderId() const;
    int tableNumber() const;
    int cashierId() const;
    const core::Date& date() const;

    core::Money subtotal() const;
    core::Money gst() const;
    core::Money discount() const;
    core::Money tip() const;
    core::Money total() const;
    bool paid() const;

    void setCashierId(int cashierId);
    void setDiscountPercent(double percent);
    void setTip(const core::Money& tip);
    void markPaid();

    const std::vector<OrderItem>& items() const;

    Bill* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    void recompute();

    int id_;
    int orderId_;
    int tableNumber_;
    int cashierId_;
    core::Date date_;
    std::vector<OrderItem> items_;
    double discountPercent_;
    core::Money gst_;
    core::Money discount_;
    core::Money tip_;
    core::Money total_;
    bool paid_;
};

std::ostream& operator<<(std::ostream& os, const Bill& bill);

} // namespace orders
} // namespace restaurant
