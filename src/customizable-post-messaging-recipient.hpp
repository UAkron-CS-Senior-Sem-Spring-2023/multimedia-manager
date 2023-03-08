#pragma once

#include "messaging-recipient.hpp"

#include <array>
#include <functional>
#include <unordered_map>

class CustomizablePostMessagingRecipient : public MessagingRecipient {
    public:
        // CustomizablePostMessagingRecipient takes
        // a url directing what url to send the post request to
        // postMessageField, stating what post field should be filled with 
        CustomizablePostMessagingRecipient(std::string url, std::string postMessageField, std::unordered_map<std::string, std::string> additionalPostFields = {});

        std::optional<std::string> send(const Request::MimeData& message) const override;

    private:
        std::string url;
        mutable std::unordered_map<std::string, std::string> postMessageField;
        std::unordered_map<std::string, std::string> postFields;
        std::array<std::reference_wrapper<const std::unordered_map<std::string, std::string>>, 2> postFieldss;
}; 