#include "google-gateway.hpp"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>

#include "request.hpp"

const QString CLIENT_ID = "358961584307-33g6eh5pqrr5t7snk8ta9qqejmtmt8kp.apps.googleusercontent.com";
const QString CLIENT_SECRET = "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";

GoogleGateway::GoogleGateway(QObject *parent) : QObject(parent)
{
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("https://www.googleapis.com/auth/gmail.send https://mail.google.com/");

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(this->google, &QOAuth2AuthorizationCodeFlow::granted, [=](){
        authBearer_ = this->google->token().toStdString();

        Request::MimeData md;
        md.addText("a lot of text!!!");
        Request::SMTPHeaders smtpheaders = Request::SMTPHeaders("robertson.gregory.a@gmail.com", { "robertson.gregory.a@gmail.com"}, {}, "testing smtp");
        std::optional<std::string> resultsmtp = Request::smtp("smtp://smtp.gmail.com:587", authBearer_, smtpheaders, md);
    });

    this->google->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/auth"));
    this->google->setClientIdentifier(CLIENT_ID);
    this->google->setAccessTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    this->google->setClientIdentifierSharedKey(CLIENT_SECRET);
    google->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* parameters) {
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

    auto replyHandler = new QOAuthHttpServerReplyHandler(20289, this);
    this->google->setReplyHandler(replyHandler);
    this->google->grant();
}

const std::string& GoogleGateway::authBearer() const {
    return authBearer_;
}