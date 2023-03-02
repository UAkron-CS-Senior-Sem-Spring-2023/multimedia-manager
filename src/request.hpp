#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include <curl/curl.h>

class Request {
    public:
        static std::optional<std::string> get(const std::string& url);
        static std::optional<std::string> post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
    private:
        static Request& singleton();
        std::optional<std::string> getImpl(const std::string& url);
        std::optional<std::string> postImpl(const std::string& url, const std::unordered_map<std::string, std::string>& postFields);

        CURL* handle;
        std::string data;
        std::size_t handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);
        static std::size_t handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);

        Request();
};