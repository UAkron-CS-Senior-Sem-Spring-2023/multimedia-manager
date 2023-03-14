#include "account-info-widget.hpp"

#include "main-window.hpp"

#include <iostream>

AccountInfoWidget::AccountInfoWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow),
    managingGrid(QGridLayout(this)),
    managingAccountInfoLayout(&managingAccountInfoWidget),
    managingAccountInfoLabel(QLabel(tr("Account Information"))),
    managingAccountInfoBackgroundLayout(&managingAccountInfoBackgroundWidget),
    managingAccountGroupInfoLayout(&managingAccountGroupInfoWidget),
    managingAccountGroupInfoLabel(QLabel(tr("Account Group Information"))),
    managingAccountGroupInfoBackgroundLayout(&managingAccountGroupInfoBackgroundWidget),
    managingAccountActionButtonsLayout(&managingAccountActionButtonsWidget),
    managingAccountSendMessageButton(QPushButton(tr("Send message"))),
    managingAccountViewInboxButton(QPushButton(tr("View inbox"))),
    managingAccountGroupActionButtonsLayout(&managingAccountGroupActionButtonsWidget),
    managingAccountGroupSendMessageButton(QPushButton(tr("Send message"))),
    managingAccountGroupViewInboxButton(QPushButton(tr("View inbox")))
{
    int workingRow = 0;


    // set up managed account info
    int accountInfoWorkingRow = 0;
    managingAccountInfoLayout.setContentsMargins(0, 0, 0, 0);
    managingAccountInfoLayout.addWidget(&managingAccountInfoLabel, accountInfoWorkingRow++, 0);
    managingAccountInfoBackgroundWidget.setBackgroundRole(QPalette::Base);
    managingAccountInfoBackgroundWidget.setAutoFillBackground(true);
    managingAccountInfoBackgroundLayout.addWidget(&noAccountInfoWidget, 0, 0);
    managingAccountInfoLayout.setRowStretch(accountInfoWorkingRow, 1);
    managingAccountInfoLayout.addWidget(&managingAccountInfoBackgroundWidget, accountInfoWorkingRow++, 0);
    // updates to the new account info selected
    connect(mainWindow, &MainWindow::selectedAccountChanged, [this](const Account* account){
        if (currentAccountInfoWidget == nullptr) {
            noAccountInfoWidget.setParent(nullptr);
        } else {
            currentAccountInfoWidget->setParent(nullptr);
        }

        QWidget* to;
        if (account == nullptr) {
            currentAccountInfoWidget = nullptr;
            to = &noAccountInfoWidget;
        } else {
            currentAccountInfoWidget = &account->info();
            to = currentAccountInfoWidget;
        }

        managingAccountInfoBackgroundLayout.addWidget(to, 0, 0);
    });
    managingTypeStackedLayout.insertWidget(AccountManager::ACCOUNTS, &managingAccountInfoWidget);

    // set up managed account group info
    int accountGroupInfoWorkingRow = 0;
    managingAccountGroupInfoLayout.setContentsMargins(0, 0, 0, 0);
    managingAccountGroupInfoLayout.addWidget(&managingAccountGroupInfoLabel, accountGroupInfoWorkingRow++, 0);
    managingAccountGroupInfoBackgroundWidget.setBackgroundRole(QPalette::Base);
    managingAccountGroupInfoBackgroundWidget.setAutoFillBackground(true);
    managingAccountGroupInfoBackgroundLayout.addWidget(&noAccountGroupInfoWidget, 0, 0);
    managingAccountGroupInfoLayout.setRowStretch(accountGroupInfoWorkingRow, 1);
    managingAccountGroupInfoLayout.addWidget(&managingAccountGroupInfoBackgroundWidget, accountGroupInfoWorkingRow++, 0);
    // updates to the new account group info selected
    connect(mainWindow, &MainWindow::selectedAccountGroupChanged, [this](const AccountGroup* accountGroup){
        if (currentAccountGroupInfoWidget == nullptr) {
            noAccountGroupInfoWidget.setParent(nullptr);
        } else {
            currentAccountGroupInfoWidget->setParent(nullptr);
        }

        QWidget* to;
        if (accountGroup == nullptr) {
            currentAccountGroupInfoWidget = nullptr;
            to = &noAccountGroupInfoWidget;
        } else {
            currentAccountGroupInfoWidget = &accountGroup->info();
            to = currentAccountGroupInfoWidget;
        }

        managingAccountGroupInfoBackgroundLayout.addWidget(to, 0, 0);
    });
    managingTypeStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &managingAccountGroupInfoWidget);

    managingTypeStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &managingTypeStackedLayout, &QStackedLayout::setCurrentIndex);
    managingGrid.setRowStretch(workingRow, 1);
    managingGrid.addLayout(&managingTypeStackedLayout, workingRow++, 0);

    // set up actionable buttons for that account

    managingAccountSendMessageButton.setEnabled(false);
    connect(&accountSendMessageWizard, &SendMessageWizard::gotMessage, [this](std::string subject, Request::MimeData mimeData) {
        dynamic_cast<const RecipientAccount*>(this->mainWindow->selectedAccount())->send(std::move(subject), std::move(mimeData));
    });
    connect(&managingAccountSendMessageButton, &QPushButton::clicked, [this](){
        accountSendMessageWizard.show();
    });
    connect(mainWindow, &MainWindow::selectedAccountChanged, [this](){
        accountSendMessageWizard.hide();
    });
    managingAccountActionButtonsLayout.addWidget(&managingAccountSendMessageButton);
    managingAccountActionButtonsLayout.addStretch(1);
    managingAccountViewInboxButton.setEnabled(false);
    managingAccountActionButtonsLayout.addWidget(&managingAccountViewInboxButton);
    connect(mainWindow, &MainWindow::selectedAccountSourceRecipientTypeChanged, [this](AccountManager::SourceRecipientType sourceRecipientType){
        // enable the button only if the account is capable of sending
        managingAccountSendMessageButton.setEnabled(sourceRecipientType == AccountManager::RECIPIENT || sourceRecipientType == AccountManager::DUAL);
        // enable the button only if the account is capable of receiving
        managingAccountViewInboxButton.setEnabled(sourceRecipientType == AccountManager::SOURCE || sourceRecipientType == AccountManager::DUAL);
    });
    managingActionButtonsStackedLayout.insertWidget(AccountManager::ACCOUNTS, &managingAccountActionButtonsWidget);

    managingAccountGroupSendMessageButton.setEnabled(false);
    connect(&accountGroupSendMessageWizard, &SendMessageWizard::gotMessage, [this](std::string subject, Request::MimeData mimeData) {
        for (const auto* account : *this->mainWindow->selectedAccountGroup()) {
            std::cout << "sent on " << account->name() << std::endl;
            dynamic_cast<const RecipientAccount*>(account)->send(std::move(subject), std::move(mimeData));
        }
    });
    connect(&managingAccountGroupSendMessageButton, &QPushButton::clicked, [this](){
        accountGroupSendMessageWizard.show();
    });
    connect(mainWindow, &MainWindow::selectedAccountGroupChanged, [this](){
        accountGroupSendMessageWizard.hide();
    });
    managingAccountGroupActionButtonsLayout.addWidget(&managingAccountGroupSendMessageButton);
    managingAccountGroupActionButtonsLayout.addStretch(1);
    managingAccountGroupViewInboxButton.setEnabled(false);
    connect(mainWindow, &MainWindow::selectedAccountGroupSourceRecipientTypeChanged, [this](AccountManager::SourceRecipientType sourceRecipientType) {
        // enable the button only if the account is capable of sending
        managingAccountGroupSendMessageButton.setEnabled(sourceRecipientType == AccountManager::RECIPIENT || sourceRecipientType == AccountManager::DUAL);
        // enable the button only if the account is capable of receiving
        managingAccountGroupViewInboxButton.setEnabled(sourceRecipientType == AccountManager::SOURCE || sourceRecipientType == AccountManager::DUAL);
    });
    managingAccountGroupActionButtonsLayout.addWidget(&managingAccountGroupViewInboxButton);
    managingActionButtonsStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &managingAccountGroupActionButtonsWidget);
    managingActionButtonsStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &managingActionButtonsStackedLayout, &QStackedLayout::setCurrentIndex);

    managingGrid.addLayout(&managingActionButtonsStackedLayout, workingRow++, 0);
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