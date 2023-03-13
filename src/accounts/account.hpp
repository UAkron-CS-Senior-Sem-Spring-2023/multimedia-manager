#pragma once

#include <vector>

#include <QLabel>

#include "messaging-source.hpp"
#include "messaging-recipient.hpp"

class Account {
    public:
        virtual ~Account() = default;

        virtual const std::string& name() const = 0;
        virtual QWidget& info() const = 0;
};

// accounts with sending capabilities
class SourceAccount : public virtual Account, public MessagingSource {};
// accounts that can have inboxes
class RecipientAccount : public virtual Account, public MessagingRecipient {};
// accounts with sending capabilities and inboxes
class DualAccount : public SourceAccount, public RecipientAccount {};