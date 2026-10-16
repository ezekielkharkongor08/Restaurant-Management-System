#include "ui/Auth.h"
#include "core/Exceptions.h"

namespace restaurant {
namespace ui {

staff::Staff* Auth::login(storage::Repository<staff::Staff>& staff,
                          int id, const std::string& password) {
    staff::Staff* member = staff.findOrNull(id);
    if (!member)
        throw core::StaffNotFoundException("No staff member with ID " + std::to_string(id));
    if (member->password() != password)
        throw core::UnauthorizedAccessException("Wrong password for " + member->name());
    return member;
}

} // namespace ui
} // namespace restaurant
