#pragma once
#include <vector>
#include "staff/Staff.h"
#include "core/Tags.h"

namespace restaurant {
namespace staff {

// Chef cooks the orders.  Keeps a queue of order IDs to cook and can change an
// order status to Cooking or Ready.  Pay = base salary + rate per dish cooked.
class Chef : virtual public Staff {
public:
    Chef();
    Chef(int id, const std::string& name, const std::string& phone,
         const std::string& address, const core::Date& joinDate,
         const core::Money& baseSalary, const core::Money& ratePerDish);
    virtual ~Chef();

    core::Money ratePerDish() const;
    void setRatePerDish(const core::Money& rate);

    int dishesCooked() const;
    void addDishCooked();

    void enqueueOrder(int orderId);
    int nextOrder() const;
    void dequeueOrder(int orderId);
    const std::vector<int>& cookingQueue() const;

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

    core::Money ratePerDish_;
    int dishesCooked_;
    std::vector<int> queue_;
};

} // namespace staff
} // namespace restaurant
