#include "payroll/TaxConfig.h"

namespace restaurant {
namespace payroll {

double TaxConfig::pfPercent_ = 12.0;   // 12% provident fund
double TaxConfig::gstPercent_ = 5.0;   // 5% GST on bills
double TaxConfig::slabLimit1_ = 25000.0;
double TaxConfig::slabLimit2_ = 50000.0;
double TaxConfig::slabRate1_ = 10.0;   // 10% between the two limits
double TaxConfig::slabRate2_ = 20.0;   // 20% above the second limit

double TaxConfig::pfPercent() { return pfPercent_; }
void TaxConfig::setPfPercent(double value) { pfPercent_ = value; }

double TaxConfig::gstPercent() { return gstPercent_; }
void TaxConfig::setGstPercent(double value) { gstPercent_ = value; }

double TaxConfig::slabLimit1() { return slabLimit1_; }
double TaxConfig::slabLimit2() { return slabLimit2_; }
double TaxConfig::slabRate1() { return slabRate1_; }
double TaxConfig::slabRate2() { return slabRate2_; }

void TaxConfig::setSlab(double limit1, double limit2, double rate1, double rate2) {
    slabLimit1_ = limit1;
    slabLimit2_ = limit2;
    slabRate1_ = rate1;
    slabRate2_ = rate2;
}

core::Money TaxConfig::providentFund(const core::Money& gross) {
    return core::Money::fromRupees(gross.rupees() * pfPercent_ / 100.0);
}

core::Money TaxConfig::incomeTax(const core::Money& gross) {
    double g = gross.rupees();
    double tax = 0.0;
    if (g > slabLimit2_) {
        tax = (slabLimit2_ - slabLimit1_) * slabRate1_ / 100.0;
        tax += (g - slabLimit2_) * slabRate2_ / 100.0;
    } else if (g > slabLimit1_) {
        tax = (g - slabLimit1_) * slabRate1_ / 100.0;
    }
    return core::Money::fromRupees(tax);
}

core::Money TaxConfig::gst(const core::Money& amount) {
    return core::Money::fromRupees(amount.rupees() * gstPercent_ / 100.0);
}

} // namespace payroll
} // namespace restaurant
