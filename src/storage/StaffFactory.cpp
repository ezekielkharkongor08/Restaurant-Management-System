#include "storage/StaffFactory.h"
#include "staff/Manager.h"
#include "staff/Chef.h"
#include "staff/Waiter.h"
#include "staff/Cashier.h"
#include "staff/Cleaner.h"
#include "staff/HeadChef.h"
#include "staff/ShiftSupervisor.h"
#include "core/Tags.h"

namespace restaurant {
namespace storage {

staff::Staff* StaffFactory::create(int tag) {
    switch (tag) {
        case core::TAG_MANAGER:          return new staff::Manager();
        case core::TAG_CHEF:             return new staff::Chef();
        case core::TAG_WAITER:           return new staff::Waiter();
        case core::TAG_CASHIER:          return new staff::Cashier();
        case core::TAG_CLEANER:          return new staff::Cleaner();
        case core::TAG_HEAD_CHEF:        return new staff::HeadChef();
        case core::TAG_SHIFT_SUPERVISOR: return new staff::ShiftSupervisor();
        default:                         return 0;
    }
}

int StaffFactory::tagOf(const staff::Staff& s) { return s.typeTag(); }

} // namespace storage
} // namespace restaurant
