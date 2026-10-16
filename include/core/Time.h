#pragma once
#include <iosfwd>
#include <string>

namespace restaurant {
namespace core {

// Time stores hours and minutes of a day, used for shift start/end and to
// compute worked hours / overtime.
class Time {
public:
    Time(int hour = 0, int minute = 0);

    bool valid() const;
    int hour() const;
    int minute() const;
    int minutesSinceMidnight() const;

    int operator-(const Time& other) const;  // minutes between two times
    bool operator<(const Time& other) const;
    bool operator==(const Time& other) const;

    std::string toString() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int hour_;
    int minute_;
};

std::ostream& operator<<(std::ostream& os, const Time& t);
std::istream& operator>>(std::istream& is, Time& t);

} // namespace core
} // namespace restaurant
