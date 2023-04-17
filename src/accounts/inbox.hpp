#pragma once

#include "request.hpp"
#include <chrono>

#include <QObject>

class Inbox : public QObject {
    Q_OBJECT

    public:
        struct Info {
            std::chrono::high_resolution_clock::time_point time;
            std::size_t uniqueId;
            std::string from;
            std::string to;
            std::string subject;
        };

        Inbox(QObject* parent = nullptr);
        virtual ~Inbox() = default;

        class iterator {
            public:
                virtual ~iterator() = default;

                using iterator_category = std::forward_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = std::pair<const Info, std::string>;
                using pointer = std::pair<const Info, std::string>*;
                using const_pointer = const std::pair<const Info, std::string>*;
                using reference = std::pair<const Info, std::string>&;
                using const_reference = const std::pair<const Info, std::string>&;

                virtual value_type operator*() const = 0;
                virtual iterator& operator++() = 0;
                virtual std::unique_ptr<iterator> operator++(int) = 0;
                virtual bool operator==(const iterator& other) const = 0;
                virtual bool operator!=(const iterator& other) const = 0;
        };

        virtual std::unique_ptr<iterator> begin() const = 0;
        virtual std::unique_ptr<iterator> end() const = 0;

        virtual void populate() const = 0;
    signals:
        void populatedInbox(const Inbox* inbox) const;
};

bool operator<(const Inbox::Info& infoLHS, const Inbox::Info& infoRHS);