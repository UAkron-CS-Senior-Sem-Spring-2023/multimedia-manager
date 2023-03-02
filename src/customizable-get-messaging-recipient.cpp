#include "customizable-get-messaging-recipient.hpp"

#include "request.hpp"

CustomizableGetMessagingRecipient::CustomizableGetMessagingRecipient(std::string url, std::size_t messageInsertionPoint)
    : url(std::move(url)), messageInsertionPoint(messageInsertionPoint)
{
    if (messageInsertionPoint > url.size()) {
        messageInsertionPoint = url.size();
    }
}

std::optional<std::string> CustomizableGetMessagingRecipient::send(const Message& message) const {
    return Request::get(url.substr(0, messageInsertionPoint) + messageToString(message) + url.substr(messageInsertionPoint)); 
}