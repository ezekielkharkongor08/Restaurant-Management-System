#include "utils.h"
#include "exceptions.h"
#include <iostream>
#include <limits>
#include <cstdio>
#include <cstring>

namespace restaurant {

int readInt(const std::string &prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw RestaurantException("Invalid input: expected an integer.");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

double readDouble(const std::string &prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw RestaurantException("Invalid input: expected a number.");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

std::string trim(const std::string &s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t')) start++;
    size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t')) end--;
    return s.substr(start, end - start);
}

void copyToCharArray(const std::string &src, char *dest, int destSize) {
    std::memcpy(dest, src.c_str(), (src.size() < (size_t)destSize) ? src.size() : (size_t)(destSize - 1));
    dest[(src.size() < (size_t)destSize) ? src.size() : (size_t)(destSize - 1)] = '\0';
}

}
