#pragma once
#include "staff/Staff.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// Waiter takes orders at tables, serves food and receives tips.
// Pay = base salary + tips share (through a CommissionPay policy).
class Waiter : virtual public Staff {
public:
    Waiter();
    Waiter(int id, const std::string& name, const std::string& phone,
           const std::string& address, const core::Date& joinDate,
           const core::Money& baseSalary);
    virtual ~Waiter();

    int ordersTaken() const;
    void addOrderTaken();

    core::Money totalTips() const;
    void addTip(const core::Money& tip);

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

    int ordersTaken_;
};

} // namespace staff
} // namespace restaurant
