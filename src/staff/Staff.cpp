#include "staff/Staff.h"
#include "payroll/PayPolicy.h"
#include "core/Serialization.h"
#include "core/Exceptions.h"

namespace restaurant {
namespace staff {

int Staff::totalStaff_ = 0;

Staff::Staff()
    : Person(), id_(0), joinDate_(1, 1, 2000), baseSalary_(0), password_(""),
      attendance_(), policy_(0) {
    ++totalStaff_;
}

Staff::Staff(int id, const std::string& name, const std::string& phone,
             const std::string& address, const core::Date& joinDate,
             const core::Money& baseSalary)
    : Person(name, phone, address), id_(id), joinDate_(joinDate),
      baseSalary_(baseSalary), password_(""), attendance_(), policy_(0) {
    if (baseSalary < core::Money(0))
        throw core::InvalidInputException("Base salary cannot be negative");
    ++totalStaff_;
}

Staff::Staff(const Staff& other)
    : Person(other), id_(other.id_), joinDate_(other.joinDate_),
      baseSalary_(other.baseSalary_), password_(other.password_),
      attendance_(other.attendance_),
      policy_(other.policy_ ? other.policy_->clone() : 0) {
    ++totalStaff_;
}

Staff& Staff::operator=(const Staff& other) {
    if (this != &other) {
        Person::operator=(other);
        id_ = other.id_;
        joinDate_ = other.joinDate_;
        baseSalary_ = other.baseSalary_;
        password_ = other.password_;
        attendance_ = other.attendance_;
        delete policy_;
        policy_ = other.policy_ ? other.policy_->clone() : 0;
    }
    return *this;
}

Staff::~Staff() {
    delete policy_;
    policy_ = 0;
    --totalStaff_;
}

int Staff::id() const { return id_; }
void Staff::setId(int id) { id_ = id; }

const core::Date& Staff::joinDate() const { return joinDate_; }
void Staff::setJoinDate(const core::Date& date) { joinDate_ = date; }

core::Money Staff::baseSalary() const { return baseSalary_; }
void Staff::setBaseSalary(const core::Money& salary) {
    if (salary < core::Money(0))
        throw core::InvalidInputException("Base salary cannot be negative");
    baseSalary_ = salary;
}

const std::string& Staff::password() const { return password_; }
void Staff::setPassword(const std::string& password) { password_ = password; }

Attendance& Staff::attendance() { return attendance_; }
const Attendance& Staff::attendance() const { return attendance_; }

payroll::PayPolicy* Staff::payPolicy() const { return policy_; }

void Staff::setPayPolicy(payroll::PayPolicy* policy) {
    if (policy_ != policy) {
        delete policy_;
        policy_ = policy;
    }
}

void Staff::changePayPolicy(payroll::PayPolicy* policy) {
    setPayPolicy(policy);
}

int Staff::totalStaff() { return totalStaff_; }

void Staff::write(std::ostream& os) const {
    Person::writeOwn(os);
    writeOwn(os);
}

void Staff::writeOwn(std::ostream& os) const {
    core::writeValue(os, id_);
    joinDate_.write(os);
    baseSalary_.write(os);
    core::writeString(os, password_);
    attendance_.write(os);
    int policyTag = policy_ ? policy_->typeTag() : 0;
    core::writeValue(os, policyTag);
    if (policy_) policy_->write(os);
}

void Staff::read(std::istream& is) {
    Person::readOwn(is);
    readOwn(is);
}

void Staff::readOwn(std::istream& is) {
    core::readValue(is, id_);
    joinDate_.read(is);
    baseSalary_.read(is);
    core::readString(is, password_);
    attendance_.read(is);
    int policyTag = 0;
    core::readValue(is, policyTag);
    delete policy_;
    policy_ = payroll::PayPolicy::create(policyTag);
    if (policy_) policy_->read(is);
}

void Staff::print(std::ostream& os) const {
    os << "#" << id_ << "  " << name_ << "  [" << roleName() << "]"
       << "  joined " << joinDate_ << "  base " << baseSalary_;
}

std::ostream& operator<<(std::ostream& os, const Staff& s) {
    s.print(os);
    return os;
}

} // namespace staff
} // namespace restaurant
