#pragma once

#include "request.hpp"

#include <QObject>

class QRequest : public QObject {
    Q_OBJECT

    using SMTPHeaders = Request::SMTPHeaders;
    using MimeData = Request::MimeData;

    public:
        static QRequest& singleton();

        void get(std::size_t request, const std::string& url);
        void post(std::size_t request, const std::string& url, const std::unordered_map<std::string, std::string>& postFields);
        void smtp(
            std::size_t request,
            const std::string& url,
            const std::string& oauthBearer,
            const SMTPHeaders& headers,
            const MimeData& mimeData
        );
        void imap(std::size_t request);
        
        void gmailSMTP(
            std::size_t request,
            const SMTPHeaders& headers,
            const MimeData& mimeData
        );
    signals:
        void onResponse(std::size_t request, std::optional<std::string> response);

    private:
        QRequest(QObject* parent = nullptr);
};