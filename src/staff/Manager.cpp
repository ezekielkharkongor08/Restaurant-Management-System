#include "staff/Manager.h"
#include "payroll/MonthlySalary.h"
#include "core/Serialization.h"
#include <iostream>

namespace restaurant {
namespace staff {

Manager::Manager() : Staff(), allowance_(0), canApproveLeave_(true) {
    setPayPolicy(new payroll::MonthlySalary());
}

Manager::Manager(int id, const std::string& name, const std::string& phone,
                 const std::string& address, const core::Date& joinDate,
                 const core::Money& baseSalary, const core::Money& allowance)
    : Staff(id, name, phone, address, joinDate, baseSalary),
      allowance_(allowance), canApproveLeave_(true) {
    setPayPolicy(new payroll::MonthlySalary());
}

Manager::~Manager() {}

core::Money Manager::allowance() const { return allowance_; }
void Manager::setAllowance(const core::Money& allowance) { allowance_ = allowance; }

bool Manager::canApproveLeave() const { return canApproveLeave_; }
void Manager::setCanApproveLeave(bool value) { canApproveLeave_ = value; }

std::string Manager::roleName() const { return "Manager"; }

void Manager::showDuties() const {
    std::cout << "Manager duties: approve leave, assign shifts, run payroll, "
                 "hire/fire staff and view all reports.\n";
}

core::Money Manager::calculatePay() const {
    core::Money pay = policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
    return pay + allowance_;
}

Staff* Manager::clone() const { return new Manager(*this); }
int Manager::typeTag() const { return core::TAG_MANAGER; }

void Manager::write(std::ostream& os) const {
    Staff::write(os);
    writeOwn(os);
}

void Manager::writeOwn(std::ostream& os) const {
    allowance_.write(os);
    core::writeValue(os, canApproveLeave_);
}

void Manager::read(std::istream& is) {
    Staff::read(is);
    readOwn(is);
}

void Manager::readOwn(std::istream& is) {
    allowance_.read(is);
    core::readValue(is, canApproveLeave_);
}

} // namespace staff
} // namespace restaurant
