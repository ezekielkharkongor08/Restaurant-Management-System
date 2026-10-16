#pragma once
#include "payroll/PayPolicy.h"

namespace restaurant {
namespace payroll {

// MonthlySalary: base salary + fixed allowance, reduced for unpaid leave days.
class MonthlySalary : public PayPolicy {
public:
    MonthlySalary();
    explicit MonthlySalary(const core::Money& allowance);

    const core::Money& allowance() const;
    void setAllowance(const core::Money& allowance);

    core::Money compute(const staff::Attendance& att, const core::Money& baseSalary) const;
    PayPolicy* clone() const;
    int typeTag() const;
    std::string name() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    core::Money allowance_;
};

} // namespace payroll
} // namespace restaurant
