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

std::optional<std::string> ImplRequest::get(const std::string& url) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    CURLcode status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

std::optional<std::string> ImplRequest::post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);

    std::string postFieldsStr;
    for (const auto& postOption : postFields) {
        postFieldsStr += "&" + postOption.first + "=" + postOption.second;
    }

    if (postFieldsStr.size() > 0) {
        // + 1 to pass over initial &
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postFieldsStr.c_str() + 1);
    } else {
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, "");
    }

    CURLcode status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

std::optional<std::string> Request::get(const std::string& url) {
    return ImplRequest::singleton().get(url);
}

std::optional<std::string> Request::post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    return ImplRequest::singleton().post(url, postFields);
}