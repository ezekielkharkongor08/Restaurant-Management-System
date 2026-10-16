#include "orders/Order.h"
#include "core/Exceptions.h"
#include "core/Serialization.h"

namespace restaurant {
namespace orders {

std::string orderStatusName(OrderStatus status) {
    switch (status) {
        case ORDER_PLACED:  return "Placed";
        case ORDER_COOKING: return "Cooking";
        case ORDER_READY:   return "Ready";
        case ORDER_SERVED:  return "Served";
        case ORDER_PAID:    return "Paid";
        default:            return "Unknown";
    }
}

Order::Order()
    : id_(0), tableNumber_(0), waiterId_(0), chefId_(0),
      status_(ORDER_PLACED), date_(1, 1, 2000), items_() {}

Order::Order(int id, int tableNumber, int waiterId, const core::Date& date)
    : id_(id), tableNumber_(tableNumber), waiterId_(waiterId), chefId_(0),
      status_(ORDER_PLACED), date_(date), items_() {}

int Order::id() const { return id_; }
int Order::tableNumber() const { return tableNumber_; }
int Order::waiterId() const { return waiterId_; }
int Order::chefId() const { return chefId_; }
OrderStatus Order::status() const { return status_; }
const core::Date& Order::date() const { return date_; }
const std::vector<OrderItem>& Order::items() const { return items_; }

void Order::setWaiterId(int waiterId) { waiterId_ = waiterId; }
void Order::setChefId(int chefId) { chefId_ = chefId; }
void Order::setStatus(OrderStatus status) { status_ = status; }
void Order::setTableNumber(int tableNumber) { tableNumber_ = tableNumber; }

Order& Order::operator+=(const OrderItem& item) {
    items_.push_back(item);
    return *this;
}

const OrderItem& Order::operator[](std::size_t index) const {
    if (index >= items_.size())
        throw core::InvalidInputException("Order item index out of range");
    return items_[index];
}

bool Order::empty() const { return items_.empty(); }
std::size_t Order::itemCount() const { return items_.size(); }

core::Money Order::total() const {
    core::Money sum(0);
    for (std::size_t i = 0; i < items_.size(); ++i) sum += items_[i].lineTotal();
    return sum;
}

Order* Order::clone() const { return new Order(*this); }

void Order::print(std::ostream& os) const {
    os << "Order #" << id_ << "  table " << tableNumber_
       << "  waiter #" << waiterId_
       << "  chef #" << chefId_
       << "  status " << orderStatusName(status_)
       << "  date " << date_ << "\n";
    for (std::size_t i = 0; i < items_.size(); ++i)
        os << "    " << items_[i] << "\n";
    os << "  TOTAL: " << total();
}

void Order::write(std::ostream& os) const {
    core::writeValue(os, id_);
    core::writeValue(os, tableNumber_);
    core::writeValue(os, waiterId_);
    core::writeValue(os, chefId_);
    int st = static_cast<int>(status_);
    core::writeValue(os, st);
    date_.write(os);
    std::uint64_t n = items_.size();
    core::writeValue(os, n);
    for (std::size_t i = 0; i < items_.size(); ++i) items_[i].write(os);
}

void Order::read(std::istream& is) {
    core::readValue(is, id_);
    core::readValue(is, tableNumber_);
    core::readValue(is, waiterId_);
    core::readValue(is, chefId_);
    int st = 0;
    core::readValue(is, st);
    status_ = static_cast<OrderStatus>(st);
    date_.read(is);
    std::uint64_t n = 0;
    core::readValue(is, n);
    items_.clear();
    for (std::uint64_t i = 0; i < n; ++i) {
        OrderItem item;
        item.read(is);
        items_.push_back(item);
    }
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    order.print(os);
    return os;
}

} // namespace orders
} // namespace restaurant
