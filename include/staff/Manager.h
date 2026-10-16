#pragma once
#include "staff/Staff.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// Manager runs the restaurant: approves leave, assigns shifts, runs payroll
// and sees reports.  Pay = base salary + fixed allowance + bonus.
// Staff is inherited virtually so the diamond classes only ever contain a
// single copy of it.
class Manager : virtual public Staff {
public:
    Manager();
    Manager(int id, const std::string& name, const std::string& phone,
            const std::string& address, const core::Date& joinDate,
            const core::Money& baseSalary, const core::Money& allowance);
    virtual ~Manager();

    core::Money allowance() const;
    void setAllowance(const core::Money& allowance);

    bool canApproveLeave() const;
    void setCanApproveLeave(bool value);

    std::string roleName() const;
    void showDuties() const;
    core::Money calculatePay() const;
    Staff* clone() const;
    int typeTag() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

protected:
    void writeOwn(std::ostream& os) const;
    void readOwn(std::istream& is);

    core::Money allowance_;
    bool canApproveLeave_;
};

} // namespace staff
} // namespace restaurant
