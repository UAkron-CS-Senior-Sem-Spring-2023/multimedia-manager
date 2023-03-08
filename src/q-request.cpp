#include "q-request.hpp"

#include "request.hpp"

#include <memory>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QTimer>

std::unique_ptr<QRequest> singleton_;
QRequest& QRequest::singleton() {
    if (!singleton_) {
        singleton_ = std::unique_ptr<QRequest>(new QRequest);
    }

    return *singleton_;
}

QRequest::QRequest(QObject* parent)
    : QObject(parent)
{}

void QRequest::get(std::size_t request, const std::string& url) {
    emit onResponse(request, std::move(Request::get(url)));
}

void QRequest::post(std::size_t request, const std::string& url, const std::unordered_map<std::string, std::string>& postFields) {
    emit onResponse(request, std::move(Request::post(url, postFields)));
}

void QRequest::smtp(
    std::size_t request,
    const std::string& url,
    const std::string& oauthBearer,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    emit onResponse(request, std::move(Request::smtp(url, oauthBearer, headers, mimeData)));
}

void QRequest::gmailSMTP(
    std::size_t request,
    const SMTPHeaders& headers,
    const MimeData& mimeData
) {
    // this actually must be new, if otherwise it will go out of scope
    auto googleOAuth = new QOAuth2AuthorizationCodeFlow(this);
    googleOAuth->setScope("https://mail.google.com/");

    auto replyHandler = new QOAuthHttpServerReplyHandler(20289, this);
    connect(googleOAuth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(googleOAuth, &QOAuth2AuthorizationCodeFlow::granted, [this, &headers, &mimeData, request, googleOAuth, replyHandler](){
        auto authBearer_ = googleOAuth->token().toStdString();
        emit onResponse(request, std::move(Request::smtp("smtp://smtp.gmail.com:587", authBearer_, headers, mimeData)));
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

    auto* timeoutOAuth = new QTimer();
    timeoutOAuth->setSingleShot(true);
    connect(timeoutOAuth, &QTimer::timeout, [googleOAuth, replyHandler, timeoutOAuth](){
        disconnect(googleOAuth, nullptr, nullptr, nullptr);
        disconnect(replyHandler, nullptr, nullptr, nullptr);
        disconnect(timeoutOAuth, nullptr, nullptr, nullptr);
        delete googleOAuth;
        delete replyHandler;
        delete timeoutOAuth;
    });
    // 5 minute time to authenticate
    timeoutOAuth->start(300000);
}
