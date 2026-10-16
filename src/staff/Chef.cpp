#include "staff/Chef.h"
#include "payroll/MonthlySalary.h"
#include "core/Serialization.h"
#include <iostream>
#include <algorithm>

namespace restaurant {
namespace staff {

Chef::Chef() : Staff(), ratePerDish_(0), dishesCooked_(0), queue_() {
    setPayPolicy(new payroll::MonthlySalary());
}

Chef::Chef(int id, const std::string& name, const std::string& phone,
           const std::string& address, const core::Date& joinDate,
           const core::Money& baseSalary, const core::Money& ratePerDish)
    : Staff(id, name, phone, address, joinDate, baseSalary),
      ratePerDish_(ratePerDish), dishesCooked_(0), queue_() {
    setPayPolicy(new payroll::MonthlySalary());
}

Chef::~Chef() {}

core::Money Chef::ratePerDish() const { return ratePerDish_; }
void Chef::setRatePerDish(const core::Money& rate) { ratePerDish_ = rate; }

int Chef::dishesCooked() const { return dishesCooked_; }
void Chef::addDishCooked() { ++dishesCooked_; }

void Chef::enqueueOrder(int orderId) { queue_.push_back(orderId); }

int Chef::nextOrder() const { return queue_.empty() ? -1 : queue_.front(); }

void Chef::dequeueOrder(int orderId) {
    queue_.erase(std::remove(queue_.begin(), queue_.end(), orderId), queue_.end());
}

const std::vector<int>& Chef::cookingQueue() const { return queue_; }

std::string Chef::roleName() const { return "Chef"; }

void Chef::showDuties() const {
    std::cout << "Chef duties: cook the queued orders and mark them Cooking "
                 "then Ready.\n";
}

core::Money Chef::calculatePay() const {
    core::Money base = policy_ ? policy_->compute(attendance_, baseSalary_) : baseSalary_;
    return base + ratePerDish_ * static_cast<double>(dishesCooked_);
}

Staff* Chef::clone() const { return new Chef(*this); }
int Chef::typeTag() const { return core::TAG_CHEF; }

void Chef::write(std::ostream& os) const {
    Staff::write(os);
    writeOwn(os);
}

void Chef::writeOwn(std::ostream& os) const {
    ratePerDish_.write(os);
    core::writeValue(os, dishesCooked_);
    std::uint64_t n = queue_.size();
    core::writeValue(os, n);
    for (std::size_t i = 0; i < queue_.size(); ++i) core::writeValue(os, queue_[i]);
}

void Chef::read(std::istream& is) {
    Staff::read(is);
    readOwn(is);
}

void Chef::readOwn(std::istream& is) {
    ratePerDish_.read(is);
    core::readValue(is, dishesCooked_);
    std::uint64_t n = 0;
    core::readValue(is, n);
    queue_.clear();
    for (std::uint64_t i = 0; i < n; ++i) {
        int v = 0;
        core::readValue(is, v);
        queue_.push_back(v);
    }
}

} // namespace staff
} // namespace restaurant
