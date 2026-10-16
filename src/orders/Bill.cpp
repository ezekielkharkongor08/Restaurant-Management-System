#include "orders/Bill.h"
#include "payroll/TaxConfig.h"
#include "core/Serialization.h"

namespace restaurant {
namespace orders {

Bill::Bill()
    : id_(0), orderId_(0), tableNumber_(0), cashierId_(0), date_(1, 1, 2000),
      items_(), discountPercent_(0.0), gst_(0), discount_(0), tip_(0),
      total_(0), paid_(false) {}

Bill::Bill(int id, const Order& order)
    : id_(id), orderId_(order.id()), tableNumber_(order.tableNumber()),
      cashierId_(0), date_(order.date()), items_(order.items()),
      discountPercent_(0.0), gst_(0), discount_(0), tip_(0), total_(0),
      paid_(false) {
    recompute();
}

int Bill::id() const { return id_; }
int Bill::orderId() const { return orderId_; }
int Bill::tableNumber() const { return tableNumber_; }
int Bill::cashierId() const { return cashierId_; }
const core::Date& Bill::date() const { return date_; }

core::Money Bill::subtotal() const {
    core::Money sum(0);
    for (std::size_t i = 0; i < items_.size(); ++i) sum += items_[i].lineTotal();
    return sum;
}

core::Money Bill::gst() const { return gst_; }
core::Money Bill::discount() const { return discount_; }
core::Money Bill::tip() const { return tip_; }
core::Money Bill::total() const { return total_; }
bool Bill::paid() const { return paid_; }

void Bill::setCashierId(int cashierId) { cashierId_ = cashierId; }
void Bill::setDiscountPercent(double percent) {
    discountPercent_ = percent;
    recompute();
}
void Bill::setTip(const core::Money& tip) {
    tip_ = tip;
    recompute();
}
void Bill::markPaid() { paid_ = true; }

const std::vector<OrderItem>& Bill::items() const { return items_; }

void Bill::recompute() {
    core::Money sub = subtotal();
    discount_ = sub * (discountPercent_ / 100.0);
    gst_ = payroll::TaxConfig::gst(sub - discount_);
    total_ = sub - discount_ + gst_ + tip_;
}

Bill* Bill::clone() const { return new Bill(*this); }

void Bill::print(std::ostream& os) const {
    os << "=============================================\n";
    os << "BILL #" << id_ << "   order #" << orderId_
       << "   table " << tableNumber_ << "   " << date_ << "\n";
    os << "---------------------------------------------\n";
    for (std::size_t i = 0; i < items_.size(); ++i)
        os << "  " << items_[i] << "\n";
    os << "---------------------------------------------\n";
    os << "  Subtotal      : " << subtotal() << "\n";
    os << "  Discount (" << discountPercent_ << "%) : " << discount_ << "\n";
    os << "  GST           : " << gst_ << "\n";
    os << "  Tip           : " << tip_ << "\n";
    os << "  TOTAL PAYABLE : " << total_ << "\n";
    os << "  Status        : " << (paid_ ? "PAID" : "UNPAID") << "\n";
    os << "  Cashier #" << cashierId_ << "\n";
    os << "=============================================\n";
}

void Bill::write(std::ostream& os) const {
    core::writeValue(os, id_);
    core::writeValue(os, orderId_);
    core::writeValue(os, tableNumber_);
    core::writeValue(os, cashierId_);
    date_.write(os);
    core::writeValue(os, discountPercent_);
    gst_.write(os);
    discount_.write(os);
    tip_.write(os);
    total_.write(os);
    core::writeValue(os, paid_);
    std::uint64_t n = items_.size();
    core::writeValue(os, n);
    for (std::size_t i = 0; i < items_.size(); ++i) items_[i].write(os);
}

void Bill::read(std::istream& is) {
    core::readValue(is, id_);
    core::readValue(is, orderId_);
    core::readValue(is, tableNumber_);
    core::readValue(is, cashierId_);
    date_.read(is);
    core::readValue(is, discountPercent_);
    gst_.read(is);
    discount_.read(is);
    tip_.read(is);
    total_.read(is);
    core::readValue(is, paid_);
    std::uint64_t n = 0;
    core::readValue(is, n);
    items_.clear();
    for (std::uint64_t i = 0; i < n; ++i) {
        OrderItem item;
        item.read(is);
        items_.push_back(item);
    }
}

std::ostream& operator<<(std::ostream& os, const Bill& bill) {
    bill.print(os);
    return os;
}

} // namespace orders
} // namespace restaurant
