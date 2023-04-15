#include "request.hpp"

#include <ctime>

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

std::string* Request::get(const std::string& url) {
    return singleton().getImpl(url);
}
std::string* Request::getImpl(const std::string& url) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return nullptr;
    } else {
        return &data;
    }
}

std::string* Request::authGet(const std::string& url, const std::string& oauthBearer) {
    return singleton().authGetImpl(url, oauthBearer);
}
std::string* Request::authGetImpl(const std::string& url, const std::string& oauthBearer) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_XOAUTH2_BEARER, oauthBearer.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    curl_easy_setopt(handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return nullptr;
    } else {
        return &data;
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

std::string* Request::post(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    return singleton().postImpl(url, postFields);
}
std::string* Request::postImpl(const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    data.clear();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postifyMappedValues(postFields).c_str());

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return nullptr;
    } else {
        return &data;
    }
}

std::string* Request::smtp(
    const std::string& url,
    const std::string& oauthBearer,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    return singleton().smtpImpl(url, oauthBearer, headers, mimeData);
}
std::string* Request::smtpImpl(
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

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return nullptr;
    } else {
        return &data;
    }
}

std::string* Request::imap(const std::string& url, const std::string& oauthBearer, const std::string& imapCommands) {
    return singleton().imapImpl(url, oauthBearer, imapCommands);
}
std::string* Request::imapImpl(const std::string& url, const std::string& oauthBearer, const std::string& imapCommands) {
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, handleDataStatic);
    curl_easy_setopt(handle, CURLOPT_XOAUTH2_BEARER, oauthBearer.c_str());
    if (imapCommands != "") {
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, imapCommands.c_str());
    }

    auto status = curl_easy_perform(handle);
    if (status != CURLE_OK) {
        return nullptr;
    } else {
        return &data;
    }
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
    stringRepresentation_ += text;
}

const std::string& Request::MimeData::stringRepresentation() const {
    if (!onlyStringContents) {
        throw std::logic_error("Grabbed string representation of non-string only mime data");
    }
    return stringRepresentation_;
}

const curl_mime* Request::MimeData::curlMimePost() const {
    return mime;
}