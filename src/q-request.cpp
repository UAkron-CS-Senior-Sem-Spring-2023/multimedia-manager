#include "q-request.hpp"

#include "request.hpp"

#include <limits>
#include <memory>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QTimer>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
    if (!jsonResponse.contains("emailAddress")) {
        emit onResponse(request, nullptr);
    }
    emit onResponse(request, new std::string(jsonResponse.at("emailAddress")));
}

void QRequest::gmailOAuth(std::size_t request) {
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
    connect(googleOAuth, &QOAuth2AuthorizationCodeFlow::granted, [this, request, googleOAuth, disconnectAll]() {
        std::string* oauthToken = new std::string(googleOAuth->token().toStdString());
        emit onResponse(request, oauthToken);

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
    connect(timeoutOAuth, &QTimer::timeout, [this, request, disconnectAll](){
        emit onResponse(request, nullptr);

        disconnectAll();
    });
    // 5 minute time to authenticate
    timeoutOAuth->start(300000);
}