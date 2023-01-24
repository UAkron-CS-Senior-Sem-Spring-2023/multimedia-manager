#pragma once

#include "message.hpp"

class Inbox {
    public:
        virtual ~Inbox() = default;

        class iterator {
            public:
                virtual ~iterator() = default;

                using iterator_category = std::forward_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = Message;
                using pointer = Message*;
                using const_pointer = const Message*;
                using reference = Message&;
                using const_reference = const Message&;

                virtual const_reference operator*() const = 0;
                virtual iterator& operator++();
                virtual iterator operator++(int);
                virtual bool operator==(const iterator& other) const;
                virtual bool operator!=(const iterator& other) const;
        };

        virtual iterator begin() const;
        virtual iterator cbegin() const;
        virtual iterator end() const;
        virtual iterator cend() const;
};