#pragma once
#include <iosfwd>
#include "core/Money.h"

namespace restaurant {
namespace interfaces {

// Serializable: any class that can save itself to / load itself from a stream.
class Serializable {
public:
    virtual ~Serializable() {}
    virtual void write(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
};

// Payable: any class that can be paid.
class Payable {
public:
    virtual ~Payable() {}
    virtual core::Money calculatePay() const = 0;
};

// Printable: any class that can print itself on screen.
class Printable {
public:
    virtual ~Printable() {}
    virtual void print(std::ostream& os) const = 0;
};

} // namespace interfaces
} // namespace restaurant
