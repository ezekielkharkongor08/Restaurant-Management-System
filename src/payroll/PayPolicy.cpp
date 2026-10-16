#include "payroll/PayPolicy.h"
#include "payroll/MonthlySalary.h"
#include "payroll/HourlyWage.h"
#include "payroll/CommissionPay.h"
#include "core/Tags.h"

namespace restaurant {
namespace payroll {

PayPolicy* PayPolicy::create(int tag) {
    switch (tag) {
        case core::TAG_MONTHLY_SALARY: return new MonthlySalary();
        case core::TAG_HOURLY_WAGE:    return new HourlyWage();
        case core::TAG_COMMISSION_PAY: return new CommissionPay();
        default:                       return 0;
    }
}

} // namespace payroll
} // namespace restaurant
