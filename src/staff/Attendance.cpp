#include "staff/Attendance.h"
#include "core/Serialization.h"

namespace restaurant {
namespace staff {

Attendance::Attendance()
    : presentDays_(0), absentDays_(0), leaveDays_(0),
      overtimeHours_(0.0), tipsEarned_(0), salesAttributed_(0) {}

void Attendance::markPresent(const core::Date& date) {
    presentDates_.push_back(date);
    ++presentDays_;
}

void Attendance::markAbsent(const core::Date& date) {
    (void)date;
    ++absentDays_;
}

void Attendance::markLeave(const core::Date& date) {
    leaveDates_.push_back(date);
    ++leaveDays_;
}

void Attendance::addOvertime(double hours) {
    if (hours > 0) overtimeHours_ += hours;
}

void Attendance::addTips(const core::Money& amount) { tipsEarned_ += amount; }
void Attendance::addSales(const core::Money& amount) { salesAttributed_ += amount; }

int Attendance::presentDays() const { return presentDays_; }
int Attendance::absentDays() const { return absentDays_; }
int Attendance::leaveDays() const { return leaveDays_; }
double Attendance::overtimeHours() const { return overtimeHours_; }
core::Money Attendance::tipsEarned() const { return tipsEarned_; }
core::Money Attendance::salesAttributed() const { return salesAttributed_; }

const std::vector<core::Date>& Attendance::presentDates() const { return presentDates_; }
const std::vector<core::Date>& Attendance::leaveDates() const { return leaveDates_; }

void Attendance::reset() {
    presentDays_ = absentDays_ = leaveDays_ = 0;
    overtimeHours_ = 0.0;
    tipsEarned_ = core::Money(0);
    salesAttributed_ = core::Money(0);
    presentDates_.clear();
    leaveDates_.clear();
}

void Attendance::write(std::ostream& os) const {
    core::writeValue(os, presentDays_);
    core::writeValue(os, absentDays_);
    core::writeValue(os, leaveDays_);
    core::writeValue(os, overtimeHours_);
    tipsEarned_.write(os);
    salesAttributed_.write(os);

    std::uint64_t np = presentDates_.size();
    core::writeValue(os, np);
    for (std::size_t i = 0; i < presentDates_.size(); ++i) presentDates_[i].write(os);

    std::uint64_t nl = leaveDates_.size();
    core::writeValue(os, nl);
    for (std::size_t i = 0; i < leaveDates_.size(); ++i) leaveDates_[i].write(os);
}

void Attendance::read(std::istream& is) {
    core::readValue(is, presentDays_);
    core::readValue(is, absentDays_);
    core::readValue(is, leaveDays_);
    core::readValue(is, overtimeHours_);
    tipsEarned_.read(is);
    salesAttributed_.read(is);

    std::uint64_t np = 0;
    core::readValue(is, np);
    presentDates_.clear();
    for (std::uint64_t i = 0; i < np; ++i) {
        core::Date d(1, 1, 2000);
        d.read(is);
        presentDates_.push_back(d);
    }

    std::uint64_t nl = 0;
    core::readValue(is, nl);
    leaveDates_.clear();
    for (std::uint64_t i = 0; i < nl; ++i) {
        core::Date d(1, 1, 2000);
        d.read(is);
        leaveDates_.push_back(d);
    }
}

std::ostream& operator<<(std::ostream& os, const Attendance& a) {
    os << "present=" << a.presentDays_ << " absent=" << a.absentDays_
       << " leave=" << a.leaveDays_ << " overtime=" << a.overtimeHours_ << "h"
       << " tips=" << a.tipsEarned_;
    return os;
}

} // namespace staff
} // namespace restaurant
