#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class RestaurantException : public std::runtime_error {
public:
    RestaurantException(const std::string &message) : std::runtime_error(message) {}
};

#endif
