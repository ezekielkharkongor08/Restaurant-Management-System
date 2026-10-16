#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "core/Date.h"
#include "interfaces/Interfaces.h"
#include "orders/OrderItem.h"

namespace restaurant {
namespace orders {

enum OrderStatus {
    ORDER_PLACED = 0,
    ORDER_COOKING = 1,
    ORDER_READY = 2,
    ORDER_SERVED = 3,
    ORDER_PAID = 4
};

std::string orderStatusName(OrderStatus status);

// Order is a full order for one table.  It keeps a vector<OrderItem> and a
// status that moves Placed -> Cooking -> Ready -> Served -> Paid.
// operator+= adds an item, operator[] gets an item by position and operator<<
// prints the whole order.
class Order : public interfaces::Printable, public interfaces::Serializable {
public:
    Order();
    Order(int id, int tableNumber, int waiterId, const core::Date& date);

    int id() const;
    int tableNumber() const;
    int waiterId() const;
    int chefId() const;
    OrderStatus status() const;
    const core::Date& date() const;
    const std::vector<OrderItem>& items() const;

    void setWaiterId(int waiterId);
    void setChefId(int chefId);
    void setStatus(OrderStatus status);
    void setTableNumber(int tableNumber);

    Order& operator+=(const OrderItem& item);      // order += item;
    const OrderItem& operator[](std::size_t index) const;  // order[i]

    bool empty() const;
    std::size_t itemCount() const;
    core::Money total() const;

    Order* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int id_;
    int tableNumber_;
    int waiterId_;
    int chefId_;
    OrderStatus status_;
    core::Date date_;
    std::vector<OrderItem> items_;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

} // namespace orders
} // namespace restaurant
