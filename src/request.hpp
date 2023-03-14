#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
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
                template <class ToRecipientsContainer>
                SMTPHeaders(std::string from, std::string subject, const ToRecipientsContainer& toRecipients)
                    : from(from) {
                    if (toRecipients.size() == 0) {
                        throw std::logic_error("There must be at least 1 recipient");
                    }
                    if (from.size() < 3) {
                        throw std::logic_error("The email must have been sent from somebody (i.e. at least \"<a>\" (3) in length");
                    }

                    std::string dateStr = "Date: ";
                    // - 1 to exclude null terminator
                    dateStr.resize(sizeof("Date: Tue, 22 Aug 2020 22:33:44 -0200") - 1);
                    time_t now_t = time(NULL);
                    tm* now = localtime(&now_t);
                    // example "Tue, 22 Aug 2020 22:33:44 -0200"
                    std::strftime(dateStr.data() + sizeof("Date: ") - 1, 31, "%a, %d %b %Y %H:%M:%S %z", now);
                    curlHeaders_ = curl_slist_append(curlHeaders_, dateStr.c_str());

                    std::string toStr = "To:";
                    for (const auto& toRecipient : toRecipients) {
                        curlMailRecipients_ = curl_slist_append(curlMailRecipients_, toRecipient.c_str());
                        toStr += " " + toRecipient;
                    }
                    curlHeaders_ = curl_slist_append(curlHeaders_, toStr.c_str());

                    std::string fromStr = "From: " + from;
                    curlHeaders_ = curl_slist_append(curlHeaders_, fromStr.c_str());

                    std::string messageIdStr = "Message-ID: <";

                    std::random_device dev;
                    std::mt19937 rng(dev());
                    std::uniform_int_distribution<std::mt19937::result_type> dist(0,35);
                    const char MESSAGE_ID_CHARACTERS[] = {
                        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
                    };
                    for (int i = 0; i < 40; ++i) {
                        messageIdStr += MESSAGE_ID_CHARACTERS[dist(rng)];
                    }
                    messageIdStr += "@mmanager>";

                    curlHeaders_ = curl_slist_append(curlHeaders_, messageIdStr.c_str());

                    std::string subjectStr = "Subject: " + subject;
                    curlHeaders_ = curl_slist_append(curlHeaders_, subjectStr.c_str());
                }

                template <class ToRecipientsContainer, class CCRecipientsContainer>
                SMTPHeaders(std::string from, std::string subject, const ToRecipientsContainer& toRecipients, const CCRecipientsContainer& ccRecipients)
                    : SMTPHeaders(from, subject, toRecipients)
                {
                    std::string ccStr = "Cc:";
                    for (const auto& ccRecipient : ccRecipients) {
                        curlMailRecipients_ = curl_slist_append(curlMailRecipients_, ccRecipient.c_str());
                        ccStr += " " + ccRecipient;
                    }
                    curlHeaders_ = curl_slist_append(curlHeaders_, ccStr.c_str());
                }

                SMTPHeaders(const SMTPHeaders&) = delete;
                SMTPHeaders& operator=(const SMTPHeaders&) = delete;
                SMTPHeaders(SMTPHeaders&& headers);

                ~SMTPHeaders();

                const std::string& curlMailFrom() const;
                const curl_slist* curlMailRecipients() const;
                const curl_slist* curlHeaders() const;
            private:
                mutable curl_slist* curlMailRecipients_ = nullptr;
                mutable curl_slist* curlHeaders_ = nullptr;

                std::string from;
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