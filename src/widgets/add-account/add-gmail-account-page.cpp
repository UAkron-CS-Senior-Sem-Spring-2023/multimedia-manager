#include "add-gmail-account-page.hpp"

#include "add-account-wizard.hpp"
#include "q-request.hpp"

#include "accounts/gmail-account.hpp"


AddGmailAccountPage::AddGmailAccountPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : ValidatableAddAccountPage(parent),
    addAccountWizard(addAccountWizard),
    gmailAccountInformation(tr("Enter the information for the gmail account you want to add.")),
    gmailEntryLabel(tr("Your email address will be automatically entered into this field upon authenticating.")),
    gmailAuthenticationPrompt(tr("Click the button below to authenticate in browser. You will need to finish within "
                                 "5 minutes or the authentication will time out. After authenticating this wizard will close and the account will be added.")),
    gmailAuthenticationButton(tr("Authenticate"))
{
    setLayout(&addGmailAccountPageLayout);

    int workingRow = 0;

    gmailAccountInformation.setWordWrap(true);
    addGmailAccountPageLayout.addWidget(&gmailAccountInformation, workingRow++, 0);
    addGmailAccountPageLayout.setRowStretch(workingRow++, 1);
    gmailEntryLabel.setWordWrap(true);
    addGmailAccountPageLayout.addWidget(&gmailEntryLabel, workingRow++, 0);
    gmailEntryTextLine.setReadOnly(true);
    uneditableTextLinePalette.setColor(QPalette::Base, Qt::gray);
    uneditableTextLinePalette.setColor(QPalette::Text, Qt::darkGray);
    addGmailAccountPageLayout.addWidget(&gmailEntryTextLine, workingRow++, 0);
    gmailAuthenticationPrompt.setWordWrap(true);
    addGmailAccountPageLayout.addWidget(&gmailAuthenticationPrompt, workingRow++, 0);

    std::size_t authRequest = QRequest::singleton().getUniqueRequest();
    std::size_t verifyRequest = QRequest::singleton().getUniqueRequest();

    connect(addAccountWizard, &AddAccountWizard::finished, [this](){
        gmailEntryTextLine.setText("");
        verified = false;
    });

    connect(&QRequest::singleton(), &QRequest::onResponse, [this, verifyRequest, addAccountWizard](std::size_t requestProvided, std::string* response) {
        if (verifyRequest != requestProvided || response == nullptr) {
            return;
        }
        gmail = std::string(*response);
        delete response;

        gmailEntryTextLine.setText(QString::fromStdString(gmail));
        addAccountWizard->clearAccountsToAdd();
        addAccountWizard->addSourceAccountToAdd(std::unique_ptr<SourceAccount>(new GmailAccount(gmail, oauthBearer)));

        verified = true;
        emit completeChanged();
    });

    connect(&QRequest::singleton(), &QRequest::onResponse, [this, authRequest, verifyRequest](std::size_t requestProvided, std::string* response) {
        if (authRequest != requestProvided || response == nullptr) {
            return;
        }
        oauthBearer = std::string(*response);
        delete response;

        QRequest::singleton().gmailGetUser(verifyRequest, oauthBearer);
    });

    connect(&gmailAuthenticationButton, &QPushButton::clicked, [this, authRequest](){
        gmail = gmailEntryTextLine.text().toStdString();
        QRequest::singleton().gmailOAuth(authRequest);
    });

    addGmailAccountPageLayout.addWidget(&gmailAuthenticationButton, workingRow++, 0);
}

bool AddGmailAccountPage::isComplete() const {
    return verified;
}