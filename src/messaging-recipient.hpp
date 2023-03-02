#pragma once

#include "message.hpp"

#include <optional>

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual std::optional<std::string> send(const Message& message) const = 0;
    protected:
        virtual std::string messageToString(const Message& message) const;
};