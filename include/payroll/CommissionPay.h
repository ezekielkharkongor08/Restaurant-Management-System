#pragma once
#include "payroll/PayPolicy.h"

namespace restaurant {
namespace payroll {

// CommissionPay: base salary + a percentage of the sales or tips the person
// brought in (used for waiters and shift supervisors).
class CommissionPay : public PayPolicy {
public:
    CommissionPay();
    explicit CommissionPay(double commissionRate);

    double commissionRate() const;
    void setCommissionRate(double rate);

    core::Money compute(const staff::Attendance& att, const core::Money& baseSalary) const;
    PayPolicy* clone() const;
    int typeTag() const;
    std::string name() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    double commissionRate_;  // 1.0 means 100% of tips
};

} // namespace payroll
} // namespace restaurant
