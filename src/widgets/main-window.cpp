#include "main-window.hpp"

#include "accounts/account-manager.hpp"

#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    windowGrid(QGridLayout(&windowWidget)),
    accountsWidget(this),
    accountInfoWidget(this),
    manageableAccountsButton(QPushButton(tr("Managed Accounts"))),
    manageableAccountGroupsButton(QPushButton(tr("Managed Account Groups"))),
    manageableTypeGroupBox(QGroupBox(tr("Current Managed Type")))
{
    resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.7);

    windowGrid.addWidget(&accountsWidget, 0, 0);
    windowGrid.addWidget(&accountInfoWidget, 0, 1);

    // set up manageable account type buttons
    QObject::connect(&manageableAccountsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountsButtonClicked);
    QObject::connect(&manageableAccountGroupsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountGroupsButtonClicked);
    manageableTypeGroupBoxLayout.addWidget(&manageableAccountsButton);
    manageableTypeGroupBoxLayout.addWidget(&manageableAccountGroupsButton);
    manageableTypeGroupBox.setLayout(&manageableTypeGroupBoxLayout);
    manageableTypeLayout.addWidget(&manageableTypeGroupBox);
    manageableTypeLayout.addStretch(1);

    windowGrid.addLayout(&manageableTypeLayout, 1, 0);

    setCentralWidget(&windowWidget);
}

AddAccountWizard& MainWindow::addAccountWizard() {
    return addAccountWizard_;
}

void MainWindow::changeSelectedAccount(const Account* account) {
    emit selectedAccountChanged(account);
    emit selectedAccountSourceRecipientTypeChanged(AccountManager::singleton().accountSourceRecipientType(account));
}

void MainWindow::changeSelectedAccountGroup(const AccountGroup* accountGroup) {
    emit selectedAccountGroupChanged(accountGroup);
    bool isRecipient = true;
    bool isSource = true;
    for (const Account* account : *accountGroup) {
        auto sourceRecipientType = AccountManager::singleton().accountSourceRecipientType(account);
        isSource &= sourceRecipientType == AccountManager::SOURCE || sourceRecipientType == AccountManager::DUAL;
        isRecipient &= sourceRecipientType == AccountManager::RECIPIENT || sourceRecipientType == AccountManager::DUAL;
    }
    if (isRecipient && isSource) {
        emit selectedAccountGroupSourceRecipientTypeChanged(AccountManager::DUAL);
    } else if (isRecipient) {
        emit selectedAccountGroupSourceRecipientTypeChanged(AccountManager::RECIPIENT);
    } else if (isSource) {
        emit selectedAccountGroupSourceRecipientTypeChanged(AccountManager::SOURCE);
    } else {
        emit selectedAccountGroupSourceRecipientTypeChanged(AccountManager::NONE);
    }
}

void MainWindow::manageableAccountsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNTS);
}

void MainWindow::manageableAccountGroupsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNT_GROUPS);
}