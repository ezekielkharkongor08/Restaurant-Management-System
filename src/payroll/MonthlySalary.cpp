#include "payroll/MonthlySalary.h"
#include "core/Tags.h"
#include "core/Serialization.h"

namespace restaurant {
namespace payroll {

MonthlySalary::MonthlySalary() : allowance_(0) {}

MonthlySalary::MonthlySalary(const core::Money& allowance) : allowance_(allowance) {}

const core::Money& MonthlySalary::allowance() const { return allowance_; }
void MonthlySalary::setAllowance(const core::Money& allowance) { allowance_ = allowance; }

core::Money MonthlySalary::compute(const staff::Attendance& att,
                                   const core::Money& baseSalary) const {
    core::Money perDay = core::Money::fromRupees(baseSalary.rupees() / 30.0);
    core::Money deduction = perDay * static_cast<double>(att.absentDays());
    core::Money pay = baseSalary + allowance_ - deduction;
    if (pay < core::Money(0)) return core::Money(0);
    return pay;
}

PayPolicy* MonthlySalary::clone() const { return new MonthlySalary(*this); }
int MonthlySalary::typeTag() const { return core::TAG_MONTHLY_SALARY; }
std::string MonthlySalary::name() const { return "Monthly Salary"; }

void MonthlySalary::write(std::ostream& os) const { allowance_.write(os); }
void MonthlySalary::read(std::istream& is) { allowance_.read(is); }

} // namespace payroll
} // namespace restaurant
