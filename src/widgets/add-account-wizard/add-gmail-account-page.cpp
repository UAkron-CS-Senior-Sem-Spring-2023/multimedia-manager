#include "add-gmail-account-page.hpp"

#include "add-account-wizard.hpp"
#include "q-request.hpp"

#include "accounts/gmail-account.hpp"

#include <iostream>

AddGmailAccountPage::AddGmailAccountPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : ValidatableAddAccountPage(parent),
    addAccountWizard(addAccountWizard)
{
    auto* addGmailAccountPageLayout = new QGridLayout;
    setLayout(addGmailAccountPageLayout);

    int workingRow = 0;


    auto* gmailAccountInformation = new QLabel(tr("Enter the information for the gmail account you want to add."));
    gmailAccountInformation->setWordWrap(true);
    addGmailAccountPageLayout->addWidget(gmailAccountInformation, workingRow++, 0);
    addGmailAccountPageLayout->setRowStretch(workingRow++, 1);
    auto* gmailEntryLabel = new QLabel(tr("Your email address will be automatically entered into this field upon authenticating."));
    gmailEntryLabel->setWordWrap(true);
    addGmailAccountPageLayout->addWidget(gmailEntryLabel, workingRow++, 0);
    
    auto* gmailEntryTextLine = new QLineEdit;
    gmailEntryTextLine->setReadOnly(true);
    QPalette uneditableTextLinePalette;
    uneditableTextLinePalette.setColor(QPalette::Base, Qt::black);
    uneditableTextLinePalette.setColor(QPalette::Text, Qt::darkGray);
    gmailEntryTextLine->setPalette(uneditableTextLinePalette);
    addGmailAccountPageLayout->addWidget(gmailEntryTextLine, workingRow++, 0);
    auto* gmailAuthenticationPrompt = new QLabel(tr("Click the button below to authenticate in browser. You will need to finish within "
        "5 minutes or the authentication will time out. After authenticating this wizard will close and the account will be added."
    ));
    gmailAuthenticationPrompt->setWordWrap(true);
    addGmailAccountPageLayout->addWidget(gmailAuthenticationPrompt, workingRow++, 0);

    std::size_t authRequest = QRequest::singleton().getUniqueRequest();
    std::size_t refreshTokenRequest = QRequest::singleton().getUniqueRequest();
    std::size_t verifyRequest = QRequest::singleton().getUniqueRequest();

    connect(addAccountWizard, &AddAccountWizard::finished, [this, gmailEntryTextLine](){
        gmailEntryTextLine->setText("");
        verified = false;
    });

    connect(&QRequest::singleton(), &QRequest::onResponse, [this, gmailEntryTextLine, verifyRequest, addAccountWizard](std::size_t requestProvided, std::string* response) {
        if (verifyRequest != requestProvided || response == nullptr) {
            return;
        }
        gmail = std::string(*response);
        delete response;

        gmailEntryTextLine->setText(QString::fromStdString(gmail));
        addAccountWizard->clearAccountsToAdd();
        addAccountWizard->addSourceAccountToAdd(std::unique_ptr<SourceAccount>(new GmailAccount(gmail, oauthBearer, refreshToken)));

        verified = true;
        emit completeChanged();
    });

    connect(&QRequest::singleton(), &QRequest::onResponse, [this, refreshTokenRequest, verifyRequest](std::size_t requestProvided, std::string* response) {
        if (refreshTokenRequest != requestProvided || response == nullptr) {
            return;
        }
        refreshToken = std::string(*response);
        std::cout << refreshToken << std::endl;
        delete response;

        QRequest::singleton().gmailGetUser(verifyRequest, oauthBearer);
    });
    connect(&QRequest::singleton(), &QRequest::onResponse, [this, authRequest](std::size_t requestProvided, std::string* response) {
        if (authRequest != requestProvided || response == nullptr) {
            return;
        }
        oauthBearer = std::string(*response);
        delete response;
    });

    auto* gmailAuthenticationButton = new QPushButton(tr("Authenticate"));
    connect(gmailAuthenticationButton, &QPushButton::clicked, [this, gmailEntryTextLine, authRequest, refreshTokenRequest](){
        gmail = gmailEntryTextLine->text().toStdString();
        QRequest::singleton().gmailOAuth(authRequest, refreshTokenRequest);
    });

    addGmailAccountPageLayout->addWidget(gmailAuthenticationButton, workingRow++, 0);
}

bool AddGmailAccountPage::isComplete() const {
    return verified;
}