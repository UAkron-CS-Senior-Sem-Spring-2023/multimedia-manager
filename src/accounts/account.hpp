#pragma once

#include "messaging-source.hpp"
#include "messaging-recipient.hpp"

class Account {
    class Info {
        public:
            virtual ~Info() = default;
    };
    
    public:
        virtual ~Account() = default;

        virtual const std::string& name() const;
        virtual const Info& info() const;
};

// accounts with sending capabilities
class SourceAccount : public virtual Account, public MessagingSource {};
// accounts that can have inboxes
class RecipientAccount : public virtual Account, public MessagingRecipient {};
// accounts with sending capabilities and inboxes
class DualAccount : public SourceAccount, public RecipientAccount {};