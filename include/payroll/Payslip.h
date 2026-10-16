#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "core/Date.h"
#include "interfaces/Interfaces.h"

namespace restaurant {
namespace payroll {

// Payslip is one person's salary slip for one month.  It is printable and
// serializable so a payment history can be kept on disk.
class Payslip : public interfaces::Printable, public interfaces::Serializable {
public:
    Payslip();
    Payslip(int id, int staffId, const std::string& staffName,
            const std::string& role, const core::Date& month,
            const core::Money& gross, const core::Money& pf,
            const core::Money& tax, const core::Money& bonus,
            const core::Money& net);

    int id() const;
    int staffId() const;
    const std::string& staffName() const;
    const std::string& role() const;
    const core::Date& month() const;
    core::Money gross() const;
    core::Money pf() const;
    core::Money tax() const;
    core::Money bonus() const;
    core::Money net() const;

    Payslip* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

    friend std::ostream& operator<<(std::ostream& os, const Payslip& p);

private:
    int id_;
    int staffId_;
    std::string staffName_;
    std::string role_;
    core::Date month_;
    core::Money gross_;
    core::Money pf_;
    core::Money tax_;
    core::Money bonus_;
    core::Money net_;
};

} // namespace payroll
} // namespace restaurant
