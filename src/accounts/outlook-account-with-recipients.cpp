#include "outlook-account-with-recipients.hpp"

#include "q-request.hpp"

OutlookAccountWithRecipients::OutlookAccountWithRecipients(const OutlookAccount& outlookAccount, std::list<std::string> recipients, QObject* parent)
    : OutlookAccount(outlookAccount.outlookUsername(), outlookAccount.oauthBearer(), outlookAccount.refreshToken(), parent), recipients_(std::move(recipients))
{
    name_ = outlookAccount.name() + " with " + std::to_string(recipients_.size()) + " recipients";
}

const std::string& OutlookAccountWithRecipients::name() const {
    return name_;
}

void OutlookAccountWithRecipients::send(const std::string& subject, const Request::MimeData& mimeData) const {
    auto headers = Request::SMTPHeaders(outlookUsername(), subject, recipients_);
    QRequest::singleton().outlookSendMail(QRequest::getNonUniqueRequest(), oauthBearer(), headers, mimeData);
}