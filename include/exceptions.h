#ifndef RMS_EXCEPTIONS_H
#define RMS_EXCEPTIONS_H

#include <exception>
#include <string>

namespace restaurant {

// One simple custom exception class used so the program never crashes on bad input
class RestaurantException : public std::exception {
private:
    std::string message;
public:
    explicit RestaurantException(const std::string &msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
    virtual ~RestaurantException() throw() {}
};

}

#endif
