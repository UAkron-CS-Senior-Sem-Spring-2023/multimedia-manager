#pragma once

#include <string>
#include <string_view>

class Media {
    public:
        static Media fromBytes(std::string contents);
        static Media fromFile(std::string_view fileName);

        std::string_view contents() const;
    private:
        Media(std::string contents)
            : contents_(std::move(contents))
        {}

        std::string contents_;
};