#ifndef RMS_TEMPLATES_H
#define RMS_TEMPLATES_H

#include <cstdio>
#include <string>
#include <vector>
#include "exceptions.h"

namespace restaurant {

// Class template: generic container for objects (used for menu and payslip history)
template <typename T>
class Repo {
private:
    T *items;
    int capacity;
    int count;
public:
    Repo() : items(NULL), capacity(0), count(0) {}

    // Copy constructor (deep copy)
    Repo(const Repo &other) : items(NULL), capacity(0), count(0) {
        *this = other;
    }

    // Assignment operator (deep copy)
    Repo& operator=(const Repo &other) {
        if (this == &other) return *this;
        clear();
        capacity = other.capacity;
        count = other.count;
        if (capacity > 0) {
            items = new T[capacity];
            for (int i = 0; i < count; i++) {
                items[i] = other.items[i];
            }
        } else {
            items = NULL;
        }
        return *this;
    }

    ~Repo() {
        clear();
    }

    void clear() {
        if (items != NULL) {
            delete[] items;
            items = NULL;
        }
        capacity = 0;
        count = 0;
    }

    void add(const T &item) {
        if (count >= capacity) {
            int newCap = (capacity == 0) ? 4 : capacity * 2;
            T *newItems = new T[newCap];
            for (int i = 0; i < count; i++) {
                newItems[i] = items[i];
            }
            if (items != NULL) {
                delete[] items;
            }
            items = newItems;
            capacity = newCap;
        }
        items[count] = item;
        count++;
    }

    T& operator[](int index) {
        return items[index];
    }

    const T& operator[](int index) const {
        return items[index];
    }

    int size() const {
        return count;
    }

    T* rawData() {
        return items;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count) return;
        for (int i = index; i < count - 1; i++) {
            items[i] = items[i + 1];
        }
        count--;
    }
};

// Class template: generic binary file storage with random access (seek-based)
template <typename T>
class FileStore {
private:
    std::string filename;
public:
    explicit FileStore(const std::string &fname) : filename(fname) {}

    void append(const T &record) {
        FILE *f = std::fopen(filename.c_str(), "ab");
        if (f == NULL) throw RestaurantException("Cannot open file for append: " + filename);
        std::fwrite(&record, sizeof(T), 1, f);
        std::fclose(f);
    }

    int count() {
        FILE *f = std::fopen(filename.c_str(), "rb");
        if (f == NULL) return 0;
        std::fseek(f, 0, SEEK_END);
        long size = std::ftell(f);
        std::fclose(f);
        return (int)(size / sizeof(T));
    }

    bool readAt(int index, T &record) {
        FILE *f = std::fopen(filename.c_str(), "rb");
        if (f == NULL) return false;
        std::fseek(f, index * sizeof(T), SEEK_SET);
        size_t n = std::fread(&record, sizeof(T), 1, f);
        std::fclose(f);
        return (n == 1);
    }

    bool updateAt(int index, const T &record) {
        FILE *f = std::fopen(filename.c_str(), "r+b");
        if (f == NULL) return false;
        std::fseek(f, index * sizeof(T), SEEK_SET);
        size_t n = std::fwrite(&record, sizeof(T), 1, f);
        std::fclose(f);
        return (n == 1);
    }

    std::vector<T> readAll() {
        std::vector<T> result;
        FILE *f = std::fopen(filename.c_str(), "rb");
        if (f == NULL) return result;
        T rec;
        while (std::fread(&rec, sizeof(T), 1, f) == 1) {
            result.push_back(rec);
        }
        std::fclose(f);
        return result;
    }
};

// Function template: simple sort (bubble sort) using operator<
template <typename T>
void sortItems(T *items, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (items[j] < items[i]) {
                T temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }
}

// Function template: sum of array elements using operator+
template <typename T>
T arraySum(T *items, int count) {
    T sum = T();
    for (int i = 0; i < count; i++) {
        sum = sum + items[i];
    }
    return sum;
}

}

#endif
