#include "staff/HeadChef.h"
#include "payroll/MonthlySalary.h"
#include <iostream>

namespace restaurant {
namespace staff {

HeadChef::HeadChef() : Staff(), Chef(), Manager() {
    setPayPolicy(new payroll::MonthlySalary());
}

HeadChef::HeadChef(int id, const std::string& name, const std::string& phone,
                   const std::string& address, const core::Date& joinDate,
                   const core::Money& baseSalary, const core::Money& allowance,
                   const core::Money& ratePerDish)
    : Staff(id, name, phone, address, joinDate, baseSalary), Chef(), Manager() {
    allowance_ = allowance;
    ratePerDish_ = ratePerDish;
    setPayPolicy(new payroll::MonthlySalary());
}

HeadChef::~HeadChef() {}

std::string HeadChef::roleName() const { return "Head Chef"; }

void HeadChef::showDuties() const {
    std::cout << "Head Chef duties: cook orders AND manage the kitchen, assign "
                 "chefs and approve kitchen leave.\n";
}

core::Money HeadChef::calculatePay() const {
    core::Money base = policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
    return base + allowance_ + ratePerDish_ * static_cast<double>(dishesCooked_);
}

Staff* HeadChef::clone() const { return new HeadChef(*this); }
int HeadChef::typeTag() const { return core::TAG_HEAD_CHEF; }

void HeadChef::write(std::ostream& os) const {
    Staff::write(os);      // the virtual base is written only once
    Chef::writeOwn(os);    // Chef-specific fields
    Manager::writeOwn(os); // Manager-specific fields
}

void HeadChef::read(std::istream& is) {
    Staff::read(is);
    Chef::readOwn(is);
    Manager::readOwn(is);
}

} // namespace staff
} // namespace restaurant
