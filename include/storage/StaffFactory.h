#pragma once
#include "staff/Staff.h"

namespace restaurant {
namespace storage {

// When loading, the program only sees a numeric type tag (for example "CHEF").
// StaffFactory reads the tag and creates the correct derived object, so one
// file can hold many different staff types.
class StaffFactory {
public:
    static staff::Staff* create(int tag);
    static int tagOf(const staff::Staff& s);
};

} // namespace storage
} // namespace restaurant
