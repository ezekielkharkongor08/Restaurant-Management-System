#include "staff/ShiftSupervisor.h"
#include "payroll/CommissionPay.h"
#include <iostream>

namespace restaurant {
namespace staff {

ShiftSupervisor::ShiftSupervisor() : Staff(), Waiter(), Cashier() {
    setPayPolicy(new payroll::CommissionPay(1.0));
}

ShiftSupervisor::ShiftSupervisor(int id, const std::string& name,
                                 const std::string& phone, const std::string& address,
                                 const core::Date& joinDate, const core::Money& baseSalary,
                                 const core::Money& overtimeRate)
    : Staff(id, name, phone, address, joinDate, baseSalary), Waiter(), Cashier() {
    overtimeRate_ = overtimeRate;
    setPayPolicy(new payroll::CommissionPay(1.0));
}

ShiftSupervisor::~ShiftSupervisor() {}

std::string ShiftSupervisor::roleName() const { return "Shift Supervisor"; }

void ShiftSupervisor::showDuties() const {
    std::cout << "Shift Supervisor duties: take orders at tables AND handle "
                 "billing during the shift.\n";
}

core::Money ShiftSupervisor::calculatePay() const {
    core::Money base = policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
    return base + overtimeRate_ * attendance_.overtimeHours();
}

Staff* ShiftSupervisor::clone() const { return new ShiftSupervisor(*this); }
int ShiftSupervisor::typeTag() const { return core::TAG_SHIFT_SUPERVISOR; }

void ShiftSupervisor::write(std::ostream& os) const {
    Staff::write(os);
    Waiter::writeOwn(os);
    Cashier::writeOwn(os);
}

void ShiftSupervisor::read(std::istream& is) {
    Staff::read(is);
    Waiter::readOwn(is);
    Cashier::readOwn(is);
}

} // namespace staff
} // namespace restaurant
