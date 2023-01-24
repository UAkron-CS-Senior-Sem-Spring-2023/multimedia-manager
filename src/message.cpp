#include "message.hpp"

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