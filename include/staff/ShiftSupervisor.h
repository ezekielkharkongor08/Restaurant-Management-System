#pragma once
#include "staff/Waiter.h"
#include "staff/Cashier.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// ShiftSupervisor takes orders AND handles billing during a shift.  It is the
// second diamond example: Waiter and Cashier both inherit Staff virtually, so a
// ShiftSupervisor has one Staff part.  All functions that both parents share
// are overridden here.
class ShiftSupervisor : public Waiter, public Cashier {
public:
    ShiftSupervisor();
    ShiftSupervisor(int id, const std::string& name, const std::string& phone,
                    const std::string& address, const core::Date& joinDate,
                    const core::Money& baseSalary, const core::Money& overtimeRate);
    virtual ~ShiftSupervisor();

    std::string roleName() const;
    void showDuties() const;
    core::Money calculatePay() const;
    Staff* clone() const;
    int typeTag() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);
};

} // namespace staff
} // namespace restaurant
