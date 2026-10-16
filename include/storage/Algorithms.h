#pragma once
#include <vector>
#include <algorithm>
#include <functional>

namespace restaurant {
namespace storage {

// A small collection of function templates used across the project to sort and
// rank data (for example the top-selling dish or the best waiter).

// Sorts a range in place using the supplied comparator.
template <class RandomIt, class Compare>
void sortRange(RandomIt begin, RandomIt end, Compare compare) {
    std::sort(begin, end, compare);
}

// Returns the n items with the largest key, largest first.
template <class T, class KeyFn>
std::vector<T> topN(const std::vector<T>& items, std::size_t n, KeyFn key) {
    std::vector<T> copy(items);
    std::sort(copy.begin(), copy.end(),
              [&key](const T& a, const T& b) { return key(a) > key(b); });
    if (copy.size() > n) copy.resize(n);
    return copy;
}

// Returns the sum of key(item) over every item.
template <class Container, class KeyFn>
double sumBy(const Container& items, KeyFn key) {
    double total = 0.0;
    for (typename Container::const_iterator it = items.begin(); it != items.end(); ++it)
        total += static_cast<double>(key(*it));
    return total;
}

} // namespace storage
} // namespace restaurant
