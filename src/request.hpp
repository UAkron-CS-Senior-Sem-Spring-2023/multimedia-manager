#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

#include <curl/curl.h>

struct ImplRequest {
    struct Deleters {
        void operator()(curl_mime* mime);
    };
};

class Request {
    public:
        class SMTPHeaders {
            public:
                SMTPHeaders(std::string from, std::vector<std::string> toRecipients, std::vector<std::string> ccRecipients, std::string subject);
                ~SMTPHeaders();

                const std::string& curlMailFrom() const;
                const curl_slist* curlMailRecipients() const;
                const curl_slist* curlHeaders() const;
            private:
                mutable curl_slist* curlMailRecipients_ = nullptr;
                mutable curl_slist* curlHeaders_ = nullptr;

                std::string from;
                std::vector<std::string> toRecipients;
                std::vector<std::string> ccRecipients;
                std::string subject;
        };

        class MimeData {
            public:
                MimeData();

                void addText(const std::string& text);

                const std::string& stringRepresentation() const;
                const curl_mime* curlMimePost() const;
            private:
                bool onlyStringContents = true;
                std::string stringRepresentation_;
                curl_mime* mime;
        };


        static std::string* get(const std::string& url);
        static std::string* authGet(const std::string& url, const std::string& oauthBearer);
        static std::string* post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
        template <class PostFieldIterable>
        static std::string* post(const std::string& url, const PostFieldIterable& postFieldss) {
            return singleton().postImpl(url, postFieldss);
        }
        
        static std::string* smtp(
            const std::string& url,
            const std::string& oauthBearer,
            const SMTPHeaders& headers,
            const MimeData& mimeData
        );

        static std::string* imap(const std::string& url, const std::string& username, const std::string& oauthBearer);
    private:
        std::string* getImpl(const std::string& url);
        std::string* authGetImpl(const std::string& url, const std::string& oauthBearer);
        std::string postifyMappedValues(const std::unordered_map<std::string, std::string>& postFields);
        std::string* postImpl(const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
        template <class PostFieldIterable>
        std::string* postImpl(const std::string& url, const PostFieldIterable& postFieldss) {
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

            auto status = curl_easy_perform(handle);
            if (status != CURLE_OK) {
                return nullptr;
            } else {
                return &data;
            }
        }
        
        std::string* smtpImpl(
            const std::string& url,
            const std::string& oauthBearer,
            const SMTPHeaders& headers,
            const MimeData& mimeData
        );

        std::string* imapImpl(const std::string& url, const std::string& username, const std::string& oauthBearer);

        static Request& singleton();

        CURL* handle;
        std::string data;
        std::size_t handleData(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);
        static std::size_t handleDataStatic(void* buffer, std::size_t size, std::size_t bufferLength, void* userData);

        Request();
};