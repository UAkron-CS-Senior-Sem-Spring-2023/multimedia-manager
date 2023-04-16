#include "account-info-widget.hpp"

#include "main-window.hpp"

AccountInfoWidget::AccountInfoWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow)
{
    int workingRow = 0;


    auto* managingGrid = new QGridLayout(this);

    auto* managingTypeStackedLayout = new QStackedLayout;

    // set up managed account info
    int accountInfoWorkingRow = 0;

    auto* managingAccountInfoWidget = new QWidget;
    auto* managingAccountInfoLayout = new QGridLayout(managingAccountInfoWidget);
    managingAccountInfoLayout->setContentsMargins(0, 0, 0, 0);
    managingAccountInfoLayout->addWidget(new QLabel(tr("Account Information")), accountInfoWorkingRow++, 0);

    auto* managingAccountInfoBackgroundWidget = new QWidget;
    auto* managingAccountInfoBackgroundLayout = new QGridLayout(managingAccountInfoBackgroundWidget);
    managingAccountInfoBackgroundWidget->setBackgroundRole(QPalette::Base);
    managingAccountInfoBackgroundWidget->setAutoFillBackground(true);

    auto* noAccountInfoWidget = new QWidget;
    managingAccountInfoBackgroundLayout->addWidget(noAccountInfoWidget, 0, 0);
    managingAccountInfoLayout->setRowStretch(accountInfoWorkingRow, 1);
    managingAccountInfoLayout->addWidget(managingAccountInfoBackgroundWidget, accountInfoWorkingRow++, 0);
    // updates to the new account info selected
    connect(mainWindow, &MainWindow::selectedAccountChanged, [this, managingAccountInfoBackgroundLayout, noAccountInfoWidget](const Account* account){
        if (currentAccountInfoWidget != nullptr) {
            currentAccountInfoWidget->setParent(nullptr);
        }

        QWidget* to;
        if (account == nullptr) {
            currentAccountInfoWidget = nullptr;
            to = noAccountInfoWidget;
        } else {
            currentAccountInfoWidget = &account->info();
            to = currentAccountInfoWidget;
        }

        managingAccountInfoBackgroundLayout->addWidget(to, 0, 0);
    });
    managingTypeStackedLayout->insertWidget(AccountManager::ACCOUNTS, managingAccountInfoWidget);

    // set up managed account group info
    int accountGroupInfoWorkingRow = 0;

    auto* managingAccountGroupInfoWidget = new QWidget;
    auto* managingAccountGroupInfoLayout = new QGridLayout(managingAccountGroupInfoWidget);

    managingAccountGroupInfoLayout->setContentsMargins(0, 0, 0, 0);
    managingAccountGroupInfoLayout->addWidget(new QLabel(tr("Account Group Information")), accountGroupInfoWorkingRow++, 0);

    auto* managingAccountGroupInfoBackgroundWidget = new QWidget;
    auto* managingAccountGroupInfoBackgroundLayout = new QGridLayout(managingAccountGroupInfoBackgroundWidget);
    managingAccountGroupInfoBackgroundWidget->setBackgroundRole(QPalette::Base);
    managingAccountGroupInfoBackgroundWidget->setAutoFillBackground(true);

    auto* noAccountGroupInfoWidget = new QWidget;
    managingAccountGroupInfoBackgroundLayout->addWidget(noAccountGroupInfoWidget, 0, 0);
    managingAccountGroupInfoLayout->setRowStretch(accountGroupInfoWorkingRow, 1);
    managingAccountGroupInfoLayout->addWidget(managingAccountGroupInfoBackgroundWidget, accountGroupInfoWorkingRow++, 0);
    // updates to the new account group info selected
    connect(mainWindow, &MainWindow::selectedAccountGroupChanged, [this, managingAccountGroupInfoBackgroundLayout, noAccountGroupInfoWidget](const AccountGroup* accountGroup){
        if (currentAccountGroupInfoWidget != nullptr) {
            currentAccountGroupInfoWidget->setParent(nullptr);
        }

        QWidget* to;
        if (accountGroup == nullptr) {
            currentAccountGroupInfoWidget = nullptr;
            to = noAccountGroupInfoWidget;
        } else {
            currentAccountGroupInfoWidget = &accountGroup->info();
            to = currentAccountGroupInfoWidget;
        }

        managingAccountGroupInfoBackgroundLayout->addWidget(to, 0, 0);
    });
    managingTypeStackedLayout->insertWidget(AccountManager::ACCOUNT_GROUPS, managingAccountGroupInfoWidget);

    managingTypeStackedLayout->setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, managingTypeStackedLayout, &QStackedLayout::setCurrentIndex);
    managingGrid->setRowStretch(workingRow, 1);
    managingGrid->addLayout(managingTypeStackedLayout, workingRow++, 0);

    // set up actionable buttons for that account
    auto* managingActionButtonsStackedLayout = new QStackedLayout;

    auto* managingAccountActionButtonsWidget = new QWidget;
    auto* managingAccountActionButtonsLayout = new QHBoxLayout(managingAccountActionButtonsWidget);

    auto* managingAccountSendMessageButton = new QPushButton(tr("Send message"));
    managingAccountSendMessageButton->setEnabled(false);
    connect(&accountSendMessageWizard, &SendMessageWizard::gotMessage, [this](std::string subject, Request::MimeData mimeData) {
        dynamic_cast<const RecipientAccount*>(this->mainWindow->selectedAccount())->send(std::move(subject), std::move(mimeData));
    });
    connect(managingAccountSendMessageButton, &QPushButton::clicked, [this]() {
        accountSendMessageWizard.show();
    });
    connect(mainWindow, &MainWindow::selectedAccountChanged, [this](){
        accountSendMessageWizard.hide();
    });
    managingAccountActionButtonsLayout->addWidget(managingAccountSendMessageButton);
    managingAccountActionButtonsLayout->addStretch(1);

    auto* managingAccountViewInboxButton = new QPushButton(tr("View inbox"));
    managingAccountViewInboxButton->setEnabled(false);
    connect(managingAccountViewInboxButton, &QPushButton::clicked, [this]() {
        dynamic_cast<const SourceAccount*>(this->mainWindow->selectedAccount())->inbox().populate();
    });

    managingAccountActionButtonsLayout->addWidget(managingAccountViewInboxButton);
    connect(mainWindow, &MainWindow::selectedAccountSourceRecipientTypeChanged, [this, managingAccountSendMessageButton, managingAccountViewInboxButton](AccountManager::SourceRecipientType sourceRecipientType){
        // enable the button only if the account is capable of sending
        managingAccountSendMessageButton->setEnabled(sourceRecipientType == AccountManager::RECIPIENT || sourceRecipientType == AccountManager::DUAL);
        // enable the button only if the account is capable of receiving
        managingAccountViewInboxButton->setEnabled(sourceRecipientType == AccountManager::SOURCE || sourceRecipientType == AccountManager::DUAL);
    });
    managingActionButtonsStackedLayout->insertWidget(AccountManager::ACCOUNTS, managingAccountActionButtonsWidget);

    auto* managingAccountGroupActionButtonsWidget = new QWidget;
    auto* managingAccountGroupActionButtonsLayout = new QHBoxLayout(managingAccountGroupActionButtonsWidget);


    auto* managingAccountGroupSendMessageButton = new QPushButton(tr("Send message"));
    managingAccountGroupSendMessageButton->setEnabled(false);
    connect(&accountGroupSendMessageWizard, &SendMessageWizard::gotMessage, [this](std::string subject, Request::MimeData mimeData) {
        for (const auto* account : *this->mainWindow->selectedAccountGroup()) {
            dynamic_cast<const RecipientAccount*>(account)->send(std::move(subject), std::move(mimeData));
        }
    });
    connect(managingAccountGroupSendMessageButton, &QPushButton::clicked, [this](){
        accountGroupSendMessageWizard.show();
    });
    connect(mainWindow, &MainWindow::selectedAccountGroupChanged, [this](){
        accountGroupSendMessageWizard.hide();
    });
    managingAccountGroupActionButtonsLayout->addWidget(managingAccountGroupSendMessageButton);
    managingAccountGroupActionButtonsLayout->addStretch(1);

    auto* managingAccountGroupViewInboxButton = new QPushButton(tr("View inbox"));
    managingAccountGroupViewInboxButton->setEnabled(false);
    connect(mainWindow, &MainWindow::selectedAccountGroupSourceRecipientTypeChanged, [this, managingAccountGroupSendMessageButton, managingAccountGroupViewInboxButton](AccountManager::SourceRecipientType sourceRecipientType) {
        // enable the button only if the account is capable of sending
        managingAccountGroupSendMessageButton->setEnabled(sourceRecipientType == AccountManager::RECIPIENT || sourceRecipientType == AccountManager::DUAL);
        // enable the button only if the account is capable of receiving
        managingAccountGroupViewInboxButton->setEnabled(sourceRecipientType == AccountManager::SOURCE || sourceRecipientType == AccountManager::DUAL);
    });
    managingAccountGroupActionButtonsLayout->addWidget(managingAccountGroupViewInboxButton);
    managingActionButtonsStackedLayout->insertWidget(AccountManager::ACCOUNT_GROUPS, managingAccountGroupActionButtonsWidget);
    managingActionButtonsStackedLayout->setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, managingActionButtonsStackedLayout, &QStackedLayout::setCurrentIndex);

    managingGrid->addLayout(managingActionButtonsStackedLayout, workingRow++, 0);
}

AccountInfoWidget::~AccountInfoWidget() {
    // set their parents to nullptr so they aren't deleted by qt parent-child deletion
    if (currentAccountInfoWidget != nullptr) {
        currentAccountInfoWidget->setParent(nullptr);
    }
    if (currentAccountGroupInfoWidget != nullptr) {
        currentAccountGroupInfoWidget->setParent(nullptr);
    }
}