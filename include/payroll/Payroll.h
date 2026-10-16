#pragma once
#include <vector>
#include "core/Money.h"
#include "core/Date.h"
#include "payroll/Payslip.h"

namespace restaurant {

namespace staff {
class Staff;
class Attendance;
}

namespace payroll {

// Payroll performs the monthly payroll run.  It is a FRIEND of Staff and
// Attendance, so it can read their private salary and attendance data.
class Payroll {
public:
    Payroll();

    // Produce the payslip for one staff member for one month.
    Payslip runMonthly(staff::Staff& s, const core::Date& month,
                       const core::Money& bonus = core::Money(0));

    Payslip payFor(staff::Staff& s, const core::Date& month,
                   const core::Money& bonus = core::Money(0));

    void addPayslip(const Payslip& slip);
    core::Money totalExpense() const;   // sum of all produced payslips
    const std::vector<Payslip>& payslips() const;
    void clear();

private:
    std::vector<Payslip> payslips_;
};

} // namespace payroll
} // namespace restaurant
