#pragma once

#include <memory>
#include <optional>
#include <string>

#include <curl/curl.h>

class ImplRequest {
    public:
        static ImplRequest& singleton();

        std::optional<std::string> send(const char* url);
    private:

        CURL* handle;
        std::string data;
        std::size_t handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);
        static std::size_t handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);

        ImplRequest();
        static std::unique_ptr<ImplRequest> singleton_;
};

namespace Request {
    std::optional<std::string> send(const char* url);
};