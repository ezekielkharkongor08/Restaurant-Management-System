#pragma once
#include <exception>
#include <string>

namespace restaurant {
namespace core {

// Base class for every error the restaurant can raise.  It derives from
// std::exception so it can be caught like a standard exception.
class RestaurantException : public std::exception {
public:
    explicit RestaurantException(const std::string& message) : message_(message) {}
    virtual ~RestaurantException() throw() {}
    const char* what() const throw() { return message_.c_str(); }
    const std::string& message() const { return message_; }

private:
    std::string message_;
};

// Wrong input such as a negative salary or a malformed date.
class InvalidInputException : public RestaurantException {
public:
    explicit InvalidInputException(const std::string& message)
        : RestaurantException(message) {}
};

// No staff member exists with the requested ID.
class StaffNotFoundException : public RestaurantException {
public:
    explicit StaffNotFoundException(const std::string& message)
        : RestaurantException(message) {}
};

// Not enough ingredients in the inventory to prepare a dish.
class InsufficientStockException : public RestaurantException {
public:
    explicit InsufficientStockException(const std::string& message)
        : RestaurantException(message) {}
};

// A role tried to perform an action it is not allowed to do.
class UnauthorizedAccessException : public RestaurantException {
public:
    explicit UnauthorizedAccessException(const std::string& message)
        : RestaurantException(message) {}
};

// A data file on disk is damaged or in an unexpected format.
class FileCorruptException : public RestaurantException {
public:
    explicit FileCorruptException(const std::string& message)
        : RestaurantException(message) {}
};

} // namespace core
} // namespace restaurant
