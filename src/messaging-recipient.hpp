#pragma once

#include "request.hpp"

#include <optional>

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual std::optional<std::string> send(const Request::MimeData& message) const = 0;
};