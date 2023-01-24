#pragma once

#include "message.hpp"

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual send(const Message& message) const;
};