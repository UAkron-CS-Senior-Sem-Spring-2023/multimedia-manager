#include "gmail-account-with-recipients.hpp"

GmailAccountWithRecipients::GmailAccountWithRecipients(const GmailAccount& gmailAccount, std::list<std::string> recipients, QObject* parent)
    : GmailAccount(gmailAccount.gmail(), gmailAccount.oauthBearer(), parent), recipients_(recipients)
{
    name_ = gmailAccount.gmail() + " with " + std::to_string(recipients.size()) + " recipients";
}

const std::string& GmailAccountWithRecipients::name() const {
    return name_;
}

void GmailAccountWithRecipients::send(const Request::MimeData& mimeData) const {

}