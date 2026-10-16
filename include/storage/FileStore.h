#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <functional>
#include <cstdint>
#include "core/Exceptions.h"

namespace restaurant {
namespace storage {

// FileStore<T> saves and loads objects of type T in a binary file.
// Each record is written as [id][type tag][size][data].
// An ID -> file position index is kept so that a single record can be read or
// updated with random access (seekg / seekp) without rewriting the whole file.
template <class T>
class FileStore {
public:
    typedef std::function<int(const T&)> TagFn;
    typedef std::function<T*(int)> LoadFn;

    FileStore(const std::string& filename,
              TagFn tagger = TagFn(),
              LoadFn loader = LoadFn())
        : filename_(filename), tagger_(tagger), loader_(loader) {
        rebuildIndex();
    }

    // Read the file once and remember where every record lives.
    void rebuildIndex() {
        index_.clear();
        sizes_.clear();
        std::ifstream in(filename_.c_str(), std::ios::binary);
        if (!in) return;
        while (true) {
            std::streampos pos = in.tellg();
            int id = 0;
            in.read(reinterpret_cast<char*>(&id), sizeof(id));
            if (!in) break;
            int tag = 0;
            std::uint64_t size = 0;
            in.read(reinterpret_cast<char*>(&tag), sizeof(tag));
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            if (!in) throw core::FileCorruptException("Corrupt record in " + filename_);
            index_[id] = pos;
            sizes_[id] = static_cast<std::size_t>(size);
            in.seekg(static_cast<std::streamoff>(size), std::ios::cur);
        }
    }

    bool exists(int id) const { return index_.find(id) != index_.end(); }

    // Insert a new record or update an existing one (by id).
    void save(const T& obj) {
        int tag = tagger_ ? tagger_(obj) : 0;
        std::string body = serialize(obj);
        if (exists(obj.id()) && sizes_[obj.id()] == body.size()) {
            updateInPlace(obj.id(), body);
        } else {
            append(obj.id(), tag, body);
        }
    }

    T* load(int id) {
        std::map<int, std::streampos>::iterator it = index_.find(id);
        if (it == index_.end()) return 0;
        std::ifstream in(filename_.c_str(), std::ios::binary);
        if (!in) return 0;
        in.seekg(it->second);
        int rid = 0, rtag = 0;
        std::uint64_t size = 0;
        in.read(reinterpret_cast<char*>(&rid), sizeof(rid));
        in.read(reinterpret_cast<char*>(&rtag), sizeof(rtag));
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        std::string body(size, '\0');
        if (size) in.read(&body[0], static_cast<std::streamsize>(size));
        if (!loader_)
            throw core::FileCorruptException("No loader configured for " + filename_);
        T* obj = loader_(rtag);
        std::istringstream is(body, std::ios::binary);
        obj->read(is);
        return obj;
    }

    std::vector<T*> loadAll() {
        std::vector<T*> out;
        for (typename std::map<int, std::streampos>::iterator it = index_.begin();
             it != index_.end(); ++it) {
            T* obj = load(it->first);
            if (obj) out.push_back(obj);
        }
        return out;
    }

    bool remove(int id) {
        if (!exists(id)) return false;
        std::vector<T*> keep = loadAll();
        reset();
        for (std::size_t i = 0; i < keep.size(); ++i) {
            if (keep[i]->id() == id) {
                delete keep[i];
                continue;
            }
            save(*keep[i]);
            delete keep[i];
        }
        return true;
    }

    // Empty the file and forget the index (used for a clean full rewrite).
    void reset() {
        std::ofstream out(filename_.c_str(), std::ios::binary | std::ios::trunc);
        out.close();
        index_.clear();
        sizes_.clear();
    }

    const std::string& filename() const { return filename_; }
    std::size_t count() const { return index_.size(); }

private:
    std::string serialize(const T& obj) const {
        std::ostringstream os(std::ios::binary);
        obj.write(os);
        return os.str();
    }

    void append(int id, int tag, const std::string& body) {
        std::ofstream out(filename_.c_str(), std::ios::binary | std::ios::app);
        if (!out) throw core::FileCorruptException("Cannot write to " + filename_);
        std::streampos pos = out.tellp();
        std::uint64_t size = static_cast<std::uint64_t>(body.size());
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        out.write(reinterpret_cast<const char*>(&tag), sizeof(tag));
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        if (size) out.write(body.data(), static_cast<std::streamsize>(size));
        index_[id] = pos;
        sizes_[id] = body.size();
    }

    void updateInPlace(int id, const std::string& body) {
        std::fstream io(filename_.c_str(),
                        std::ios::binary | std::ios::in | std::ios::out);
        if (!io) throw core::FileCorruptException("Cannot update " + filename_);
        std::streamoff header = static_cast<std::streamoff>(
            sizeof(int) * 2 + sizeof(std::uint64_t));
        io.seekp(index_[id] + header);
        io.write(body.data(), static_cast<std::streamsize>(body.size()));
    }

    std::string filename_;
    TagFn tagger_;
    LoadFn loader_;
    std::map<int, std::streampos> index_;
    std::map<int, std::size_t> sizes_;
};

} // namespace storage
} // namespace restaurant
