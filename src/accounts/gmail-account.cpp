#include "gmail-account.hpp"

#include "gmail-account-with-recipients.hpp"
#include "account-manager.hpp"
#include "q-request.hpp"

GmailInbox::GmailInbox(GmailAccount* account, QObject* parent)
    : Inbox(parent), account(account)
{}

GmailInbox::iterator::iterator(typename std::map<Info, std::string>::const_iterator messageIterator)
    : messageIterator(messageIterator)
{}

Inbox::iterator& GmailInbox::iterator::operator++() {
    ++messageIterator;
    return *this;
}
std::unique_ptr<Inbox::iterator> GmailInbox::iterator::operator++(int) {
    auto iterator = std::unique_ptr<Inbox::iterator>(new GmailInbox::iterator(messageIterator));
    ++messageIterator;
    return iterator;
}

typename Inbox::iterator::value_type GmailInbox::iterator::operator*() const {
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

void GmailInbox::populate() const {
    std::size_t getEmailsRequest = QRequest::singleton().getUniqueRequest();

    auto* getEmailsConnection = new QMetaObject::Connection;
    *getEmailsConnection = connect(&QRequest::singleton(), &QRequest::onResponse, [this, getEmailsRequest, getEmailsConnection](std::size_t requestProvided, std::string* response) {
        if (requestProvided != getEmailsRequest) {
            return;
        }
        disconnect(*getEmailsConnection);
        delete getEmailsConnection;
        if (response == nullptr) {
            return;
        }

        messages = QRequest::gmailParseMails(*response, account->gmail());
        emit populatedInbox(this);
        delete response;
    });
    QRequest::singleton().gmailGetUnparsedMails(getEmailsRequest, account->gmail(), account->oauthBearer());
}

GmailAccount::GmailAccount(std::string gmail, std::string oauthBearer, std::string refreshToken, QObject* parent)
    : QObject(parent), name_(gmail), gmail_(gmail), oauthBearer_(oauthBearer), refreshToken_(refreshToken), inbox_(GmailInbox(this))
{
    QGridLayout* infoLayout = new QGridLayout(&infoWidget);
    int workingRow = 0;

    infoLayout->addWidget(new QLabel(tr("Gmail Information:")), workingRow++, 0);
    infoLayout->addWidget(new QLabel(QString::fromStdString(std::string("Gmail: ") + gmail_)), workingRow++, 0);

    QPushButton* gmailCloneWithRecipientsButton = new QPushButton(tr("Create account from this with recipients"));
    connect(gmailCloneWithRecipientsButton, &QPushButton::clicked, [this]() {
        if (!getEmailRecipientsWizard) {
            getEmailRecipientsWizard = std::make_unique<GetEmailRecipientsWizard>();
        }
        connect(getEmailRecipientsWizard.get(), &GetEmailRecipientsWizard::gotEmailRecipients, [this](std::list<std::string> recipients){
            if (recipients.size() != 0) {
                AccountManager::singleton().addDualAccount(std::make_unique<GmailAccountWithRecipients>(*this, std::move(recipients)));
            }
            disconnect(getEmailRecipientsWizard.get(), &GetEmailRecipientsWizard::gotEmailRecipients, nullptr, nullptr);
        });
        getEmailRecipientsWizard->show();
    });
    infoLayout->addWidget(gmailCloneWithRecipientsButton, workingRow++, 0);
    infoLayout->setRowStretch(workingRow, 1);
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

const std::string& GmailAccount::refreshToken() const {
    return refreshToken_;
}

const GmailInbox& GmailAccount::inbox() const {
    return inbox_;
}