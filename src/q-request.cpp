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
void QRequest::postJson(std::size_t request, const std::string& url, const json& postFields) {
    emit onResponse(request, Request::postJson(url, postFields));
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

void QRequest::refreshAuthToken(
    std::size_t request,
    const std::string& authorizationUrl,
    const std::string& clientId,
    const std::string& clientSecret,
    const std::string& refreshToken
) {
    emit onResponse(request, Request::post(authorizationUrl, {
        {"client_id", clientId},
        {"client_secret", clientSecret},
        {"refresh_token", refreshToken}
    }));
}

void QRequest::oauth2(
    std::size_t requestAuthToken,
    std::size_t requestRefreshToken,
    const std::string& scope,
    const std::string& authorizationUrl,
    const std::string& accessTokenUrl,
    const std::string& clientId,
    const std::string& clientSecret
) {
    // this actually must be new, if otherwise it will go out of scope
    auto* timeoutOAuth = new QTimer();

    auto* oauth = new QOAuth2AuthorizationCodeFlow(this);
    oauth->setScope(QString::fromStdString(scope));

    auto* replyHandler = new QOAuthHttpServerReplyHandler(1634, this);

    auto disconnectAll = [oauth, replyHandler, timeoutOAuth]() {
        disconnect(oauth, nullptr, nullptr, nullptr);
        disconnect(replyHandler, nullptr, nullptr, nullptr);
        disconnect(timeoutOAuth, nullptr, nullptr, nullptr);
        delete oauth;
        delete replyHandler;
        delete timeoutOAuth;
    };

    connect(oauth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(oauth, &QOAuth2AuthorizationCodeFlow::granted, [this, requestAuthToken, requestRefreshToken, oauth, disconnectAll]() {
        emit onResponse(requestAuthToken, new std::string(oauth->token().toStdString()));
        emit onResponse(requestRefreshToken, new std::string(oauth->refreshToken().toStdString()));
        disconnectAll();
    });

    oauth->setAuthorizationUrl(QUrl(QString::fromStdString(authorizationUrl)));
    oauth->setAccessTokenUrl(QUrl(QString::fromStdString(accessTokenUrl)));
    oauth->setClientIdentifier(QString::fromStdString(clientId));
    if (clientSecret != "") {
        oauth->setClientIdentifierSharedKey(QString::fromStdString(clientSecret));
    }
    oauth->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* parameters) {
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

    oauth->setReplyHandler(replyHandler);
    oauth->grant();

    timeoutOAuth->setSingleShot(true);
    connect(timeoutOAuth, &QTimer::timeout, [this, requestAuthToken, requestRefreshToken, disconnectAll](){
        emit onResponse(requestAuthToken, nullptr);
        emit onResponse(requestRefreshToken, nullptr);

        disconnectAll();
    });
    // 5 minute time to authenticate
    timeoutOAuth->start(300000);
}

void QRequest::gmailOAuth(std::size_t requestAuthToken, std::size_t requestRefreshToken) {
    oauth2(
        requestAuthToken,
        requestRefreshToken,
        "https://mail.google.com/",
        "https://accounts.google.com/o/oauth2/auth",
        "https://oauth2.googleapis.com/token",
        "358961584307-33g6eh5pqrr5t7snk8ta9qqejmtmt8kp.apps.googleusercontent.com", // client id
        "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" // client secret (it's not actually a secret for desktop applications)
    );
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
std::chrono::high_resolution_clock::time_point gmailDateToTimePoint(const std::string& dateString) {
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
                .time = gmailDateToTimePoint(dateString),
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

void QRequest::outlookOAuth(std::size_t requestAuthToken, std::size_t requestRefreshToken) {
    oauth2(
        requestAuthToken,
        requestRefreshToken,
        "https://graph.microsoft.com/User.Read https://graph.microsoft.com/Mail.Send https://graph.microsoft.com/Mail.Read",
        "https://login.microsoftonline.com/common/oauth2/v2.0/authorize",
        "https://login.microsoftonline.com/common/oauth2/v2.0/token",
        "4c15dfee-a228-4233-930e-c7fcedc94769", // client id
        "" // no client secret for outlook
    );    	
}

void QRequest::outlookGetUser(std::size_t request, const std::string& oauthBearer) {
    std::string* response = Request::authGet("https://graph.microsoft.com/v1.0/me", oauthBearer);
    if (response == nullptr) {
        emit onResponse(request, nullptr);
        return;
    }

    json jsonResponse = json::parse(*response);
    delete response;
    if (!jsonResponse.contains("userPrincipalName")) {
        emit onResponse(request, nullptr);
        return;
    }

    emit onResponse(request, new std::string(jsonResponse.at("userPrincipalName")));
}


void QRequest::outlookGetUnparsedMails(std::size_t request, const std::string& oauthBearer) {
    emit onResponse(request, Request::authGet("https://graph.microsoft.com/v1.0/me/messages", oauthBearer));
}

// 2023-04-17T20:55:35Z
std::chrono::high_resolution_clock::time_point outlookDateToTimePoint(const std::string& dateString) {
    struct tm tm = { 0 };
    std::from_chars(dateString.data(), dateString.data() + 4, tm.tm_year);
    tm.tm_year -= 1900;
    std::from_chars(dateString.data() + 5, dateString.data() + 7, tm.tm_mon);
    --tm.tm_mon;
    std::from_chars(dateString.data() + 8, dateString.data() + 10, tm.tm_mday);
    std::from_chars(dateString.data() + 11, dateString.data() + 13, tm.tm_hour);
    std::from_chars(dateString.data() + 14, dateString.data() + 16, tm.tm_min);
    std::from_chars(dateString.data() + 17, dateString.data() + 19, tm.tm_sec);
    return std::chrono::high_resolution_clock::from_time_t(mktime(&tm));
}

std::map<Inbox::Info, std::string> QRequest::outlookParseMails(std::string_view emailsString, const std::string& user) {
    std::map<Inbox::Info, std::string> allMails;

    auto hasher = std::hash<std::string>();

    json emailJson = json::parse(emailsString);
    if (!emailJson.contains("value")) {
        return allMails;
    }

    for (json messageJson : emailJson.at("value")) {
        std::string toRecipients;
        for (json toRecipient : messageJson.at("toRecipients")) {
            toRecipients += std::string(toRecipient.at("emailAddress").at("name")) + std::string(" ") + std::string(toRecipient.at("emailAddress").at("address")) + "\n";
        }
        // remove last newline
        toRecipients.resize(toRecipients.size() - 1);

        allMails[Inbox::Info {
            .time = outlookDateToTimePoint(messageJson.at("receivedDateTime")),
            .uniqueId = hasher(messageJson.at("id")),
            .from = std::string(messageJson.at("from").at("emailAddress").at("name")) + std::string(" ") + std::string(messageJson.at("from").at("emailAddress").at("address")),
            .to = toRecipients,
            .subject = messageJson.at("subject")
        }] = messageJson.at("webLink");
    }
    
    return allMails;
}

void QRequest::outlookSendMail(
    std::size_t request,
    const std::string& oauthBearer,
    const Request::SMTPHeaders& headers,
    const Request::MimeData& mimeData
) {
    json messageJson = {
        {"message", {
            {"subject", headers.subject()},
            {"body", {
                {"contentType", "Text"},
                {"content", mimeData.stringRepresentation()}
            }},
            {"toRecipients", {}},
            {"ccRecipients", {}},
        }},
        {"saveToSentItems", "true"}
    };

    for (auto it = headers.toBegin(); it != headers.toEnd(); ++it) {
        messageJson.at("message").at("toRecipients").push_back(json({
            {"emailAddress", {
                {"address", *it}
            }} 
        }));
    }
    if (messageJson.at("message").at("toRecipients").is_null()) {
        messageJson.at("message").erase("toRecipients");
    }
    for (auto it = headers.ccBegin(); it != headers.ccEnd(); ++it) {
        messageJson.at("message").at("ccRecipients").push_back(json({
            {"emailAddress", {
                {"address", *it}
            }} 
        }));
    }
    if (messageJson.at("message").at("ccRecipients").is_null()) {
        messageJson.at("message").erase("ccRecipients");
    }

    emit onResponse(request, Request::authPostJson("https://graph.microsoft.com/v1.0/me/sendMail", oauthBearer, messageJson));
}