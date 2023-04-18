#pragma once

#include "account.hpp"
#include "outlook-account.hpp"

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class OutlookAccountWithRecipients : public DualAccount, public OutlookAccount {
    public:
        OutlookAccountWithRecipients(const OutlookAccount& outlookAccount, std::list<std::string> recipients, QObject* parent = nullptr);

        const std::string& name() const;

        void send(const std::string& subject, const Request::MimeData& mimeData) const override;
    private:
        std::string name_;

        std::list<std::string> recipients_;
};