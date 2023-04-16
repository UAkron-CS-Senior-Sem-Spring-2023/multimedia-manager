#include "gmail-account.hpp"

#include "gmail-account-with-recipients.hpp"
#include "account-manager.hpp"
#include "q-request.hpp"

GmailInbox::GmailInbox(GmailAccount* account, QObject* parent)
    : QObject(parent), account(account)
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
    return iterator;
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

#include <iostream>

void GmailInbox::populate() const {
    std::size_t authRequest = QRequest::singleton().getUniqueRequest();
    std::size_t listRequest = QRequest::singleton().getUniqueRequest();
    auto authConnection = std::make_unique<QMetaObject::Connection>(); 
    *authConnection = connect(&QRequest::singleton(), &QRequest::onResponse, this, [this, authRequest, &authConnection, listRequest](std::size_t requestProvided, std::string* response) {
        if (requestProvided != authRequest) {
            return;
        }
        if (response == nullptr) {
            std::cout << "Warning: got response nullptr from GmailInbox population" << std::endl;
            return;
        }
        std::cout << *response << std::endl;
        delete response;
        QRequest::singleton().imap(listRequest, QRequest::Constants::GMAIL_IMAP, account->gmail(), "LIST \"\" %");

        disconnect(*authConnection);
    });

    auto listConnection = std::make_unique<QMetaObject::Connection>();
    *listConnection = connect(&QRequest::singleton(), &QRequest::onResponse, this, [this, listRequest, &listConnection](std::size_t requestProvided, std::string* response) {
        if (requestProvided != listRequest) {
            return;
        }
        if (response == nullptr) {
            std::cout << "Warning: got response nullptr from GmailInbox population" << std::endl;
            return;
        }
        std::cout << *response << std::endl;
        delete response;

        disconnect(*listConnection);
    });
    QRequest::singleton().authIMAP(authRequest, QRequest::Constants::GMAIL_IMAP, account->gmail(), account->oauthBearer());
}

GmailAccount::GmailAccount(std::string gmail, std::string oauthBearer, QObject* parent)
    : QObject(parent), name_(gmail), gmail_(gmail), oauthBearer_(oauthBearer), inbox_(GmailInbox(this))
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

const GmailInbox& GmailAccount::inbox() const {
    return inbox_;
}