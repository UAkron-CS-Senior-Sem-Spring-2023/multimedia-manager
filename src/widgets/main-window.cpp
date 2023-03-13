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

AccountManager& MainWindow::accountManager() {
    return accountManager_;
}

void MainWindow::manageableAccountsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNTS);
}

void MainWindow::manageableAccountGroupsButtonClicked() {
    emit onUpdateManagingType(AccountManager::ACCOUNT_GROUPS);
}