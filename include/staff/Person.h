#pragma once
#include <string>
#include <ostream>
#include <istream>

namespace restaurant {
namespace staff {

// Person is the base class of every human in the restaurant.  It only holds
// basic personal data; it is deliberately kept independent of Staff.
class Person {
public:
    Person();
    Person(const std::string& name, const std::string& phone, const std::string& address);
    virtual ~Person();

    const std::string& name() const;
    const std::string& phone() const;
    const std::string& address() const;

    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setAddress(const std::string& address);

    virtual void print(std::ostream& os) const;

protected:
    // Serialization of the Person part only (used by Staff to avoid writing
    // the virtual base twice when the diamond classes are saved).
    void writeOwn(std::ostream& os) const;
    void readOwn(std::istream& is);

    std::string name_;
    std::string phone_;
    std::string address_;
};

} // namespace staff
} // namespace restaurant
