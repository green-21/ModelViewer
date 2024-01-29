#pragma once

#include <exception>
#include <string>
#include <unordered_map>

template <class T> class ResourceStorage {
public:
    T Get(std::string name) {
        auto iter = storage.find(name);
        if (iter == storage.end())
            throw std::exception("Not Found Exception");

        return (*iter).second;
    }
    void Insert(std::string name, T &&data) { storage[name] = data; }
    void Delete(std::string name) {
        auto iter = storage.find(name);
        if (iter == storage.end())
            return;
        storage.erase(iter);
    }

    bool Exist(std::string name) {
        auto iter = storage.find(name);
        return (iter != storage.end());
    }

private:
    std::unordered_map<std::string, T> storage;
};