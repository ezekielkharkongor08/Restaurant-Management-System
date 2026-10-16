#include "staff/Person.h"
#include "core/Serialization.h"

namespace restaurant {
namespace staff {

Person::Person() : name_(""), phone_(""), address_("") {}

Person::Person(const std::string& name, const std::string& phone, const std::string& address)
    : name_(name), phone_(phone), address_(address) {}

Person::~Person() {}

const std::string& Person::name() const { return name_; }
const std::string& Person::phone() const { return phone_; }
const std::string& Person::address() const { return address_; }

void Person::setName(const std::string& name) { name_ = name; }
void Person::setPhone(const std::string& phone) { phone_ = phone; }
void Person::setAddress(const std::string& address) { address_ = address; }

void Person::print(std::ostream& os) const {
    os << name_ << " (phone: " << phone_ << ", address: " << address_ << ")";
}

void Person::writeOwn(std::ostream& os) const {
    core::writeString(os, name_);
    core::writeString(os, phone_);
    core::writeString(os, address_);
}

void Person::readOwn(std::istream& is) {
    core::readString(is, name_);
    core::readString(is, phone_);
    core::readString(is, address_);
}

} // namespace staff
} // namespace restaurant
