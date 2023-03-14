#pragma once

#include <unordered_map>
#include <list>

#include "dynamic-assert.hpp"

template <class T>
class SetList {
    public:
        using iterator = typename std::list<T>::iterator;
        using const_iterator = typename std::list<T>::const_iterator;

        iterator begin() { return list.begin(); }
        iterator end() { return list.end(); }
        const_iterator begin() const { return list.begin(); }
        const_iterator end() const { return list.end(); }
        const_iterator cbegin() const { return list.cbegin(); }
        const_iterator cend() const { return list.cend(); }

        void push_front(T value) {
            dynamic_assert(count(value) == 0, "set already contains value");
            list.push_front(value);
            listIndex[value] = list.begin();
        }
        void push_back(T value) {
            dynamic_assert(count(value) == 0, "set already contains value");
            list.push_back(value);
            listIndex[value] = std::prev(list.end());
        }

        std::size_t size() const {
            return list.size();
        }

        void clear() {
            list.clear();
            listIndex.clear();
        }

        void erase(const T& value) {
            list.erase(listIndex[value]);
            listIndex.erase(value);
        }

        std::size_t count(const T& value) const {
            return listIndex.count(value);
        }

    private:
        std::list<T> list;
        std::unordered_map<T, typename std::list<T>::iterator> listIndex;
};