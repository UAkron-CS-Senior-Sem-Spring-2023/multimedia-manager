#include "q-request.hpp"

#include "request.hpp"

#include <charconv>
#include <limits>
#include <memory>
#include <string_view>
#include <ctime>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QTimer>

#include <nlohmann/json.hpp>
#include "base64/include/base64.hpp"
using json = nlohmann::json;
using namespace std::string_view_literals;

std::unique_ptr<QRequest> singleton_;
QRequest& QRequest::singleton() {
    if (!singleton_) {
        singleton_ = std::unique_ptr<QRequest>(new QRequest);
    }

    return *singleton_;
}

std::size_t QRequest::getUniqueRequest() {
    return request_++;
}

std::size_t QRequest::getNonUniqueRequest() {
    return std::numeric_limits<std::size_t>::max();
}

QRequest::QRequest(QObject* parent)
    : QObject(parent)
{}

void QRequest::get(std::size_t request, const std::string& url) {
    emit onResponse(request, Request::get(url));
}
void QRequest::authGet(std::size_t request, const std::string& url, const std::string& oauthBearer) {
    emit onResponse(request, Request::authGet(url, oauthBearer));
}

void QRequest::post(std::size_t request, const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    emit onResponse(request, Request::post(url, postFields));
}

void QRequest::smtp(
    std::size_t request,
    const std::string& url,
    const std::string& oauthBearer,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    emit onResponse(request, Request::smtp(url, oauthBearer, headers, mimeData));
}

void QRequest::imap(
    std::size_t request,
    const std::string& url,
    const std::string& user,
    const std::string& imapCommands
) {
    emit onResponse(request, Request::imap(url, user, imapCommands));
}
void QRequest::authIMAP(std::size_t request, const std::string& url, const std::string& user, const std::string& oauthBearer) {
    emit onResponse(request, Request::authIMAP(url, user, oauthBearer));
}

void QRequest::gmailGetUser(
    std::size_t request,
    const std::string& oauthBearer
) {
    std::string* response = Request::authGet("https://gmail.googleapis.com/gmail/v1/users/me/profile", oauthBearer);
    if (response == nullptr) {
        emit onResponse(request, nullptr);
        return;
    }

    json jsonResponse = json::parse(*response);
    delete response;
    if (!jsonResponse.contains("emailAddress")) {
        emit onResponse(request, nullptr);
    }
    emit onResponse(request, new std::string(jsonResponse.at("emailAddress")));
}

void QRequest::gmailOAuth(std::size_t requestAuthToken, std::size_t requestRefreshToken) {
    // this actually must be new, if otherwise it will go out of scope
    auto* timeoutOAuth = new QTimer();

    auto* googleOAuth = new QOAuth2AuthorizationCodeFlow(this);
    googleOAuth->setScope("https://mail.google.com/");

    auto* replyHandler = new QOAuthHttpServerReplyHandler(1634, this);

    auto disconnectAll = [googleOAuth, replyHandler, timeoutOAuth]() {
        disconnect(googleOAuth, nullptr, nullptr, nullptr);
        disconnect(replyHandler, nullptr, nullptr, nullptr);
        disconnect(timeoutOAuth, nullptr, nullptr, nullptr);
        delete googleOAuth;
        delete replyHandler;
        delete timeoutOAuth;
    };

    connect(googleOAuth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(googleOAuth, &QOAuth2AuthorizationCodeFlow::granted, [this, requestAuthToken, requestRefreshToken, googleOAuth, disconnectAll]() {
        emit onResponse(requestAuthToken, new std::string(googleOAuth->token().toStdString()));
        emit onResponse(requestRefreshToken, new std::string(googleOAuth->refreshToken().toStdString()));
        disconnectAll();
    });

    const QString CLIENT_ID = "358961584307-33g6eh5pqrr5t7snk8ta9qqejmtmt8kp.apps.googleusercontent.com";
    const QString CLIENT_SECRET = "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";
    googleOAuth->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/auth"));
    googleOAuth->setClientIdentifier(CLIENT_ID);
    googleOAuth->setAccessTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    googleOAuth->setClientIdentifierSharedKey(CLIENT_SECRET);
    googleOAuth->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* parameters) {
        // Percent-decode the "code" parameter so Google can match it
        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
            auto codes = parameters->values("code");
            for (const auto& code : codes) {
                if (code.userType() == QMetaType::QByteArray) {
                    parameters->remove("code", code);
                    parameters->insert("code", QUrl::fromPercentEncoding(code.toByteArray()));
                }
            }
        }
    });

    googleOAuth->setReplyHandler(replyHandler);
    googleOAuth->grant();

    timeoutOAuth->setSingleShot(true);
    connect(timeoutOAuth, &QTimer::timeout, [this, requestAuthToken, requestRefreshToken, disconnectAll](){
        emit onResponse(requestAuthToken, nullptr);
        emit onResponse(requestRefreshToken, nullptr);

        disconnectAll();
    });
    // 5 minute time to authenticate
    timeoutOAuth->start(300000);
}

void QRequest::gmailGetUnparsedMails(
    std::size_t request,
    const std::string& user,
    const std::string& oauthBearer
) {
    auto* response = Request::authIMAP(Constants::GMAIL_IMAP, user, oauthBearer);
    if (response == nullptr) {
        emit onResponse(request, nullptr);
        return;
    }
    delete response;
    response = Request::imap(Constants::GMAIL_IMAP, user, "SELECT \"[Gmail]/All Mail\"");
    if (response == nullptr) {
        emit onResponse(request, nullptr);
        return;
    }
    delete response;
    emit onResponse(request, Request::imap(Constants::GMAIL_IMAP, user, "FETCH 1:* (INTERNALDATE X-GM-THRID BODY.PEEK[HEADER.FIELDS (FROM TO SUBJECT)])"));
}

const char BASE16[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
std::string toBase16(std::size_t num) {
    std::string constructed;
    constructed.reserve(16);
    for (int i = 15; i >= 0; --i) {
        constructed += BASE16[(num >> (4 * i)) & 0b1111];
    }
    return constructed;
}

const std::string_view UTF8_QUOTED_START_SENTINEL = "=?UTF-8?Q?"sv;
const std::string_view UTF8_BASE64_START_SENTINEL = "=?UTF-8?B?"sv;
const std::string_view QUOTE_STRING_END_SENTINEL = "?="sv;
const std::unordered_map<char, char> HEX_MAP = {
    { '0', '\x00' },
    { '1', '\x01' },
    { '2', '\x02' },
    { '3', '\x03' },
    { '4', '\x04' },
    { '5', '\x05' },
    { '6', '\x06' },
    { '7', '\x07' },
    { '8', '\x08' },
    { '9', '\x09' },
    { 'a', '\x0a' },
    { 'b', '\x0b' },
    { 'c', '\x0c' },
    { 'd', '\x0d' },
    { 'e', '\x0e' },
    { 'f', '\x0f' },
    { 'A', '\x0a' },
    { 'B', '\x0b' },
    { 'C', '\x0c' },
    { 'D', '\x0d' },
    { 'E', '\x0e' },
    { 'F', '\x0f' }
};

std::string fixQuotedString(std::string_view quotedString) {
    bool isReadingUTFQuote = false;

    std::string constructedString = "";
    for (std::size_t i = 0; i < quotedString.size(); ++i) {
        if (quotedString.substr(i, UTF8_QUOTED_START_SENTINEL.size()) == UTF8_QUOTED_START_SENTINEL && quotedString.find(QUOTE_STRING_END_SENTINEL, i) != std::string::npos) {
            isReadingUTFQuote = true;
            i += UTF8_QUOTED_START_SENTINEL.size() - 1;
            continue;
        } else if (quotedString.substr(i, UTF8_BASE64_START_SENTINEL.size()) == UTF8_BASE64_START_SENTINEL && quotedString.find(QUOTE_STRING_END_SENTINEL, i) != std::string::npos) {
            i += UTF8_BASE64_START_SENTINEL.size();
            std::size_t end = quotedString.find(QUOTE_STRING_END_SENTINEL, i);
            constructedString += base64::from_base64(quotedString.substr(i, end - i));
            i = end + 2;
        }
        if (isReadingUTFQuote) {
            if (quotedString.substr(i, QUOTE_STRING_END_SENTINEL.size()) == QUOTE_STRING_END_SENTINEL) {
                isReadingUTFQuote = false;
                // it is not a bug that this does not decrement, they sometimes have spaces in between that are not intended
                i += QUOTE_STRING_END_SENTINEL.size();
                continue;
            }
            if (quotedString[i] == '_') {
                constructedString += ' ';
                continue;
            } else if (quotedString[i] == '=') {
                char constructedChar = '\0';
                constructedChar += HEX_MAP.at(quotedString[i + 1]) << 4;
                constructedChar += HEX_MAP.at(quotedString[i + 2]);
                constructedString += constructedChar;
                i += 2;
                continue;
            }
        }

        constructedString += quotedString[i];
    }
    return constructedString;
}

const std::unordered_map<std::string, int> MONTH_MAP = {
    {"Jan", 0},
    {"Feb", 1},
    {"Mar", 2},
    {"Apr", 3},
    {"May", 4},
    {"Jun", 5},
    {"Jul", 6},
    {"Aug", 7},
    {"Sep", 8},
    {"Oct", 9},
    {"Nov", 10},
    {"Dec", 11}
};
std::chrono::high_resolution_clock::time_point toTimePoint(const std::string& dateString) {
    struct tm tm = { 0 };
    std::from_chars(dateString.data(), dateString.data() + 2, tm.tm_mday);
    tm.tm_mon = MONTH_MAP.at(dateString.substr(3, 3));
    std::from_chars(dateString.data() + 7, dateString.data() + 11, tm.tm_year);
    tm.tm_year -= 1900;
    std::from_chars(dateString.data() + 12, dateString.data() + 14, tm.tm_hour);
    std::from_chars(dateString.data() + 15, dateString.data() + 17, tm.tm_min);
    std::from_chars(dateString.data() + 18, dateString.data() + 20, tm.tm_sec);
    return std::chrono::high_resolution_clock::from_time_t(mktime(&tm));
}

const std::string_view GMAIL_THREAD_ID_SENTINEL = "X-GM-THRID "sv;
const std::string_view INTERNAL_DATE_SENTINEL = "INTERNALDATE \""sv;
const std::string_view TO_SENTINEL = "To: "sv;
const std::string_view FROM_SENTINEL = "From: "sv;
const std::string_view SUBJECT_SENTINEL = "Subject: ";
const std::string_view CONTINUE_HEADER_SENTINEL = "\r\n ";
const std::string_view END_HEADER_SENTINEL = "\r\n";
const std::string_view FINISHED_SENTINEL = "\r\n)\r\n";

std::size_t INTERNAL_DATE_SIZE = sizeof("14-Apr-2023 19:19:05 +0000") - 1;
std::map<Inbox::Info, std::string> QRequest::gmailParseMails(std::string_view emailsString, const std::string& user) {
    std::map<Inbox::Info, std::string> allMails;
    std::size_t gmailThreadId = 0;
    std::string dateString;
    std::string toString;
    std::string fromString;
    std::string subjectString;

    for (std::size_t i = 0; i < emailsString.size(); ++i) {
        if (gmailThreadId == 0 && emailsString.substr(i, GMAIL_THREAD_ID_SENTINEL.size()) == GMAIL_THREAD_ID_SENTINEL) {
            i += GMAIL_THREAD_ID_SENTINEL.size();
            std::size_t gmailThreadIdEnd = emailsString.find(' ', i);
            std::from_chars(emailsString.data() + i, emailsString.data() + gmailThreadIdEnd, gmailThreadId);
            i = gmailThreadIdEnd;
            continue;
        }
        if (gmailThreadId != 0 && dateString == "" && emailsString.substr(i, INTERNAL_DATE_SENTINEL.size()) == INTERNAL_DATE_SENTINEL) {
            i += INTERNAL_DATE_SENTINEL.size();
            dateString = std::string(emailsString.substr(i, INTERNAL_DATE_SIZE));
            i = emailsString.find('\n', i);
            continue;
        }
        if (dateString == "") {
            continue;
        }

        std::size_t headerFieldEnd = emailsString.find(' ', i);
        std::size_t finishedPos = emailsString.find(FINISHED_SENTINEL, i);
        if (finishedPos < headerFieldEnd) {
            allMails[Inbox::Info {
                .time = toTimePoint(dateString),
                .uniqueId = gmailThreadId,
                .from = fixQuotedString(fromString),
                .to = fixQuotedString(toString),
                .subject = fixQuotedString(subjectString)
            }] = std::string("https://mail.google.com/mail/u/") + user + std::string("/#inbox/") + toBase16(gmailThreadId);

            i = finishedPos + FINISHED_SENTINEL.size();
            gmailThreadId = 0;
            dateString = "";

            continue;
        }

        std::string_view headerField = emailsString.substr(i, headerFieldEnd - i + 1);
        i = headerFieldEnd + 1;
        std::string headerValue;
        std::size_t continueHeaderPos = emailsString.find(CONTINUE_HEADER_SENTINEL, i);
        std::size_t endHeaderPos = emailsString.find(END_HEADER_SENTINEL, i);
        while (continueHeaderPos == endHeaderPos) {
            headerValue += std::string(emailsString.substr(i, endHeaderPos - i));
            i = continueHeaderPos + 2;
            continueHeaderPos = emailsString.find(CONTINUE_HEADER_SENTINEL, i);
            endHeaderPos = emailsString.find(END_HEADER_SENTINEL, i);
        }
        headerValue += std::string(emailsString.substr(i, endHeaderPos - i));
        i = endHeaderPos + 1;

        if (headerField == TO_SENTINEL) {
            toString = std::move(headerValue);
        } else if (headerField == FROM_SENTINEL) {
            fromString = std::move(headerValue);
        } else if (headerField == SUBJECT_SENTINEL) {
            subjectString = std::move(headerValue);
        } else {
            throw std::logic_error("not a header found");
        }
    }

    return allMails;
}