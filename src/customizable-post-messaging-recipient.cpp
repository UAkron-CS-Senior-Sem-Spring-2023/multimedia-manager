#include "customizable-post-messaging-recipient.hpp"

#include "request.hpp"

CustomizablePostMessagingRecipient::CustomizablePostMessagingRecipient(std::string url, std::string postMessageFieldName, std::unordered_map<std::string, std::string> additionalPostFields)
    : url(std::move(url)), postMessageField({{std::move(postMessageFieldName), ""}}), postFields(std::move(additionalPostFields)), postFieldss({postMessageField, postFields})
{}

std::optional<std::string> CustomizablePostMessagingRecipient::send(const Message& message) const {
    postMessageField.begin()->second = std::move(messageToString(message));
    return Request::post(url, postFieldss);
}