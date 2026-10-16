#pragma once
#include <iosfwd>
#include <string>

namespace restaurant {
namespace core {

// Money stores an amount in paise (1 rupee = 100 paise) as an integer so that
// money arithmetic never suffers from floating point rounding errors.
class Money {
public:
    Money(long long paise = 0);

    static Money fromRupees(double rupees);
    static Money fromPaise(long long paise);

    long long paise() const;
    double rupees() const;

    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(double factor) const;
    Money& operator+=(const Money& other);
    Money& operator-=(const Money& other);

    bool operator<(const Money& other) const;
    bool operator>(const Money& other) const;
    bool operator<=(const Money& other) const;
    bool operator>=(const Money& other) const;
    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;

    Money abs() const;
    std::string toString() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    long long paise_;
};

std::ostream& operator<<(std::ostream& os, const Money& m);

} // namespace core
} // namespace restaurant
