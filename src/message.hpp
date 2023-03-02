#pragma once

#include "media.hpp"

#include <memory>
#include <vector>


class MessageConstructor;
class Message {
    public:
        Message(MessageConstructor messageConstructor);
        std::string toString() const;

    private:
        static const bool STRING_PORTION = false;
        static const bool MEDIA_PORTION = true;
        std::vector<bool> orderedPortionsUsed;
        std::vector<std::string> stringPortions;
        std::vector<Media> mediaPortions;


        friend class MessageConstructor;
};

class MessageConstructor {
    void append(std::string string);
    void append(Media media);

    private:
        std::vector<bool> orderedPortionsUsed;
        std::vector<std::string> stringPortions;
        std::vector<Media> mediaPortions;

        friend class Message;
};