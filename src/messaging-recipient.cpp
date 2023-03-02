#include "messaging-recipient.hpp"

std::string MessagingRecipient::messageToString(const Message& message) const {
    return message.toString();
}