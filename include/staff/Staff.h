#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "core/Money.h"
#include "core/Date.h"
#include "interfaces/Interfaces.h"
#include "staff/Person.h"
#include "staff/Attendance.h"

namespace restaurant {

namespace payroll {
class PayPolicy;
class Payroll;
}

namespace storage {
class StaffFactory;
}

namespace staff {

// Staff is the abstract parent of every worker.  It cannot be instantiated
// directly: calculatePay(), showDuties(), clone() and typeTag() are pure
// virtual.  It inherits Person (identity), Payable (pay), Serializable (file)
// and Printable (screen).
class Staff : public Person,
              public interfaces::Payable,
              public interfaces::Serializable,
              public interfaces::Printable {
public:
    Staff();
    Staff(int id, const std::string& name, const std::string& phone,
          const std::string& address, const core::Date& joinDate,
          const core::Money& baseSalary);
    Staff(const Staff& other);              // deep copy of the pay policy
    Staff& operator=(const Staff& other);   // deep copy assignment
    virtual ~Staff();                       // virtual so the child destructor runs

    int id() const;
    void setId(int id);

    const core::Date& joinDate() const;
    void setJoinDate(const core::Date& date);

    core::Money baseSalary() const;
    void setBaseSalary(const core::Money& salary);

    const std::string& password() const;
    void setPassword(const std::string& password);

    Attendance& attendance();
    const Attendance& attendance() const;

    payroll::PayPolicy* payPolicy() const;
    void setPayPolicy(payroll::PayPolicy* policy);
    void changePayPolicy(payroll::PayPolicy* policy);  // runtime pay-method swap

    // Role specific information that every worker must be able to provide.
    virtual std::string roleName() const = 0;
    virtual void showDuties() const = 0;
    virtual Staff* clone() const = 0;
    virtual int typeTag() const = 0;

    // interfaces::Payable
    virtual core::Money calculatePay() const = 0;
    // interfaces::Serializable
    virtual void write(std::ostream& os) const;
    virtual void read(std::istream& is);
    // interfaces::Printable
    virtual void print(std::ostream& os) const;

    static int totalStaff();

protected:
    // Serialization of the Staff part only (Person fields first, then Staff).
    void writeOwn(std::ostream& os) const;
    void readOwn(std::istream& is);

    int id_;
    core::Date joinDate_;
    core::Money baseSalary_;
    std::string password_;
    Attendance attendance_;
    payroll::PayPolicy* policy_;

private:
    static int totalStaff_;

    friend class payroll::Payroll;      // payroll reads private salary data
    friend class storage::StaffFactory; // factory rebuilds objects from tags
};

std::ostream& operator<<(std::ostream& os, const Staff& s);

} // namespace staff
} // namespace restaurant
