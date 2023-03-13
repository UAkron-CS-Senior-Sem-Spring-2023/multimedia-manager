#pragma once

#include "request.hpp"

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual void send(const Request::MimeData& message) const = 0;
};