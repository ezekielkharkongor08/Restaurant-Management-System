#include "core/Money.h"
#include <ostream>
#include <istream>
#include <sstream>
#include <cmath>

namespace restaurant {
namespace core {

Money::Money(long long paise) : paise_(paise) {}

Money Money::fromRupees(double rupees) {
    return Money(static_cast<long long>(std::llround(rupees * 100.0)));
}

Money Money::fromPaise(long long paise) { return Money(paise); }

long long Money::paise() const { return paise_; }

double Money::rupees() const { return static_cast<double>(paise_) / 100.0; }

Money Money::operator+(const Money& other) const { return Money(paise_ + other.paise_); }
Money Money::operator-(const Money& other) const { return Money(paise_ - other.paise_); }
Money Money::operator*(double factor) const {
    return Money(static_cast<long long>(std::llround(static_cast<double>(paise_) * factor)));
}
Money& Money::operator+=(const Money& other) { paise_ += other.paise_; return *this; }
Money& Money::operator-=(const Money& other) { paise_ -= other.paise_; return *this; }

bool Money::operator<(const Money& other) const { return paise_ < other.paise_; }
bool Money::operator>(const Money& other) const { return paise_ > other.paise_; }
bool Money::operator<=(const Money& other) const { return paise_ <= other.paise_; }
bool Money::operator>=(const Money& other) const { return paise_ >= other.paise_; }
bool Money::operator==(const Money& other) const { return paise_ == other.paise_; }
bool Money::operator!=(const Money& other) const { return paise_ != other.paise_; }

Money Money::abs() const { return Money(paise_ < 0 ? -paise_ : paise_); }

std::string Money::toString() const {
    long long p = paise_;
    std::string sign;
    if (p < 0) {
        sign = "-";
        p = -p;
    }
    long long rupees = p / 100;
    long long paise = p % 100;
    std::ostringstream os;
    os << sign << "Rs " << rupees << "." << (paise < 10 ? "0" : "") << paise;
    return os.str();
}

void Money::write(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&paise_), sizeof(paise_));
}

void Money::read(std::istream& is) {
    is.read(reinterpret_cast<char*>(&paise_), sizeof(paise_));
}

std::ostream& operator<<(std::ostream& os, const Money& m) {
    return os << m.toString();
}

} // namespace core
} // namespace restaurant
