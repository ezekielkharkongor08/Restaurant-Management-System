#include "payroll.h"
#include <iostream>
#include <iomanip>

namespace restaurant {

const double Payroll::PF_RATE = 0.12;
const double Payroll::TAX_SLAB_1 = 25000.0;
const double Payroll::TAX_SLAB_2 = 50000.0;
const double Payroll::TAX_RATE_LOW = 0.05;
const double Payroll::TAX_RATE_HIGH = 0.10;

Payslip::Payslip() : staffId(0), staffName(""), gross(0.0), pf(0.0), tax(0.0), net(0.0) {}
Payslip::Payslip(int id, const std::string &name, double g, double p, double t, double n)
    : staffId(id), staffName(name), gross(g), pf(p), tax(t), net(n) {}

int Payslip::getStaffId() const { return staffId; }
const std::string& Payslip::getStaffName() const { return staffName; }
double Payslip::getGross() const { return gross; }
double Payslip::getPF() const { return pf; }
double Payslip::getTax() const { return tax; }
double Payslip::getNet() const { return net; }

std::ostream& operator<<(std::ostream &os, const Payslip &p) {
    os << "\n===== PAYSLIP =====\n";
    os << "Staff ID: " << p.staffId << " | Name: " << p.staffName << "\n";
    os << "Gross Pay:  Rs." << p.gross << "\n";
    os << "PF (12%):   -Rs." << p.pf << "\n";
    os << "Tax:        -Rs." << p.tax << "\n";
    os << "-------------------\n";
    os << "NET PAY:    Rs." << p.net << "\n";
    os << "===================\n";
    return os;
}

Payroll::Payroll() {}

Payslip Payroll::computePayslip(const Staff &s) const {
    double gross = s.calculatePay();
    double pf = gross * PF_RATE;
    double tax = 0.0;
    if (gross > TAX_SLAB_2) {
        tax = gross * TAX_RATE_HIGH;
    } else if (gross > TAX_SLAB_1) {
        tax = gross * TAX_RATE_LOW;
    }
    double net = gross - pf - tax;
    return Payslip(s.getId(), s.getName(), gross, pf, tax, net);
}

void Payroll::runPayroll(const std::vector<Staff*> &staffList) {
    history.clear();
    std::cout << "\n========== PAYROLL RUN ==========\n";
    double totalGross = 0.0, totalNet = 0.0;
    for (size_t i = 0; i < staffList.size(); i++) {
        Staff *s = staffList[i];
        if (!s) continue;
        Payslip p = computePayslip(*s);
        history.push_back(p);
        std::cout << p;
        totalGross = totalGross + p.getGross();
        totalNet = totalNet + p.getNet();
    }
    std::cout << "---------------------------------\n";
    std::cout << "Total Gross: Rs." << totalGross << " | Total Net: Rs." << totalNet << "\n";
    std::cout << "=================================\n";
}

void Payroll::showHistory() const {
    std::cout << "\n========== PAYSLIP HISTORY ==========\n";
    if (history.empty()) {
        std::cout << "No payslips generated yet.\n";
    } else {
        for (size_t i = 0; i < history.size(); i++) {
            std::cout << history[i];
        }
    }
    std::cout << "====================================\n";
}

std::ostream& operator<<(std::ostream &os, const Payroll &p) {
    (void)p;
    return os;
}

}
