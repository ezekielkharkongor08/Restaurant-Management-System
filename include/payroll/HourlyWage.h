#pragma once
#include "payroll/PayPolicy.h"

namespace restaurant {
namespace payroll {

// HourlyWage: hours worked x hourly rate, with overtime paid at 1.5 times.
class HourlyWage : public PayPolicy {
public:
    HourlyWage();
    explicit HourlyWage(const core::Money& hourlyRate, double hoursPerDay = 8.0);

    const core::Money& hourlyRate() const;
    void setHourlyRate(const core::Money& rate);
    double hoursPerDay() const;
    void setHoursPerDay(double hours);

    core::Money compute(const staff::Attendance& att, const core::Money& baseSalary) const;
    PayPolicy* clone() const;
    int typeTag() const;
    std::string name() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    core::Money hourlyRate_;
    double hoursPerDay_;
};

} // namespace payroll
} // namespace restaurant
