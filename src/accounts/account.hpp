#pragma once

#include <vector>

#include <QLabel>

#include "messaging-source.hpp"
#include "messaging-recipient.hpp"

class Account {
    public:
        class Info {
            public:
                virtual ~Info() = default;

                virtual const std::vector<QLabel>& labels() const;
            private:
                static std::vector<QLabel> NO_LABELS;
        };

        virtual ~Account() = default;

        virtual const std::string& name() const = 0;
        virtual const Info& info() const = 0;
};

// accounts with sending capabilities
class SourceAccount : public virtual Account, public MessagingSource {};
// accounts that can have inboxes
class RecipientAccount : public virtual Account, public MessagingRecipient {};
// accounts with sending capabilities and inboxes
class DualAccount : public SourceAccount, public RecipientAccount {};