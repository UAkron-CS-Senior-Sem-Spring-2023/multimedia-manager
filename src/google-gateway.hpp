#pragma once

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QNetworkReply>

class GoogleGateway : public QObject
{
public:
    GoogleGateway(QObject *parent = nullptr);
    const std::string& authBearer() const;

private:
    QOAuth2AuthorizationCodeFlow * google;

    std::string authBearer_;
}; 
