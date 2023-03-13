#include "accounts-widget.hpp"

#include "main-window.hpp"

AccountsWidget::AccountsWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow),
    addAccountWizard(&mainWindow->accountManager()),
    manageableGrid(QGridLayout(this)),
    managingAccountsLabel(QLabel(tr("Managed Accounts"))),
    managingAccountGroupsLabel(QLabel(tr("Managed Account Groups"))),
    manageableNamesLayout(QVBoxLayout(&manageableNamesWidget)),
    modifyManageableAccountLayout(QHBoxLayout(&modifyManageableAccountWidget)),
    addManageableAccountButton(QPushButton(tr("Add Account"))),
    removeManageableAccountButton(QPushButton(tr("Remove Account"))),
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
    manageableNamesWidget.setBackgroundRole(QPalette::Base);
    manageableNamesWidget.setAutoFillBackground(true);
    manageableNamesLayout.setContentsMargins(0, 0, 0, 16);
    connect(&mainWindow->accountManager(), &AccountManager::onAccountAdded, [this](const Account* account){
        manageableNames.push_back(std::make_unique<QLabel>(QString::fromStdString(account->name())));
        manageableNamesLayout.addWidget(manageableNames.back().get());
    });

    manageableGrid.setRowStretch(workingRow, 1);
    manageableGrid.addWidget(&manageableNamesWidget, workingRow++, 0, 1, 2);

    // set up buttons to modify accounts
    modifyManageableAccountLayout.addWidget(&addManageableAccountButton);
    connect(&addManageableAccountButton, &QPushButton::clicked, this, &AccountsWidget::addAccountClicked);
    modifyManageableAccountLayout.addStretch(1);
    modifyManageableAccountLayout.addWidget(&removeManageableAccountButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNTS, &modifyManageableAccountWidget);

    modifyManageableAccountGroupLayout.addWidget(&addManageableAccountGroupButton);
    modifyManageableAccountGroupLayout.addStretch(1);
    modifyManageableAccountGroupLayout.addWidget(&removeManageableAccountGroupButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &modifyManageableAccountGroupWidget);

    modifyManageablesButtonsStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &modifyManageablesButtonsStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid.addLayout(&modifyManageablesButtonsStackedLayout, workingRow++, 0, 1, 2);
}

void AccountsWidget::addAccountClicked() {
    addAccountWizard.show();
}