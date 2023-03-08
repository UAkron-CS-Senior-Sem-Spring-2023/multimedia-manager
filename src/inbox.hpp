#pragma once

#include "request.hpp"

class Inbox {
    public:
        virtual ~Inbox() = default;

        class iterator {
            public:
                virtual ~iterator() = default;

                using iterator_category = std::forward_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = Request::MimeData;
                using pointer = Request::MimeData*;
                using const_pointer = const Request::MimeData*;
                using reference = Request::MimeData&;
                using const_reference = const Request::MimeData&;

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