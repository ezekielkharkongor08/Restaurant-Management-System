#pragma once
#include <iosfwd>
#include <string>

namespace restaurant {
namespace core {

// Date stores a calendar day/month/year, validates itself and supports
// incrementing to the next day and finding the number of days between dates.
class Date {
public:
    Date(int day = 1, int month = 1, int year = 2000);

    static bool isLeapYear(int year);
    static int daysInMonth(int month, int year);

    bool valid() const;
    int day() const;
    int month() const;
    int year() const;

    Date& operator++();      // prefix: move to the next day
    Date operator++(int);    // postfix
    long operator-(const Date& other) const;  // days between two dates

    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

    std::string toString() const;
    long julianDay() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int day_;
    int month_;
    int year_;
};

std::ostream& operator<<(std::ostream& os, const Date& d);
std::istream& operator>>(std::istream& is, Date& d);

} // namespace core
} // namespace restaurant
