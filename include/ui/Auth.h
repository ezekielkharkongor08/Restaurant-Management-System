#pragma once
#include <string>
#include "staff/Staff.h"
#include "storage/Repository.h"

namespace restaurant {
namespace ui {

// Auth checks the user ID and password, finds the staff member and returns the
// matching role.  A wrong login throws UnauthorizedAccessException.
class Auth {
public:
    static staff::Staff* login(storage::Repository<staff::Staff>& staff,
                               int id, const std::string& password);
};

} // namespace ui
} // namespace restaurant
