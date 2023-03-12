#include "accounts-widget.hpp"

#include "main-window.hpp"

AccountsWidget::AccountsWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow),
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
    manageableGrid.addLayout(&managingTypeStackedLayout, workingRow, 0);
    ++workingRow;

    // set up manageable list
    manageableNamesWidget.setBackgroundRole(QPalette::Base);
    manageableNamesWidget.setAutoFillBackground(true);
    manageableNamesLayout.setContentsMargins(0, 0, 0, 16);

    manageableGrid.setRowStretch(workingRow, 1);
    manageableGrid.addWidget(&manageableNamesWidget, workingRow, 0, 1, 2);
    ++workingRow;

    // set up buttons to modify accounts
    modifyManageableAccountLayout.addWidget(&addManageableAccountButton);
    modifyManageableAccountLayout.addStretch(1);
    modifyManageableAccountLayout.addWidget(&removeManageableAccountButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNTS, &modifyManageableAccountWidget);

    modifyManageableAccountGroupLayout.addWidget(&addManageableAccountGroupButton);
    modifyManageableAccountGroupLayout.addStretch(1);
    modifyManageableAccountGroupLayout.addWidget(&removeManageableAccountGroupButton);
    modifyManageablesButtonsStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &modifyManageableAccountGroupWidget);

    modifyManageablesButtonsStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, &modifyManageablesButtonsStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid.addLayout(&modifyManageablesButtonsStackedLayout, workingRow, 0, 1, 2);
    ++workingRow;
}