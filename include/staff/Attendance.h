#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include "core/Date.h"
#include "core/Money.h"

namespace restaurant {

namespace payroll {
class Payroll;
}

namespace staff {

// Attendance keeps the month-by-month presence record of one staff member.
// Payroll reads it to work out the final pay.
class Attendance {
public:
    Attendance();

    void markPresent(const core::Date& date);
    void markAbsent(const core::Date& date);
    void markLeave(const core::Date& date);
    void addOvertime(double hours);
    void addTips(const core::Money& amount);
    void addSales(const core::Money& amount);

    int presentDays() const;
    int absentDays() const;
    int leaveDays() const;
    double overtimeHours() const;
    core::Money tipsEarned() const;
    core::Money salesAttributed() const;

    const std::vector<core::Date>& presentDates() const;
    const std::vector<core::Date>& leaveDates() const;

    void reset();

    void write(std::ostream& os) const;
    void read(std::istream& is);

    friend std::ostream& operator<<(std::ostream& os, const Attendance& a);
    friend class payroll::Payroll;

private:
    int presentDays_;
    int absentDays_;
    int leaveDays_;
    double overtimeHours_;
    core::Money tipsEarned_;
    core::Money salesAttributed_;
    std::vector<core::Date> presentDates_;
    std::vector<core::Date> leaveDates_;
};

} // namespace staff
} // namespace restaurant
