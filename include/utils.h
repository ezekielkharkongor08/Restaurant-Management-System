#ifndef RMS_UTILS_H
#define RMS_UTILS_H

#include <string>

namespace restaurant {

int readInt(const std::string &prompt);
double readDouble(const std::string &prompt);
std::string trim(const std::string &s);
void copyToCharArray(const std::string &src, char *dest, int destSize);

}

#endif
