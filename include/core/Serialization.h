#pragma once
#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <ostream>
#include <istream>
#include <string>

namespace restaurant {
namespace core {

// Serialization helpers shared by every class that is written to a binary
// file.  Primitives are written as raw bytes and strings as [length][chars].
template <class T>
inline void writeValue(std::ostream& os, const T& value) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <class T>
inline void readValue(std::istream& is, T& value) {
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
}

inline void writeString(std::ostream& os, const std::string& s) {
    std::uint64_t n = static_cast<std::uint64_t>(s.size());
    writeValue(os, n);
    if (n) os.write(s.data(), static_cast<std::streamsize>(n));
}

inline void readString(std::istream& is, std::string& s) {
    std::uint64_t n = 0;
    readValue(is, n);
    s.resize(static_cast<std::size_t>(n));
    if (n) is.read(&s[0], static_cast<std::streamsize>(n));
}

} // namespace core
} // namespace restaurant
