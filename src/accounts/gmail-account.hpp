#pragma once

#include "account.hpp"

class GmailAccount;

class GmailInbox : public Inbox {
    public:
        class iterator : public Inbox::iterator {
            public:
                iterator(typename std::vector<Request::MimeData>::const_iterator messageIterator);

                const_reference operator*() const override;
                Inbox::iterator& operator++() override;
                std::unique_ptr<Inbox::iterator> operator++(int) override;
                bool operator==(const Inbox::iterator& other) const override;
                bool operator!=(const Inbox::iterator& other) const override;
            private:
                typename std::vector<Request::MimeData>::const_iterator messageIterator;
        };

        GmailInbox(GmailAccount* account);

        std::unique_ptr<Inbox::iterator> begin() const override;
        std::unique_ptr<Inbox::iterator> end() const override;

        void populate() override;
    private:
        GmailAccount* account;
        std::vector<Request::MimeData> messages;
};

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class GmailAccount : public SourceAccount {
    public:
        GmailAccount(std::string gmail, std::string oauthBearer);

        const std::string& name() const;
        QWidget& info() const;

        const std::string& gmail() const;
        const std::string& oauthBearer() const;

        const GmailInbox& inbox() const;
    private:
        std::string name_;
        std::string gmail_;
        std::string oauthBearer_;

        GmailInbox inbox_;

        mutable QWidget infoWidget;
        QGridLayout infoLayout;
        QLabel gmailInfoHeader;
        QLabel gmailGmailLabel;
        QPushButton gmailCloneWithRecipientsButton;
        
};