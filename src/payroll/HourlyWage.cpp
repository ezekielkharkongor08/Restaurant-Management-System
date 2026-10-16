#include "payroll/HourlyWage.h"
#include "core/Tags.h"
#include "core/Serialization.h"

namespace restaurant {
namespace payroll {

HourlyWage::HourlyWage() : hourlyRate_(0), hoursPerDay_(8.0) {}

HourlyWage::HourlyWage(const core::Money& hourlyRate, double hoursPerDay)
    : hourlyRate_(hourlyRate), hoursPerDay_(hoursPerDay) {}

const core::Money& HourlyWage::hourlyRate() const { return hourlyRate_; }
void HourlyWage::setHourlyRate(const core::Money& rate) { hourlyRate_ = rate; }
double HourlyWage::hoursPerDay() const { return hoursPerDay_; }
void HourlyWage::setHoursPerDay(double hours) { hoursPerDay_ = hours; }

core::Money HourlyWage::compute(const staff::Attendance& att,
                                const core::Money& baseSalary) const {
    (void)baseSalary;
    double normalHours = static_cast<double>(att.presentDays()) * hoursPerDay_;
    double overtimeHours = att.overtimeHours();
    core::Money pay = hourlyRate_ * normalHours;
    pay += hourlyRate_ * (overtimeHours * 1.5);
    return pay;
}

PayPolicy* HourlyWage::clone() const { return new HourlyWage(*this); }
int HourlyWage::typeTag() const { return core::TAG_HOURLY_WAGE; }
std::string HourlyWage::name() const { return "Hourly Wage"; }

void HourlyWage::write(std::ostream& os) const {
    hourlyRate_.write(os);
    core::writeValue(os, hoursPerDay_);
}

void HourlyWage::read(std::istream& is) {
    hourlyRate_.read(is);
    core::readValue(is, hoursPerDay_);
}

} // namespace payroll
} // namespace restaurant
