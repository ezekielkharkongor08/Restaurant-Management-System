#ifndef RMS_PAYROLL_H
#define RMS_PAYROLL_H

#include <string>
#include <vector>
#include <ostream>
#include "staff.h"

namespace restaurant {

class Payslip {
private:
    int staffId;
    std::string staffName;
    double gross;
    double pf;
    double tax;
    double net;
public:
    Payslip();
    Payslip(int id, const std::string &name, double g, double p, double t, double n);

    int getStaffId() const;
    const std::string& getStaffName() const;
    double getGross() const;
    double getPF() const;
    double getTax() const;
    double getNet() const;

    friend std::ostream& operator<<(std::ostream &os, const Payslip &p);
};

class Payroll {
private:
    static const double PF_RATE;               // static member: PF percentage
    static const double TAX_SLAB_1;
    static const double TAX_SLAB_2;
    static const double TAX_RATE_LOW;
    static const double TAX_RATE_HIGH;
    std::vector<Payslip> history;              // payslips kept in history
public:
    Payroll();

    Payslip computePayslip(const Staff &s) const;
    void runPayroll(const std::vector<Staff*> &staffList);
    void showHistory() const;

    friend std::ostream& operator<<(std::ostream &os, const Payroll &p);
};

}

#endif
