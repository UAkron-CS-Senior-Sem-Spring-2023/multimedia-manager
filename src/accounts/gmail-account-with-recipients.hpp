#pragma once

#include "account.hpp"
#include "gmail-account.hpp"

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class GmailAccountWithRecipients : public DualAccount, public GmailAccount {
    public:
        GmailAccountWithRecipients(const GmailAccount& gmailAccount, std::list<std::string> recipients, QObject* parent = nullptr);

        const std::string& name() const;

        void send(const Request::MimeData& mimeData) const override;
    private:
        std::string name_;

        std::list<std::string> recipients_;
};