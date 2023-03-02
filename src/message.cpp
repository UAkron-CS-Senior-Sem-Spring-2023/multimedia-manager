#include "message.hpp"

#include <stdexcept>

Message::Message(MessageConstructor messageConstructor)
    : orderedPortionsUsed(std::move(messageConstructor.orderedPortionsUsed)),
    stringPortions(std::move(messageConstructor.stringPortions)),
    mediaPortions(std::move(messageConstructor.mediaPortions))
{}

void MessageConstructor::append(std::string string) {
    stringPortions.push_back(std::move(string));
    orderedPortionsUsed.push_back(Message::STRING_PORTION);
}

void MessageConstructor::append(Media media) {
    mediaPortions.push_back(std::move(media));
    orderedPortionsUsed.push_back(Message::MEDIA_PORTION);
}

std::string Message::toString() const {
    std::string messageString;

    std::size_t stringsUsed = 0;
    std::size_t mediasUsed = 0;
    for (std::size_t i = 0; i < orderedPortionsUsed.size(); ++i) {
        if (orderedPortionsUsed[i] == STRING_PORTION) {
            messageString += stringPortions[stringsUsed];
            ++stringsUsed;
        } else if (orderedPortionsUsed[i] == MEDIA_PORTION) {
            messageString += std::string("[File attachment #") + std::to_string(mediasUsed + 1) + "]";;
            ++mediasUsed;
        } else {
            throw std::logic_error("The portion type of the message is not a valid portion type");
        }
    }

    return messageString;
}