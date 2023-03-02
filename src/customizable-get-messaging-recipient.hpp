#pragma once

#include "messaging-recipient.hpp"

#include <limits>

class CustomizableGetMessagingRecipient : public MessagingRecipient {
    public:
        CustomizableGetMessagingRecipient(std::string url, std::size_t messageInsertionPoint = std::numeric_limits<std::size_t>::max());

        std::optional<std::string> send(const Message& message) const override;

    private:
        std::string url;
        std::size_t messageInsertionPoint;
};