#pragma once
#include "staff/Staff.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// Cashier creates bills, takes payment and closes orders.
// Pay = base salary + overtime pay.
class Cashier : virtual public Staff {
public:
    Cashier();
    Cashier(int id, const std::string& name, const std::string& phone,
            const std::string& address, const core::Date& joinDate,
            const core::Money& baseSalary, const core::Money& overtimeRate);
    virtual ~Cashier();

    core::Money overtimeRate() const;
    void setOvertimeRate(const core::Money& rate);

    int billsIssued() const;
    void addBillIssued();

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

    core::Money overtimeRate_;
    int billsIssued_;
};

} // namespace staff
} // namespace restaurant
