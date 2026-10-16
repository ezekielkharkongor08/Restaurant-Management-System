#include "staff/Cashier.h"
#include "payroll/MonthlySalary.h"
#include "core/Serialization.h"
#include <iostream>

namespace restaurant {
namespace staff {

Cashier::Cashier() : Staff(), overtimeRate_(core::Money::fromRupees(150)), billsIssued_(0) {
    setPayPolicy(new payroll::MonthlySalary());
}

Cashier::Cashier(int id, const std::string& name, const std::string& phone,
                 const std::string& address, const core::Date& joinDate,
                 const core::Money& baseSalary, const core::Money& overtimeRate)
    : Staff(id, name, phone, address, joinDate, baseSalary),
      overtimeRate_(overtimeRate), billsIssued_(0) {
    setPayPolicy(new payroll::MonthlySalary());
}

Cashier::~Cashier() {}

core::Money Cashier::overtimeRate() const { return overtimeRate_; }
void Cashier::setOvertimeRate(const core::Money& rate) { overtimeRate_ = rate; }

int Cashier::billsIssued() const { return billsIssued_; }
void Cashier::addBillIssued() { ++billsIssued_; }

std::string Cashier::roleName() const { return "Cashier"; }

void Cashier::showDuties() const {
    std::cout << "Cashier duties: create bills, take payment and close orders.\n";
}

core::Money Cashier::calculatePay() const {
    core::Money base = policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
    return base + overtimeRate_ * attendance_.overtimeHours();
}

Staff* Cashier::clone() const { return new Cashier(*this); }
int Cashier::typeTag() const { return core::TAG_CASHIER; }

void Cashier::write(std::ostream& os) const {
    Staff::write(os);
    writeOwn(os);
}

void Cashier::writeOwn(std::ostream& os) const {
    overtimeRate_.write(os);
    core::writeValue(os, billsIssued_);
}

void Cashier::read(std::istream& is) {
    Staff::read(is);
    readOwn(is);
}

void Cashier::readOwn(std::istream& is) {
    overtimeRate_.read(is);
    core::readValue(is, billsIssued_);
}

} // namespace staff
} // namespace restaurant
