#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <sstream>
#include <string>
#include "core/Exceptions.h"

namespace restaurant {
namespace storage {

// Repository<T> is a general container that owns T* pointers.
// It is a class template, so the same code stores Staff, Dish, Ingredient,
// Order and Table objects.  It has a deep-copy constructor and a destructor
// that deletes every object it holds.
template <class T>
class Repository {
public:
    Repository() {}

    Repository(const Repository& other) {
        for (std::size_t i = 0; i < other.items_.size(); ++i)
            items_.push_back(other.items_[i]->clone());
    }

    Repository& operator=(const Repository& other) {
        if (this != &other) {
            clear();
            for (std::size_t i = 0; i < other.items_.size(); ++i)
                items_.push_back(other.items_[i]->clone());
        }
        return *this;
    }

    ~Repository() { clear(); }

    void add(T* item) {
        if (item) items_.push_back(item);
    }

    // find() throws if no item has the requested id.
    T* find(int id) const {
        for (std::size_t i = 0; i < items_.size(); ++i)
            if (items_[i]->id() == id) return items_[i];
        throw core::RestaurantException("No item with id " + idToString(id) +
                                        " was found");
    }

    T* findOrNull(int id) const {
        for (std::size_t i = 0; i < items_.size(); ++i)
            if (items_[i]->id() == id) return items_[i];
        return 0;
    }

    bool remove(int id) {
        for (typename std::vector<T*>::iterator it = items_.begin();
             it != items_.end(); ++it) {
            if ((*it)->id() == id) {
                delete *it;
                items_.erase(it);
                return true;
            }
        }
        return false;
    }

    bool update(int id, const T& replacement) {
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (items_[i]->id() == id) {
                T* copy = replacement.clone();
                delete items_[i];
                items_[i] = copy;
                return true;
            }
        }
        return false;
    }

    std::vector<T*> filter(std::function<bool(const T&)> predicate) const {
        std::vector<T*> out;
        for (std::size_t i = 0; i < items_.size(); ++i)
            if (predicate(*items_[i])) out.push_back(items_[i]);
        return out;
    }

    T* findWhere(std::function<bool(const T&)> predicate) const {
        for (std::size_t i = 0; i < items_.size(); ++i)
            if (predicate(*items_[i])) return items_[i];
        return 0;
    }

    std::size_t size() const { return items_.size(); }

    T* at(std::size_t index) const {
        if (index >= items_.size())
            throw core::RestaurantException("Repository index out of range");
        return items_[index];
    }

    std::vector<T*>& all() { return items_; }
    const std::vector<T*>& all() const { return items_; }

    void clear() {
        for (std::size_t i = 0; i < items_.size(); ++i) delete items_[i];
        items_.clear();
    }

private:
    static std::string idToString(int id) {
        std::ostringstream os;
        os << id;
        return os.str();
    }

    std::vector<T*> items_;
};

} // namespace storage
} // namespace restaurant
