#ifndef PAYROLL_H
#define PAYROLL_H

#include <ostream>
#include <string>
#include <vector>
#include "staff.h"
#include "templates.h"

class Payslip {
private:
    int staffId;
    char staffName[32];
    double gross;
    double pf;
    double net;

public:
    Payslip();
    Payslip(int id, const std::string &name, double grossPay, double pfAmount, double netPay);

    int getStaffId() const;
    std::string getStaffName() const;
    double getGross() const;
    double getPf() const;
    double getNet() const;

    friend std::ostream& operator<<(std::ostream &output, const Payslip &slip);
};

class Payroll {
private:
    static const double PF_RATE;

    std::vector<Payslip> history;
    FileStore<Payslip> store;

public:
    Payroll();

    Payslip computePayslip(const Staff &staffMember) const;
    void runPayroll(const std::vector<Staff*> &staffList);
    void showHistory() const;
};

#endif
