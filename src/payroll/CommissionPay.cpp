#include "payroll/CommissionPay.h"
#include "core/Tags.h"
#include "core/Serialization.h"

namespace restaurant {
namespace payroll {

CommissionPay::CommissionPay() : commissionRate_(1.0) {}

CommissionPay::CommissionPay(double commissionRate) : commissionRate_(commissionRate) {}

double CommissionPay::commissionRate() const { return commissionRate_; }
void CommissionPay::setCommissionRate(double rate) { commissionRate_ = rate; }

core::Money CommissionPay::compute(const staff::Attendance& att,
                                   const core::Money& baseSalary) const {
    core::Money earned = att.tipsEarned() + att.salesAttributed();
    return baseSalary + earned * commissionRate_;
}

PayPolicy* CommissionPay::clone() const { return new CommissionPay(*this); }
int CommissionPay::typeTag() const { return core::TAG_COMMISSION_PAY; }
std::string CommissionPay::name() const { return "Commission Pay"; }

void CommissionPay::write(std::ostream& os) const {
    core::writeValue(os, commissionRate_);
}

void CommissionPay::read(std::istream& is) {
    core::readValue(is, commissionRate_);
}

} // namespace payroll
} // namespace restaurant
