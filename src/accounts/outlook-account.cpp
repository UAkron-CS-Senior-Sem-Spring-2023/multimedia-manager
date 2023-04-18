#include "outlook-account.hpp"

#include "outlook-account-with-recipients.hpp"
#include "account-manager.hpp"
#include "q-request.hpp"

OutlookInbox::OutlookInbox(OutlookAccount* account, QObject* parent)
    : Inbox(parent), account(account)
{}

OutlookInbox::iterator::iterator(typename std::map<Info, std::string>::const_iterator messageIterator)
    : messageIterator(messageIterator)
{}

Inbox::iterator& OutlookInbox::iterator::operator++() {
    ++messageIterator;
    return *this;
}
std::unique_ptr<Inbox::iterator> OutlookInbox::iterator::operator++(int) {
    auto iterator = std::unique_ptr<Inbox::iterator>(new OutlookInbox::iterator(messageIterator));
    ++messageIterator;
    return iterator;
}

typename Inbox::iterator::value_type OutlookInbox::iterator::operator*() const {
    return *messageIterator;
}

bool OutlookInbox::iterator::operator==(const Inbox::iterator& other) const {
    return messageIterator == dynamic_cast<const OutlookInbox::iterator&>(other).messageIterator;
}
bool OutlookInbox::iterator::operator!=(const Inbox::iterator& other) const {
    return messageIterator != dynamic_cast<const OutlookInbox::iterator&>(other).messageIterator;
}

std::unique_ptr<Inbox::iterator> OutlookInbox::begin() const {
    return std::unique_ptr<Inbox::iterator>(new OutlookInbox::iterator(messages.begin()));
}
std::unique_ptr<Inbox::iterator> OutlookInbox::end() const {
    return std::unique_ptr<Inbox::iterator>(new OutlookInbox::iterator(messages.end()));
}

void OutlookInbox::populate() const {
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

        messages = QRequest::outlookParseMails(*response, account->outlookUsername());
        emit populatedInbox(this);
        delete response;
    });
    QRequest::singleton().outlookGetUnparsedMails(getEmailsRequest, account->oauthBearer());
}

OutlookAccount::OutlookAccount(std::string outlookUsername, std::string oauthBearer, std::string refreshToken, QObject* parent)
    : QObject(parent), name_(outlookUsername), outlookUsername_(outlookUsername), oauthBearer_(oauthBearer), refreshToken_(refreshToken), inbox_(OutlookInbox(this))
{
    QGridLayout* infoLayout = new QGridLayout(&infoWidget);
    int workingRow = 0;

    infoLayout->addWidget(new QLabel(tr("Outlook Information:")), workingRow++, 0);
    infoLayout->addWidget(new QLabel(QString::fromStdString(std::string("Outlook Username: ") + outlookUsername_)), workingRow++, 0);

    QPushButton* outlookCloneWithRecipientsButton = new QPushButton(tr("Create account from this with recipients"));
    connect(outlookCloneWithRecipientsButton, &QPushButton::clicked, [this]() {
        if (!getEmailRecipientsWizard) {
            getEmailRecipientsWizard = std::make_unique<GetEmailRecipientsWizard>();
        }
        connect(getEmailRecipientsWizard.get(), &GetEmailRecipientsWizard::gotEmailRecipients, [this](std::list<std::string> recipients){
            if (recipients.size() != 0) {
                AccountManager::singleton().addDualAccount(std::make_unique<OutlookAccountWithRecipients>(*this, std::move(recipients)));
            }
            disconnect(getEmailRecipientsWizard.get(), &GetEmailRecipientsWizard::gotEmailRecipients, nullptr, nullptr);
        });
        getEmailRecipientsWizard->show();
    });
    infoLayout->addWidget(outlookCloneWithRecipientsButton, workingRow++, 0);
    infoLayout->setRowStretch(workingRow, 1);
}

const std::string& OutlookAccount::name() const {
    return name_;
}

QWidget& OutlookAccount::info() const {
    return infoWidget;
};

const std::string& OutlookAccount::outlookUsername() const {
    return outlookUsername_;
}

const std::string& OutlookAccount::oauthBearer() const {
    return oauthBearer_;
}

const std::string& OutlookAccount::refreshToken() const {
    return refreshToken_;
}

const OutlookInbox& OutlookAccount::inbox() const {
    return inbox_;
}