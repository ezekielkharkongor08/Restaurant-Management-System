#pragma once
#include "staff/Chef.h"
#include "staff/Manager.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// HeadChef cooks AND manages the kitchen.  It inherits from Chef and Manager,
// which both inherit from Staff: this is the classic "diamond problem".  It is
// solved with virtual inheritance, so a HeadChef only ever has ONE Staff part.
// HeadChef must override every function that both parents provide so the call
// is not ambiguous.
class HeadChef : public Chef, public Manager {
public:
    HeadChef();
    HeadChef(int id, const std::string& name, const std::string& phone,
             const std::string& address, const core::Date& joinDate,
             const core::Money& baseSalary, const core::Money& allowance,
             const core::Money& ratePerDish);
    virtual ~HeadChef();

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
