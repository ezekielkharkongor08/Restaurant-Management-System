#include "core/Date.h"
#include "core/Exceptions.h"
#include <ostream>
#include <istream>
#include <sstream>

namespace restaurant {
namespace core {

Date::Date(int day, int month, int year) : day_(day), month_(month), year_(year) {
    if (!valid())
        throw InvalidInputException("Invalid date: " + toString());
}

bool Date::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int month, int year) {
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

bool Date::valid() const {
    if (year_ < 1900 || year_ > 2200) return false;
    if (month_ < 1 || month_ > 12) return false;
    if (day_ < 1 || day_ > daysInMonth(month_, year_)) return false;
    return true;
}

int Date::day() const { return day_; }
int Date::month() const { return month_; }
int Date::year() const { return year_; }

Date& Date::operator++() {
    int dim = daysInMonth(month_, year_);
    ++day_;
    if (day_ > dim) {
        day_ = 1;
        ++month_;
        if (month_ > 12) {
            month_ = 1;
            ++year_;
        }
    }
    return *this;
}

Date Date::operator++(int) {
    Date copy(*this);
    ++(*this);
    return copy;
}

long Date::julianDay() const {
    int a = (14 - month_) / 12;
    long y = year_ + 4800 - a;
    int m = month_ + 12 * a - 3;
    return day_ + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

long Date::operator-(const Date& other) const {
    return julianDay() - other.julianDay();
}

bool Date::operator<(const Date& other) const { return julianDay() < other.julianDay(); }
bool Date::operator>(const Date& other) const { return julianDay() > other.julianDay(); }
bool Date::operator==(const Date& other) const { return julianDay() == other.julianDay(); }
bool Date::operator!=(const Date& other) const { return julianDay() != other.julianDay(); }

std::string Date::toString() const {
    std::ostringstream os;
    os << (day_ < 10 ? "0" : "") << day_ << "/"
       << (month_ < 10 ? "0" : "") << month_ << "/" << year_;
    return os.str();
}

void Date::write(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&day_), sizeof(day_));
    os.write(reinterpret_cast<const char*>(&month_), sizeof(month_));
    os.write(reinterpret_cast<const char*>(&year_), sizeof(year_));
}

void Date::read(std::istream& is) {
    is.read(reinterpret_cast<char*>(&day_), sizeof(day_));
    is.read(reinterpret_cast<char*>(&month_), sizeof(month_));
    is.read(reinterpret_cast<char*>(&year_), sizeof(year_));
}

std::ostream& operator<<(std::ostream& os, const Date& d) { return os << d.toString(); }

std::istream& operator>>(std::istream& is, Date& d) {
    int dd = 1, mm = 1, yy = 2000;
    char sep = '/';
    is >> dd >> sep >> mm >> sep >> yy;
    if (sep != '/')
        throw InvalidInputException("Date must be written as dd/mm/yyyy");
    d = Date(dd, mm, yy);
    return is;
}

} // namespace core
} // namespace restaurant
