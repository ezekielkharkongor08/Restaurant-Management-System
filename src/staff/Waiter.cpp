#include "staff/Waiter.h"
#include "payroll/CommissionPay.h"
#include "core/Serialization.h"
#include <iostream>

namespace restaurant {
namespace staff {

Waiter::Waiter() : Staff(), ordersTaken_(0) {
    setPayPolicy(new payroll::CommissionPay(1.0));  // base salary + 100% of tips
}

Waiter::Waiter(int id, const std::string& name, const std::string& phone,
               const std::string& address, const core::Date& joinDate,
               const core::Money& baseSalary)
    : Staff(id, name, phone, address, joinDate, baseSalary), ordersTaken_(0) {
    setPayPolicy(new payroll::CommissionPay(1.0));
}

Waiter::~Waiter() {}

int Waiter::ordersTaken() const { return ordersTaken_; }
void Waiter::addOrderTaken() { ++ordersTaken_; }

core::Money Waiter::totalTips() const { return attendance_.tipsEarned(); }
void Waiter::addTip(const core::Money& tip) { attendance_.addTips(tip); }

std::string Waiter::roleName() const { return "Waiter"; }

void Waiter::showDuties() const {
    std::cout << "Waiter duties: take orders at tables, serve food and collect "
                 "tips.\n";
}

core::Money Waiter::calculatePay() const {
    return policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
}

Staff* Waiter::clone() const { return new Waiter(*this); }
int Waiter::typeTag() const { return core::TAG_WAITER; }

void Waiter::write(std::ostream& os) const {
    Staff::write(os);
    writeOwn(os);
}

void Waiter::writeOwn(std::ostream& os) const {
    core::writeValue(os, ordersTaken_);
}

void Waiter::read(std::istream& is) {
    Staff::read(is);
    readOwn(is);
}

void Waiter::readOwn(std::istream& is) {
    core::readValue(is, ordersTaken_);
}

} // namespace staff
} // namespace restaurant
