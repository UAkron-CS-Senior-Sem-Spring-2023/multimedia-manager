#include "add-outlook-account-page.hpp"

#include "add-account-wizard.hpp"
#include "q-request.hpp"

#include "accounts/outlook-account.hpp"

#include <iostream>

AddOutlookAccountPage::AddOutlookAccountPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : ValidatableAddAccountPage(parent),
    addAccountWizard(addAccountWizard)
{
    auto* addOutlookAccountPageLayout = new QGridLayout;
    setLayout(addOutlookAccountPageLayout);

    int workingRow = 0;


    auto* outlookAccountInformation = new QLabel(tr("Enter the information for the outlook account you want to add."));
    outlookAccountInformation->setWordWrap(true);
    addOutlookAccountPageLayout->addWidget(outlookAccountInformation, workingRow++, 0);
    addOutlookAccountPageLayout->setRowStretch(workingRow++, 1);
    auto* outlookUsernameEntryLabel = new QLabel(tr("Your email address will be automatically entered into this field upon authenticating."));
    outlookUsernameEntryLabel->setWordWrap(true);
    addOutlookAccountPageLayout->addWidget(outlookUsernameEntryLabel, workingRow++, 0);
    
    auto* outlookUsernameEntryTextLine = new QLineEdit;
    outlookUsernameEntryTextLine->setReadOnly(true);
    QPalette uneditableTextLinePalette;
    uneditableTextLinePalette.setColor(QPalette::Base, Qt::black);
    uneditableTextLinePalette.setColor(QPalette::Text, Qt::darkGray);
    outlookUsernameEntryTextLine->setPalette(uneditableTextLinePalette);
    addOutlookAccountPageLayout->addWidget(outlookUsernameEntryTextLine, workingRow++, 0);
    auto* outlookAuthenticationPrompt = new QLabel(tr("Click the button below to authenticate in browser. You will need to finish within "
        "5 minutes or the authentication will time out. After authenticating this wizard will close and the account will be added."
    ));
    outlookAuthenticationPrompt->setWordWrap(true);
    addOutlookAccountPageLayout->addWidget(outlookAuthenticationPrompt, workingRow++, 0);

    std::size_t authRequest = QRequest::singleton().getUniqueRequest();
    std::size_t refreshTokenRequest = QRequest::singleton().getUniqueRequest();
    std::size_t verifyRequest = QRequest::singleton().getUniqueRequest();

    connect(addAccountWizard, &AddAccountWizard::finished, [this, outlookUsernameEntryTextLine](){
        outlookUsernameEntryTextLine->setText("");
        verified = false;
    });

    connect(&QRequest::singleton(), &QRequest::onResponse, [this, outlookUsernameEntryTextLine, verifyRequest, addAccountWizard](std::size_t requestProvided, std::string* response) {
        if (verifyRequest != requestProvided || response == nullptr) {
            return;
        }
        outlookUsername = std::string(*response);
        delete response;

        outlookUsernameEntryTextLine->setText(QString::fromStdString(outlookUsername));
        addAccountWizard->clearAccountsToAdd();
        addAccountWizard->addSourceAccountToAdd(std::unique_ptr<SourceAccount>(new OutlookAccount(outlookUsername, oauthBearer, refreshToken)));

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

        QRequest::singleton().outlookGetUser(verifyRequest, oauthBearer);
    });
    connect(&QRequest::singleton(), &QRequest::onResponse, [this, authRequest](std::size_t requestProvided, std::string* response) {
        if (authRequest != requestProvided || response == nullptr) {
            return;
        }
        oauthBearer = std::string(*response);
        delete response;
    });

    auto* outlookAuthenticationButton = new QPushButton(tr("Authenticate"));
    connect(outlookAuthenticationButton, &QPushButton::clicked, [this, outlookUsernameEntryTextLine, authRequest, refreshTokenRequest](){
        outlookUsername = outlookUsernameEntryTextLine->text().toStdString();
        QRequest::singleton().outlookOAuth(authRequest, refreshTokenRequest);
    });

    addOutlookAccountPageLayout->addWidget(outlookAuthenticationButton, workingRow++, 0);
}

bool AddOutlookAccountPage::isComplete() const {
    return verified;
}