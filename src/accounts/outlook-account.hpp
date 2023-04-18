#pragma once

#include "account.hpp"

class OutlookAccount;

class OutlookInbox : public Inbox {
    public:
        class iterator : public Inbox::iterator {
            public:
                iterator(typename std::map<Info, std::string>::const_iterator messageIterator);

                value_type operator*() const override;
                Inbox::iterator& operator++() override;
                std::unique_ptr<Inbox::iterator> operator++(int) override;
                bool operator==(const Inbox::iterator& other) const override;
                bool operator!=(const Inbox::iterator& other) const override;
            private:
                typename std::map<Inbox::Info, std::string>::const_iterator messageIterator;
        };

        OutlookInbox(OutlookAccount* account, QObject* parent = nullptr);

        std::unique_ptr<Inbox::iterator> begin() const override;
        std::unique_ptr<Inbox::iterator> end() const override;

        void populate() const override;
    private:
        OutlookAccount* account;
        mutable std::map<Inbox::Info, std::string> messages;
};

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "widgets/get-email-recipients-wizard.hpp"

class OutlookAccount : public virtual SourceAccount, public QObject {
    public:
        OutlookAccount(std::string outlookUsername, std::string oauthBearer, std::string refreshToken, QObject* parent = nullptr);

        const std::string& name() const override;
        QWidget& info() const override;
        const OutlookInbox& inbox() const override;

        const std::string& outlookUsername() const;
        const std::string& oauthBearer() const;
        const std::string& refreshToken() const;

    private:
        std::string name_;
        std::string outlookUsername_;
        std::string oauthBearer_;
        std::string refreshToken_;

        OutlookInbox inbox_;

        mutable QWidget infoWidget;

        std::unique_ptr<GetEmailRecipientsWizard> getEmailRecipientsWizard;
};