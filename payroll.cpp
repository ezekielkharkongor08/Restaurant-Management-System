#include "payroll.h"

#include <cstring>
#include <iostream>

using namespace std;

const double Payroll::PF_RATE = 0.12;

Payslip::Payslip() {
    staffId = 0;
    memset(staffName, 0, sizeof(staffName));
    gross = 0.0;
    pf = 0.0;
    net = 0.0;
}

Payslip::Payslip(int id, const std::string &name, double grossPay,
                 double pfAmount, double netPay) {
    staffId = id;
    memset(staffName, 0, sizeof(staffName));
    strncpy(staffName, name.c_str(), sizeof(staffName) - 1);
    gross = grossPay;
    pf = pfAmount;
    net = netPay;
}

int Payslip::getStaffId() const {
    return staffId;
}

std::string Payslip::getStaffName() const {
    return staffName;
}

double Payslip::getGross() const {
    return gross;
}

double Payslip::getPf() const {
    return pf;
}

double Payslip::getNet() const {
    return net;
}

std::ostream& operator<<(std::ostream &output, const Payslip &slip) {
    output << "\n===== PAYSLIP =====\n";
    output << "Staff ID: " << slip.staffId
           << " | Name: " << slip.staffName << "\n";
    output << "Gross Pay:  Rs." << slip.gross << "\n";
    output << "PF (12%):   -Rs." << slip.pf << "\n";
    output << "-------------------\n";
    output << "NET PAY:    Rs." << slip.net << "\n";
    output << "===================\n";

    return output;
}

Payroll::Payroll() : store("data/payslips.dat") {
    history = store.readAll();
}

Payslip Payroll::computePayslip(const Staff &staffMember) const {
    double grossPay = staffMember.calculateSalary();
    double pfAmount = grossPay * PF_RATE;
    double netPay = grossPay - pfAmount;

    return Payslip(staffMember.getId(), staffMember.getName(),
                   grossPay, pfAmount, netPay);
}

void Payroll::runPayroll(const std::vector<Staff*> &staffList) {
    cout << "\n========== PAYROLL RUN ==========\n";

    double totalGross = 0.0;
    double totalNet = 0.0;

    for (int i = 0; i < (int)staffList.size(); i++) {
        Staff *person = staffList[i];
        if (person == NULL) {
            continue;
        }

        Payslip slip = computePayslip(*person);
        history.push_back(slip);
        store.append(slip);

        cout << slip;
        totalGross = totalGross + slip.getGross();
        totalNet = totalNet + slip.getNet();
    }

    cout << "---------------------------------\n";
    cout << "Total Gross: Rs." << totalGross
         << " | Total Net: Rs." << totalNet << "\n";
    cout << "=================================\n";
}

void Payroll::showHistory() const {
    cout << "\n========== PAYSLIP HISTORY ==========\n";

    if (history.empty()) {
        cout << "No payslips generated yet.\n";
    } else {
        for (int i = 0; i < (int)history.size(); i++) {
            cout << history[i];
        }
    }

    cout << "=====================================\n";
}
