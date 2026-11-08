#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <fstream>
#include <string>
#include <vector>
#include "exceptions.h"

template <typename T>
class Repo {
private:
    T *items;
    int count;
    int capacity;

public:
    Repo(int max = 100) {
        if (max < 1) {
            max = 1;
        }
        capacity = max;
        count = 0;
        items = new T[capacity];
    }

    Repo(const Repo &other) {
        capacity = other.capacity;
        count = other.count;
        items = new T[capacity];
        for (int i = 0; i < count; i++) {
            items[i] = other.items[i];
        }
    }

    Repo& operator=(const Repo &other) {
        if (this == &other) {
            return *this;
        }

        delete[] items;

        capacity = other.capacity;
        count = other.count;
        items = new T[capacity];
        for (int i = 0; i < count; i++) {
            items[i] = other.items[i];
        }
        return *this;
    }

    ~Repo() {
        delete[] items;
    }

    void add(const T &item) {
        if (count >= capacity) {
            throw RestaurantException("Storage is full.");
        }
        items[count] = item;
        count = count + 1;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count) {
            throw RestaurantException("Invalid storage position.");
        }
        for (int i = index; i < count - 1; i++) {
            items[i] = items[i + 1];
        }
        count = count - 1;
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
};

template <typename T>
class FileStore {
private:
    std::string filename;

public:
    FileStore(const std::string &name) : filename(name) {}

    void append(const T &item) {
        std::ofstream output(filename.c_str(), std::ios::binary | std::ios::app);
        if (!output) {
            throw RestaurantException("Cannot open file: " + filename);
        }

        output.write((const char *)&item, sizeof(T));
        if (!output) {
            throw RestaurantException("Cannot write file: " + filename);
        }
        output.close();
    }

    std::vector<T> readAll() const {
        std::vector<T> list;
        std::ifstream input(filename.c_str(), std::ios::binary);
        if (!input) {
            return list;
        }

        T item;
        while (input.read((char *)&item, sizeof(T))) {
            list.push_back(item);
        }
        input.close();
        return list;
    }

    int count() const {
        std::ifstream input(filename.c_str(), std::ios::binary);
        if (!input) {
            return 0;
        }

        input.seekg(0, std::ios::end);
        std::streamoff size = input.tellg();
        input.close();

        if (size <= 0) {
            return 0;
        }
        return (int)(size / (std::streamoff)sizeof(T));
    }

    bool readAt(int position, T &item) const {
        if (position < 0) {
            return false;
        }

        std::ifstream input(filename.c_str(), std::ios::binary);
        if (!input) {
            return false;
        }

        input.seekg((std::streamoff)position * (std::streamoff)sizeof(T), std::ios::beg);
        bool success = (bool)input.read((char *)&item, sizeof(T));
        input.close();
        return success;
    }

    bool updateAt(int position, const T &item) {
        if (position < 0) {
            return false;
        }

        std::fstream file(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
        if (!file) {
            return false;
        }

        file.seekp((std::streamoff)position * (std::streamoff)sizeof(T), std::ios::beg);
        file.write((const char *)&item, sizeof(T));
        bool success = file.good();
        file.close();
        return success;
    }

    int findPosition(int id) const {
        int total = count();
        for (int i = 0; i < total; i++) {
            T item;
            if (readAt(i, item) && item.getId() == id) {
                return i;
            }
        }
        return -1;
    }
};

template <typename T>
void sortItems(T *items, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (items[j] < items[i]) {
                T temporary = items[i];
                items[i] = items[j];
                items[j] = temporary;
            }
        }
    }
}

#endif
