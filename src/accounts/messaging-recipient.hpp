#pragma once

#include "request.hpp"

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual void send(const std::string& subject, const Request::MimeData& message) const = 0;
};