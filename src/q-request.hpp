#pragma once

#include "request.hpp"

#include <QObject>

#include "accounts/inbox.hpp"

class QRequest : public QObject {
    Q_OBJECT

    using SMTPHeaders = Request::SMTPHeaders;
    using MimeData = Request::MimeData;

    public:
        using Constants = Request::Constants;
        static QRequest& singleton();

        std::size_t getUniqueRequest();
        static std::size_t getNonUniqueRequest();

        void get(std::size_t request, const std::string& url);
        void authGet(std::size_t request, const std::string& url, const std::string& oauthBearer);
        void post(std::size_t request, const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
        void smtp(
            std::size_t request,
            const std::string& url,
            const std::string& oauthBearer,
            const SMTPHeaders& headers,
            const MimeData& mimeData
        );
        void imap(std::size_t request, const std::string& url, const std::string& user, const std::string& imapCommands = "CAPABILITY");
        void authIMAP(std::size_t request, const std::string& url, const std::string& user, const std::string& oauthBearer);
        
        void gmailOAuth(std::size_t requestAuthToken, std::size_t requestRefreshToken);

        void gmailGetUser(
            std::size_t request,
            const std::string& oauthBearer
        );
        
        void gmailGetUnparsedMails(
            std::size_t request,
            const std::string& user,
            const std::string& oauthBearer
        );
        static std::map<Inbox::Info, std::string> gmailParseMails(std::string_view emailsString, const std::string& user);
    signals:
        void onResponse(std::size_t request, std::string* response);

    private:
        QRequest(QObject* parent = nullptr);

        std::size_t request_ = 0;
};