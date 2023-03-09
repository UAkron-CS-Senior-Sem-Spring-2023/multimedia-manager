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

class SourceAccount : public Account, public MessagingSource {};
class RecipientAccount : public Account, public MessagingRecipient {};
class DualAccount : public Account, public MessagingSource, public MessagingRecipient {};