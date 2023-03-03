#include "request.hpp"

#include <ctime>
#include <stdexcept>
#include <random>

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
    auto status = curl_easy_perform(handle);
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

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

#include <iostream>

std::optional<std::string> Request::smtp(
    const std::string& url,
    const std::string& oauthBearer,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    return singleton().smtpImpl(url, oauthBearer, headers, mimeData);
}

std::optional<std::string> Request::smtpImpl(
    const std::string& url,
    const std::string& oauthBearer,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_USERNAME, headers.curlMailFrom().c_str());
    curl_easy_setopt(handle, CURLOPT_XOAUTH2_BEARER, oauthBearer.c_str());
    curl_easy_setopt(handle, CURLOPT_MAIL_FROM, headers.curlMailFrom().c_str());
    curl_easy_setopt(handle, CURLOPT_MAIL_RCPT, headers.curlMailRecipients());
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers.curlHeaders());
    curl_easy_setopt(handle, CURLOPT_MIMEPOST, mimeData.curlMimePost());
    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    curl_easy_setopt(handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    char errbuf[CURL_ERROR_SIZE] = {0};
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errbuf);

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        std::cout << headers.curlMailFrom() << " " << oauthBearer << std::endl;
        std::cout << curl_easy_strerror(status) << std::endl;
        std::cout << std::string(errbuf, CURL_ERROR_SIZE) << std::endl;
        std::cout << data << std::endl;
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

std::optional<std::string> Request::imap(const std::string& url, const std::string& username, const std::string& oauthBearer) {
    return singleton().imapImpl(url, username, oauthBearer);
}

std::optional<std::string> Request::imapImpl(const std::string& url, const std::string& username, const std::string& oauthBearer) {
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(handle, CURLOPT_XOAUTH2_BEARER, oauthBearer.c_str());

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return std::optional<std::string>();
    } else {
        return std::optional<std::string>(std::string(std::move(data)));
    }
}

#include <iostream>

Request::SMTPHeaders::SMTPHeaders(std::string from_, std::vector<std::string> toRecipients_, std::vector<std::string> ccRecipients_, std::string subject_)
    : from(std::move(from_)), toRecipients(std::move(toRecipients_)), ccRecipients(std::move(ccRecipients_)), subject(std::move(subject_))
{
    if (toRecipients.size() == 0) {
        throw std::logic_error("There must be at least 1 recipient");
    }
    if (from.size() < 3) {
        throw std::logic_error("The email must have been sent from somebody (i.e. at least \"<a>\" (3) in length");
    }

    for (const auto& toRecipient : toRecipients) {
        curlMailRecipients_ = curl_slist_append(curlMailRecipients_, toRecipient.c_str());
    }
    for (const auto& ccRecipient : ccRecipients) {
        curlMailRecipients_ = curl_slist_append(curlMailRecipients_, ccRecipient.c_str());
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
        toStr += " " + toRecipient;
    }
    curlHeaders_ = curl_slist_append(curlHeaders_, toStr.c_str());

    std::string fromStr = "From: " + from;
    curlHeaders_ = curl_slist_append(curlHeaders_, fromStr.c_str());

    std::string ccStr = "Cc:";
    for (const auto& ccRecipient : ccRecipients) {
        toStr += " " + ccRecipient;
    }
    curlHeaders_ = curl_slist_append(curlHeaders_, ccStr.c_str());

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

Request::SMTPHeaders::~SMTPHeaders() {
    curl_slist_free_all(curlMailRecipients_);
    curl_slist_free_all(curlHeaders_);
}

const std::string& Request::SMTPHeaders::curlMailFrom() const {
    return from;
}

const curl_slist* Request::SMTPHeaders::curlMailRecipients() const {
    return curlMailRecipients_;
}

const curl_slist* Request::SMTPHeaders::curlHeaders() const {
    return curlHeaders_;
}

void ImplRequest::Deleters::operator()(curl_mime* mime) {
    curl_mime_free(mime);
}

Request::MimeData::MimeData()
    : mime(curl_mime_init(Request::singleton().handle))
{}

void Request::MimeData::addText(const std::string& text) {
    curl_mimepart* part = curl_mime_addpart(mime);
    curl_mime_data(part, text.c_str(), CURL_ZERO_TERMINATED);
}

const curl_mime* Request::MimeData::curlMimePost() const {
    return mime;
}