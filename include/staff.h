#ifndef RMS_STAFF_H
#define RMS_STAFF_H

#include <string>
#include <vector>
#include <ostream>
#include "records.h"

namespace restaurant {

// Abstract base class for all staff (pure virtual functions -> run-time polymorphism)
class Staff {
protected:
    int id;
    std::string name;
    double baseSalary;
    int daysPresent;
    double overtimeHours;
    static int nextId;
public:
    Staff();
    Staff(const std::string &n, double base);
    Staff(const Staff &other);                 // copy constructor
    Staff& operator=(const Staff &other);      // assignment operator
    virtual ~Staff();                          // virtual destructor

    static int nextIdValue() { return nextId; }
    static void setNextIdValue(int v) { nextId = v; }

    int getId() const;
    const std::string& getName() const;
    double getBaseSalary() const;
    int getDaysPresent() const;
    double getOvertimeHours() const;
    bool isActive() const;

    void setName(const std::string &n);
    void setBaseSalary(double b);
    void setDaysPresent(int d);
    void setOvertimeHours(double o);
    void setActive(bool a);

    // Pure virtual functions - each role implements its own pay rule and duties
    virtual double calculatePay() const = 0;
    virtual std::string duties() const = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual int role() const = 0;
    virtual std::string roleName() const = 0;

    // Conversion to/from fixed-size binary record
    virtual StaffRecord toRecord() const = 0;
    virtual void fromRecord(const StaffRecord &r) = 0;

    friend class Payroll;                      // friend class: Payroll can access staff internals
};

class Chef : virtual public Staff {
protected:
    int dishesCooked;
public:
    Chef();
    Chef(const std::string &n, double base);
    virtual ~Chef();

    int getDishesCooked() const;
    void setDishesCooked(int d);

    virtual double calculatePay() const;
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

class Waiter : virtual public Staff {
protected:
    double tips;
public:
    Waiter();
    Waiter(const std::string &n, double base);
    virtual ~Waiter();

    double getTips() const;
    void addTips(double t);

    virtual double calculatePay() const;
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

class Cashier : virtual public Staff {
public:
    Cashier();
    Cashier(const std::string &n, double base);
    virtual ~Cashier();

    virtual double calculatePay() const;
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

class Cleaner : virtual public Staff {
public:
    Cleaner();
    Cleaner(const std::string &n, double base);
    virtual ~Cleaner();

    virtual double calculatePay() const;
    virtual std::string duties() const;
    virtual void print(std::ostream &os) const;
    virtual int role() const;
    virtual std::string roleName() const;
    virtual StaffRecord toRecord() const;
    virtual void fromRecord(const StaffRecord &r);
};

}

#endif
