#pragma once
#include <vector>
#include <string>
#include "staff/Staff.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// Cleaner cleans tables and the kitchen.  Keeps a cleaning schedule.
// Pay = hours worked x hourly wage (through an HourlyWage policy).
class Cleaner : virtual public Staff {
public:
    Cleaner();
    Cleaner(int id, const std::string& name, const std::string& phone,
            const std::string& address, const core::Date& joinDate,
            const core::Money& baseSalary, const core::Money& hourlyWage);
    virtual ~Cleaner();

    core::Money hourlyWage() const;
    void setHourlyWage(const core::Money& wage);

    void addSchedule(const std::string& task);
    const std::vector<std::string>& schedule() const;

    std::string roleName() const;
    void showDuties() const;
    core::Money calculatePay() const;
    Staff* clone() const;
    int typeTag() const;

    void write(std::ostream& os) const;
    void read(std::istream& is);

protected:
    void writeOwn(std::ostream& os) const;
    void readOwn(std::istream& is);

    core::Money hourlyWage_;
    std::vector<std::string> schedule_;
};

} // namespace staff
} // namespace restaurant
