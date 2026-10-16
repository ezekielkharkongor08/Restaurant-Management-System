#include "payroll/Payslip.h"
#include "core/Serialization.h"
#include <iomanip>

namespace restaurant {
namespace payroll {

Payslip::Payslip()
    : id_(0), staffId_(0), staffName_(""), role_(""), month_(1, 1, 2000),
      gross_(0), pf_(0), tax_(0), bonus_(0), net_(0) {}

Payslip::Payslip(int id, int staffId, const std::string& staffName,
                 const std::string& role, const core::Date& month,
                 const core::Money& gross, const core::Money& pf,
                 const core::Money& tax, const core::Money& bonus,
                 const core::Money& net)
    : id_(id), staffId_(staffId), staffName_(staffName), role_(role), month_(month),
      gross_(gross), pf_(pf), tax_(tax), bonus_(bonus), net_(net) {}

int Payslip::id() const { return id_; }
int Payslip::staffId() const { return staffId_; }
const std::string& Payslip::staffName() const { return staffName_; }
const std::string& Payslip::role() const { return role_; }
const core::Date& Payslip::month() const { return month_; }
core::Money Payslip::gross() const { return gross_; }
core::Money Payslip::pf() const { return pf_; }
core::Money Payslip::tax() const { return tax_; }
core::Money Payslip::bonus() const { return bonus_; }
core::Money Payslip::net() const { return net_; }

Payslip* Payslip::clone() const { return new Payslip(*this); }

void Payslip::print(std::ostream& os) const {
    os << "---------------------------------------------\n";
    os << "PAYSLIP #" << id_ << "   month: " << month_ << "\n";
    os << "Staff : #" << staffId_ << "  " << staffName_ << "  [" << role_ << "]\n";
    os << "---------------------------------------------\n";
    os << std::left << std::setw(16) << "Gross pay"   << gross_ << "\n";
    os << std::left << std::setw(16) << "Bonus"       << "+ " << bonus_ << "\n";
    os << std::left << std::setw(16) << "Provident F." << "- " << pf_ << "\n";
    os << std::left << std::setw(16) << "Income tax"  << "- " << tax_ << "\n";
    os << std::left << std::setw(16) << "NET PAY"      << net_ << "\n";
    os << "---------------------------------------------\n";
}

void Payslip::write(std::ostream& os) const {
    core::writeValue(os, id_);
    core::writeValue(os, staffId_);
    core::writeString(os, staffName_);
    core::writeString(os, role_);
    month_.write(os);
    gross_.write(os);
    pf_.write(os);
    tax_.write(os);
    bonus_.write(os);
    net_.write(os);
}

void Payslip::read(std::istream& is) {
    core::readValue(is, id_);
    core::readValue(is, staffId_);
    core::readString(is, staffName_);
    core::readString(is, role_);
    month_.read(is);
    gross_.read(is);
    pf_.read(is);
    tax_.read(is);
    bonus_.read(is);
    net_.read(is);
}

std::ostream& operator<<(std::ostream& os, const Payslip& p) {
    p.print(os);
    return os;
}

} // namespace payroll
} // namespace restaurant
