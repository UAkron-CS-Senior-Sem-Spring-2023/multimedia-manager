#include "main-window.hpp"

#include "accounts/account-manager.hpp"

#include <QGuiApplication>
#include <QScreen>

#include "get-email-recipients-wizard.hpp"

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

const Account* MainWindow::selectedAccount() const {
    return selectedAccount_;
}
const AccountGroup* MainWindow::selectedAccountGroup() const {
    return selectedAccountGroup_;
}

void MainWindow::changeSelectedAccount(const Account* account) {
    selectedAccount_ = account;

    emit selectedAccountChanged(selectedAccount_);
    emit selectedAccountSourceRecipientTypeChanged(AccountManager::singleton().accountSourceRecipientType(selectedAccount_));
}

void MainWindow::changeSelectedAccountGroup(const AccountGroup* accountGroup) {
    selectedAccountGroup_ = accountGroup;

    bool isRecipient = true;
    bool isSource = true;
    for (const Account* account : *selectedAccountGroup_) {
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
    emit selectedAccountGroupChanged(selectedAccountGroup_);
}

void MainWindow::manageableAccountsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNTS);
}

void MainWindow::manageableAccountGroupsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNT_GROUPS);
}