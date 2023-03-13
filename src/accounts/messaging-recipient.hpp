#pragma once

#include "request.hpp"

class MessagingRecipient {
    public:
        virtual ~MessagingRecipient() = default;

        virtual std::string* send(const Request::MimeData& message) const = 0;
};