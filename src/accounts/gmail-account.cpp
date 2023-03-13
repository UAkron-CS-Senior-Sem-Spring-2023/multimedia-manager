#include "gmail-account.hpp"

#include "gmail-account-with-recipients.hpp"
#include "account-manager.hpp"

GmailInbox::GmailInbox(GmailAccount* account)
    : account(account)
{}

GmailInbox::iterator::iterator(typename std::vector<Request::MimeData>::const_iterator messageIterator)
    : messageIterator(messageIterator)
{}

Inbox::iterator& GmailInbox::iterator::operator++() {
    ++messageIterator;
    return *this;
}
std::unique_ptr<Inbox::iterator> GmailInbox::iterator::operator++(int) {
    auto iterator = std::unique_ptr<Inbox::iterator>(new GmailInbox::iterator(messageIterator));
    ++messageIterator;
    return std::move(iterator);
}

typename Inbox::iterator::const_reference GmailInbox::iterator::operator*() const {
    return *messageIterator;
}

bool GmailInbox::iterator::operator==(const Inbox::iterator& other) const {
    return messageIterator == dynamic_cast<const GmailInbox::iterator&>(other).messageIterator;
}
bool GmailInbox::iterator::operator!=(const Inbox::iterator& other) const {
    return messageIterator != dynamic_cast<const GmailInbox::iterator&>(other).messageIterator;
}

std::unique_ptr<Inbox::iterator> GmailInbox::begin() const {
    return std::unique_ptr<Inbox::iterator>(new GmailInbox::iterator(messages.begin()));
}
std::unique_ptr<Inbox::iterator> GmailInbox::end() const {
    return std::unique_ptr<Inbox::iterator>(new GmailInbox::iterator(messages.end()));
}

void GmailInbox::populate() {

}

GmailAccount::GmailAccount(std::string gmail, std::string oauthBearer, QWidget* parent)
    : name_(gmail), gmail_(gmail), oauthBearer_(oauthBearer), inbox_(GmailInbox(this)), QObject(parent),
    infoLayout(&infoWidget),
    gmailInfoHeader("Gmail Information:"),
    gmailCloneWithRecipientsButton("Create account from this with recipients")
{
    int workingRow = 0;
    gmailGmailLabel.setText(QString::fromStdString(std::string("Gmail: ") + gmail_));
    infoLayout.addWidget(&gmailInfoHeader, workingRow++, 0);
    infoLayout.addWidget(&gmailGmailLabel, workingRow++, 0);
    connect(&gmailCloneWithRecipientsButton, &QPushButton::clicked, [this](){
        connect(&getEmailRecipients, &GetEmailRecipientsWizard::gotEmailRecipients, [this](std::list<std::string> recipients){
            if (recipients.size() != 0) {
                AccountManager::singleton().addDualAccount(std::make_unique<GmailAccountWithRecipients>(*this, std::move(recipients)));
            }
            disconnect(&getEmailRecipients, &GetEmailRecipientsWizard::gotEmailRecipients, nullptr, nullptr);
        });
        getEmailRecipients.show();
    });
    infoLayout.addWidget(&gmailCloneWithRecipientsButton, workingRow++, 0);
    infoLayout.setRowStretch(workingRow, 1);
}

const std::string& GmailAccount::name() const {
    return name_;
}

QWidget& GmailAccount::info() const {
    return infoWidget;
};

const std::string& GmailAccount::gmail() const {
    return gmail_;
}

const std::string& GmailAccount::oauthBearer() const {
    return oauthBearer_;
}

const GmailInbox& GmailAccount::inbox() const {
    return inbox_;
}