#include "customizable-get-messaging-recipient.hpp"

#include "request.hpp"

CustomizableGetMessagingRecipient::CustomizableGetMessagingRecipient(std::string url, std::size_t messageInsertionPoint)
    : url(std::move(url)), messageInsertionPoint(messageInsertionPoint)
{
    if (messageInsertionPoint > url.size()) {
        messageInsertionPoint = url.size();
    }
}

std::optional<std::string> CustomizableGetMessagingRecipient::send(const Request::MimeData& message) const {
    return Request::get(url.substr(0, messageInsertionPoint) + message.stringRepresentation() + url.substr(messageInsertionPoint)); 
}