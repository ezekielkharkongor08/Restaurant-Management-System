#pragma once

namespace restaurant {
namespace core {

// Type tags are stored in the data files so that a single file can hold many
// different staff roles.  StaffFactory reads the tag and rebuilds the correct
// derived class.

enum StaffTag {
    TAG_STAFF = 0,
    TAG_MANAGER = 1,
    TAG_CHEF = 2,
    TAG_WAITER = 3,
    TAG_CASHIER = 4,
    TAG_CLEANER = 5,
    TAG_HEAD_CHEF = 6,
    TAG_SHIFT_SUPERVISOR = 7
};

enum PolicyTag {
    TAG_NO_POLICY = 0,
    TAG_MONTHLY_SALARY = 1,
    TAG_HOURLY_WAGE = 2,
    TAG_COMMISSION_PAY = 3
};

} // namespace core
} // namespace restaurant
