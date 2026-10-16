#include "payroll/Payroll.h"
#include "payroll/TaxConfig.h"
#include "staff/Staff.h"
#include "core/IdGenerator.h"

namespace restaurant {
namespace payroll {

Payroll::Payroll() : payslips_() {}

Payslip Payroll::runMonthly(staff::Staff& s, const core::Date& month,
                            const core::Money& bonus) {
    (void)s.id();
    core::Money gross = s.calculatePay();          // run-time polymorphism
    core::Money pf = TaxConfig::providentFund(gross);
    core::Money tax = TaxConfig::incomeTax(gross);
    core::Money net = gross + bonus - pf - tax;
    if (net < core::Money(0)) net = core::Money(0);

    Payslip slip(core::IdGenerator::nextPayslipId(), s.id(), s.name(),
                 s.roleName(), month, gross, pf, tax, bonus, net);
    payslips_.push_back(slip);
    return slip;
}

Payslip Payroll::payFor(staff::Staff& s, const core::Date& month,
                        const core::Money& bonus) {
    return runMonthly(s, month, bonus);
}

void Payroll::addPayslip(const Payslip& slip) { payslips_.push_back(slip); }

core::Money Payroll::totalExpense() const {
    core::Money total(0);
    for (std::size_t i = 0; i < payslips_.size(); ++i) total += payslips_[i].net();
    return total;
}

const std::vector<Payslip>& Payroll::payslips() const { return payslips_; }

void Payroll::clear() { payslips_.clear(); }

} // namespace payroll
} // namespace restaurant
