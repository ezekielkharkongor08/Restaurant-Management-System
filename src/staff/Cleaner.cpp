#include "staff/Cleaner.h"
#include "payroll/HourlyWage.h"
#include "core/Serialization.h"
#include <iostream>

namespace restaurant {
namespace staff {

Cleaner::Cleaner()
    : Staff(), hourlyWage_(core::Money::fromRupees(100)), schedule_() {
    setPayPolicy(new payroll::HourlyWage(hourlyWage_));
}

Cleaner::Cleaner(int id, const std::string& name, const std::string& phone,
                 const std::string& address, const core::Date& joinDate,
                 const core::Money& baseSalary, const core::Money& hourlyWage)
    : Staff(id, name, phone, address, joinDate, baseSalary),
      hourlyWage_(hourlyWage), schedule_() {
    setPayPolicy(new payroll::HourlyWage(hourlyWage_));
}

Cleaner::~Cleaner() {}

core::Money Cleaner::hourlyWage() const { return hourlyWage_; }
void Cleaner::setHourlyWage(const core::Money& wage) { hourlyWage_ = wage; }

void Cleaner::addSchedule(const std::string& task) { schedule_.push_back(task); }
const std::vector<std::string>& Cleaner::schedule() const { return schedule_; }

std::string Cleaner::roleName() const { return "Cleaner"; }

void Cleaner::showDuties() const {
    std::cout << "Cleaner duties: clean tables and the kitchen according to the "
                 "cleaning schedule.\n";
}

core::Money Cleaner::calculatePay() const {
    return policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
}

Staff* Cleaner::clone() const { return new Cleaner(*this); }
int Cleaner::typeTag() const { return core::TAG_CLEANER; }

void Cleaner::write(std::ostream& os) const {
    Staff::write(os);
    writeOwn(os);
}

void Cleaner::writeOwn(std::ostream& os) const {
    hourlyWage_.write(os);
    std::uint64_t n = schedule_.size();
    core::writeValue(os, n);
    for (std::size_t i = 0; i < schedule_.size(); ++i)
        core::writeString(os, schedule_[i]);
}

void Cleaner::read(std::istream& is) {
    Staff::read(is);
    readOwn(is);
}

void Cleaner::readOwn(std::istream& is) {
    hourlyWage_.read(is);
    std::uint64_t n = 0;
    core::readValue(is, n);
    schedule_.clear();
    for (std::uint64_t i = 0; i < n; ++i) {
        std::string s;
        core::readString(is, s);
        schedule_.push_back(s);
    }
}

} // namespace staff
} // namespace restaurant
