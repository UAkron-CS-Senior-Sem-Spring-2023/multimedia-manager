#pragma once

#include "inbox.hpp"

class MessagingSource {
    public:
        virtual ~MessagingSource() = default;

        virtual const Inbox& inbox() const = 0;
};