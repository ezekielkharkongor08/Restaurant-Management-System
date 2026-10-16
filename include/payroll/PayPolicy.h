#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "staff/Attendance.h"

namespace restaurant {
namespace payroll {

// PayPolicy is an abstract strategy: it describes HOW one person is paid.
// Because Staff holds a PayPolicy*, a person's pay method can be swapped at
// run time (run time polymorphism).
class PayPolicy {
public:
    virtual ~PayPolicy() {}

    // Uses the attendance record and the person's base salary to work out pay.
    virtual core::Money compute(const staff::Attendance& att,
                                const core::Money& baseSalary) const = 0;

    virtual PayPolicy* clone() const = 0;   // deep copy of the policy
    virtual int typeTag() const = 0;
    virtual std::string name() const = 0;

    virtual void write(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;

    // Rebuilds the correct policy from a tag stored in a data file.
    static PayPolicy* create(int tag);
};

} // namespace payroll
} // namespace restaurant
