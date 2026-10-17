#ifndef RMS_MANAGER_H
#define RMS_MANAGER_H

#include <string>
#include <ostream>
#include "staff.h"

namespace restaurant {

class Manager : virtual public Staff {
protected:
    static const double ALLOWANCE;             // static member: fixed allowance
public:
    Manager();
    Manager(const std::string &n, double base);
    virtual ~Manager();

    static double getAllowance();

    virtual double calculatePay() const;
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

// Diamond problem class: HeadChef is both Chef and Manager (multiple + virtual inheritance)
class HeadChef : public Chef, public Manager {
public:
    HeadChef();
    HeadChef(const std::string &n, double base);
    virtual ~HeadChef();

    virtual double calculatePay() const;       // pay combines both parts
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

}

#endif
