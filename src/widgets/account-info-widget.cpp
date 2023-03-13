#include "account-info-widget.hpp"

#include "main-window.hpp"

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
        QWidget* from;
        if (currentAccountInfoWidget == nullptr) {
            from = &noAccountInfoWidget;
        } else {
            from = currentAccountInfoWidget;
        }

        QWidget* to;
        if (account == nullptr) {
            currentAccountInfoWidget = nullptr;
            to = &noAccountInfoWidget;
        } else {
            currentAccountInfoWidget = &account->info();
            to = currentAccountInfoWidget;
        }

        managingAccountInfoBackgroundLayout.replaceWidget(from, to);
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
        QWidget* from;
        if (currentAccountInfoWidget == nullptr) {
            from = &noAccountInfoWidget;
        } else {
            from = currentAccountInfoWidget;
        }

        QWidget* to;
        if (accountGroup == nullptr) {
            currentAccountInfoWidget = nullptr;
            to = &noAccountInfoWidget;
        } else {
            currentAccountInfoWidget = &accountGroup->info();
            to = currentAccountInfoWidget;
        }

        managingAccountInfoBackgroundLayout.replaceWidget(from, to);
    });
    managingTypeStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &managingAccountGroupInfoWidget);

    managingTypeStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &managingTypeStackedLayout, &QStackedLayout::setCurrentIndex);
    managingGrid.setRowStretch(workingRow, 1);
    managingGrid.addLayout(&managingTypeStackedLayout, workingRow++, 0);

    // set up actionable buttons for that account

    managingAccountSendMessageButton.setEnabled(false);
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
    managingAccountGroupActionButtonsLayout.addWidget(&managingAccountGroupSendMessageButton);
    managingAccountGroupActionButtonsLayout.addStretch(1);
    managingAccountGroupViewInboxButton.setEnabled(false);
    connect(mainWindow, &MainWindow::selectedAccountGroupSourceRecipientTypeChanged, [this](AccountManager::SourceRecipientType sourceRecipientType){
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