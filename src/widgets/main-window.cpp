#include "main-window.hpp"

#include "accounts/account-manager.hpp"

#include <QGuiApplication>
#include <QScreen>

#include "get-email-recipients-wizard.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.7);

    auto* windowWidget = new QWidget;
    auto* windowGrid = new QGridLayout(windowWidget);

    windowGrid->addWidget(new AccountsWidget(this), 0, 0);
    windowGrid->addWidget(new AccountInfoWidget(this), 0, 1);

    auto* manageableAccountsButton = new QPushButton(tr("Managed Accounts"));
    auto* manageableAccountGroupsButton = new QPushButton(tr("Managed Account Groups"));
    // set up manageable account type buttons
    QObject::connect(manageableAccountsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountsButtonClicked);
    QObject::connect(manageableAccountGroupsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountGroupsButtonClicked);

    auto* manageableTypeGroupBoxLayout = new QHBoxLayout;
    manageableTypeGroupBoxLayout->addWidget(manageableAccountsButton);
    manageableTypeGroupBoxLayout->addWidget(manageableAccountGroupsButton);
    auto* manageableTypeGroupBox = new QGroupBox(tr("Current Managed Type"));
    manageableTypeGroupBox->setLayout(manageableTypeGroupBoxLayout);
    auto* manageableTypeLayout = new QHBoxLayout;
    manageableTypeLayout->addWidget(manageableTypeGroupBox);
    manageableTypeLayout->addStretch(1);

    windowGrid->addLayout(manageableTypeLayout, 1, 0);

    setCentralWidget(windowWidget);
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