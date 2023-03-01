#include "request.hpp"

#include <curl/curl.h>

ImplRequest::ImplRequest() {
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
}

ImplRequest& ImplRequest::singleton() {
    if (!singleton_) {
        singleton_ = std::unique_ptr<ImplRequest>(new ImplRequest);
    }

    return *singleton_;
}

std::unique_ptr<ImplRequest> ImplRequest::singleton_ = nullptr;

std::size_t ImplRequest::handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData) {
    data.append(static_cast<const char*>(buffer), bufferLength);
    return bufferLength;
}

std::size_t ImplRequest::handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData) {
    return ImplRequest::singleton().handleData(buffer, size, bufferLength, userData);
}

std::optional<std::string> ImplRequest::send(const char* url) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    CURLcode status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

std::optional<std::string> Request::send(const char* url) {
    return ImplRequest::singleton().send(url);
}