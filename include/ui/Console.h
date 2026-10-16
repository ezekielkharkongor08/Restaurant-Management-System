#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "core/Money.h"

namespace restaurant {
namespace ui {

// Small terminal helpers shared by every menu.  They never let bad keyboard
// input crash the program: they simply ask again.

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cin.clear();
        return std::string();
    }
    return line;
}

inline int readInt(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            std::size_t pos = 0;
            int value = std::stoi(s, &pos);
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos == s.size()) return value;
        } catch (const std::exception&) {
        }
        std::cout << "  ! Please enter a whole number.\n";
    }
}

inline double readDouble(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            std::size_t pos = 0;
            double value = std::stod(s, &pos);
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos == s.size()) return value;
        } catch (const std::exception&) {
        }
        std::cout << "  ! Please enter a number.\n";
    }
}

inline core::Money readMoney(const std::string& prompt) {
    return core::Money::fromRupees(readDouble(prompt));
}

inline void pressEnter() {
    readLine("\nPress ENTER to continue...");
}

} // namespace ui
} // namespace restaurant
