#pragma once

#include "message.hpp"

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual void send(const Message& message) const;
};