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
        template <class PostFieldIterable>
        static std::optional<std::string> post(const std::string& url, const PostFieldIterable& postFieldss) {
            return singleton().postImpl(url, postFieldss);
        }
    private:
        std::optional<std::string> getImpl(const std::string& url);
        std::string postifyMappedValues(const std::unordered_map<std::string, std::string>& postFields);
        std::optional<std::string> postImpl(const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
        template <class PostFieldIterable>
        std::optional<std::string> postImpl(const std::string& url, const PostFieldIterable& postFieldss) {
            data.clear();
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);

            std::string postStr;
            if (postFieldss.size() == 0) {
                postStr = "";
            } else {
                postStr = postifyMappedValues(*postFieldss.begin());
                for (auto postFieldsIt = std::next(postFieldss.begin()); postFieldsIt != postFieldss.end(); ++postFieldsIt) {
                    postStr += "&" + postifyMappedValues(*postFieldsIt);
                }
            }
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postStr.c_str());

            CURLcode status = curl_easy_perform(handle);
            if (status != CURLE_OK) {
                return std::optional<std::string>();
            } else {
                return std::optional<std::string>(std::string(std::move(data)));
            }
        }

        static Request& singleton();

        CURL* handle;
        std::string data;
        std::size_t handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);
        static std::size_t handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);

        Request();
};