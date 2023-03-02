#include "request.hpp"

#include <curl/curl.h>

Request::Request() {
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
}

std::unique_ptr<Request> requestSingleton_ = nullptr;
Request& Request::singleton() {
    if (!requestSingleton_) {
        requestSingleton_ = std::unique_ptr<Request>(new Request);
    }

    return *requestSingleton_;
}

std::size_t Request::handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData) {
    data.append(static_cast<const char*>(buffer), bufferLength);
    return bufferLength;
}

std::size_t Request::handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData) {
    return Request::singleton().handleData(buffer, size, bufferLength, userData);
}

std::optional<std::string> Request::get(const std::string& url) {
    return singleton().getImpl(url);
}

std::optional<std::string> Request::getImpl(const std::string& url) {
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

std::string Request::postifyMappedValues(const std::unordered_map<std::string, std::string>& postFields) {
    if (postFields.size() == 0) {
        return "";
    }

    std::string postStr;
    postStr += postFields.begin()->first + "=" + postFields.begin()->second;
    for (auto postOptionIt = std::next(postFields.begin()); postOptionIt != postFields.end(); ++postOptionIt) {
        postStr += "&" + postOptionIt->first + "=" + postOptionIt->second;
    }
    return postStr;
}

std::optional<std::string> Request::post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    return singleton().postImpl(url, postFields);
}

std::optional<std::string> Request::postImpl(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postifyMappedValues(postFields).c_str());

    CURLcode status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}