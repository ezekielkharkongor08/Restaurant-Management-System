#pragma once
#include "core/Money.h"

namespace restaurant {
namespace payroll {

// TaxConfig holds only static members: PF percentage, GST percentage and the
// income-tax slabs.  Changing a value here changes it for the whole program.
class TaxConfig {
public:
    static double pfPercent();
    static void setPfPercent(double value);

    static double gstPercent();
    static void setGstPercent(double value);

    static double slabLimit1();
    static double slabLimit2();
    static double slabRate1();
    static double slabRate2();
    static void setSlab(double limit1, double limit2, double rate1, double rate2);

    static core::Money providentFund(const core::Money& gross);
    static core::Money incomeTax(const core::Money& gross);
    static core::Money gst(const core::Money& amount);

private:
    static double pfPercent_;
    static double gstPercent_;
    static double slabLimit1_;
    static double slabLimit2_;
    static double slabRate1_;
    static double slabRate2_;
};

} // namespace payroll
} // namespace restaurant
