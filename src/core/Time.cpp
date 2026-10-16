#include "core/Time.h"
#include "core/Exceptions.h"
#include <ostream>
#include <istream>
#include <sstream>

namespace restaurant {
namespace core {

Time::Time(int hour, int minute) : hour_(hour), minute_(minute) {
    if (!valid())
        throw InvalidInputException("Invalid time: " + toString());
}

bool Time::valid() const {
    return hour_ >= 0 && hour_ <= 23 && minute_ >= 0 && minute_ <= 59;
}

int Time::hour() const { return hour_; }
int Time::minute() const { return minute_; }

int Time::minutesSinceMidnight() const { return hour_ * 60 + minute_; }

int Time::operator-(const Time& other) const {
    return minutesSinceMidnight() - other.minutesSinceMidnight();
}

bool Time::operator<(const Time& other) const {
    return minutesSinceMidnight() < other.minutesSinceMidnight();
}

bool Time::operator==(const Time& other) const {
    return minutesSinceMidnight() == other.minutesSinceMidnight();
}

std::string Time::toString() const {
    std::ostringstream os;
    os << (hour_ < 10 ? "0" : "") << hour_ << ":"
       << (minute_ < 10 ? "0" : "") << minute_;
    return os.str();
}

void Time::write(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&hour_), sizeof(hour_));
    os.write(reinterpret_cast<const char*>(&minute_), sizeof(minute_));
}

void Time::read(std::istream& is) {
    is.read(reinterpret_cast<char*>(&hour_), sizeof(hour_));
    is.read(reinterpret_cast<char*>(&minute_), sizeof(minute_));
}

std::ostream& operator<<(std::ostream& os, const Time& t) { return os << t.toString(); }

std::istream& operator>>(std::istream& is, Time& t) {
    int hh = 0, mm = 0;
    char sep = ':';
    is >> hh >> sep >> mm;
    if (sep != ':')
        throw InvalidInputException("Time must be written as hh:mm");
    t = Time(hh, mm);
    return is;
}

} // namespace core
} // namespace restaurant
