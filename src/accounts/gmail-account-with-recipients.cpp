#include "gmail-account-with-recipients.hpp"

#include "request.hpp"

GmailAccountWithRecipients::GmailAccountWithRecipients(const GmailAccount& gmailAccount, std::list<std::string> recipients, QObject* parent)
    : GmailAccount(gmailAccount.gmail(), gmailAccount.oauthBearer(), parent), recipients_(std::move(recipients))
{
    name_ = gmailAccount.gmail() + " with " + std::to_string(recipients_.size()) + " recipients";
}

const std::string& GmailAccountWithRecipients::name() const {
    return name_;
}

void GmailAccountWithRecipients::send(const std::string& subject, const Request::MimeData& mimeData) const {
    auto headers = Request::SMTPHeaders(gmail(), subject, recipients_);
    Request::smtp("smtp://smtp.gmail.com:587", oauthBearer(), headers, mimeData);
}