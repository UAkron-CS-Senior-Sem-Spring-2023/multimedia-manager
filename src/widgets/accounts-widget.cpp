#include "accounts-widget.hpp"

#include "main-window.hpp"

#include <iostream>

AccountsWidget::AccountsWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow),
    manageableGrid(QGridLayout(this)),
    managingAccountsLabel(QLabel(tr("Managed Accounts"))),
    managingAccountGroupsLabel(QLabel(tr("Managed Account Groups"))),
    accountNamesLayout(QGridLayout(&accountNamesWidget)),
    modifyManageableAccountLayout(QHBoxLayout(&modifyManageableAccountWidget)),
    addManageableAccountButton(QPushButton(tr("Add Account"))),
    removeManageableAccountButton(QPushButton(tr("Remove Account"))),
    accountGroupNamesLayout(QGridLayout(&accountGroupNamesWidget)),
    modifyManageableAccountGroupLayout(QHBoxLayout(&modifyManageableAccountGroupWidget)),
    addManageableAccountGroupButton(QPushButton(tr("Add Account Group"))),
    removeManageableAccountGroupButton(QPushButton(tr("Remove Account Group")))
{
    int workingRow = 0;

    manageableGrid.setColumnStretch(1, 1);

    managingTypeStackedLayout.insertWidget(AccountManager::ACCOUNTS, &managingAccountsLabel);
    managingTypeStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &managingAccountGroupsLabel);
    managingTypeStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &managingTypeStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid.addLayout(&managingTypeStackedLayout, workingRow++, 0);

    // set up manageable list
    accountNamesWidget.setBackgroundRole(QPalette::Base);
    accountNamesWidget.setAutoFillBackground(true);
    connect(&AccountManager::singleton(), &AccountManager::onAccountsChanged, [this](const SetList<Account*>& accounts) {
        for (auto& widget : accountNames) {
            accountNamesLayout.removeWidget(widget.get());
        }
        accountNames.clear();
        
        int workingRow = 0;
        for (const auto* account : accounts) {
            accountNames.push_back(std::make_unique<QPushButton>(QString::fromStdString(account->name())));
            accountNamesLayout.addWidget(accountNames.back().get(), workingRow++, 0);
            connect(accountNames.back().get(), &QPushButton::clicked, [this, account](){
                this->mainWindow->changeSelectedAccount(account);
            });
        }
        accountNamesLayout.setRowStretch(workingRow, 1);
    });
    namesStackedLayout.insertWidget(AccountManager::ACCOUNTS, &accountNamesWidget);

    accountGroupNamesWidget.setBackgroundRole(QPalette::Base);
    accountGroupNamesWidget.setAutoFillBackground(true);
    connect(&AccountManager::singleton(), &AccountManager::onAccountGroupsChanged, [this](const SetList<AccountGroup*>& accountGroups) {
        for (auto& widget : accountGroupNames) {
            accountGroupNamesLayout.removeWidget(widget.get());
        }
        accountGroupNames.clear();

        int workingRow = 0;
        for (const auto* accountGroup : accountGroups) {
            accountGroupNames.push_back(std::make_unique<QPushButton>(QString::fromStdString(accountGroup->name())));
            accountGroupNamesLayout.addWidget(accountGroupNames.back().get(), workingRow++, 0);
            connect(accountGroupNames.back().get(), &QPushButton::clicked, [this, accountGroup](){
                this->mainWindow->changeSelectedAccountGroup(accountGroup);
            });
        }
        accountGroupNamesLayout.setRowStretch(workingRow, 1);
    });
    namesStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &accountGroupNamesWidget);

    manageableGrid.setRowStretch(workingRow, 1);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &namesStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid.addLayout(&namesStackedLayout, workingRow++, 0, 1, 2);

    // set up buttons to modify accounts
    connect(&addManageableAccountButton, &QPushButton::clicked, [this]() {
        addAccountWizard.show();
    });
    modifyManageableAccountLayout.addWidget(&addManageableAccountButton);
    modifyManageableAccountLayout.addStretch(1);
    modifyManageableAccountLayout.addWidget(&removeManageableAccountButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNTS, &modifyManageableAccountWidget);

    connect(&addManageableAccountGroupButton, &QPushButton::clicked, [this]() {
        addAccountGroupWizard.show();
    });
    modifyManageableAccountGroupLayout.addWidget(&addManageableAccountGroupButton);
    modifyManageableAccountGroupLayout.addStretch(1);
    modifyManageableAccountGroupLayout.addWidget(&removeManageableAccountGroupButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &modifyManageableAccountGroupWidget);

    modifyManageablesButtonsStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &modifyManageablesButtonsStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid.addLayout(&modifyManageablesButtonsStackedLayout, workingRow++, 0, 1, 2);
}