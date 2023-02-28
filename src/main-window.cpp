#include "main-window.hpp"

#include <QGuiApplication>
#include <QScreen>
#include <QPalette>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    windowGrid(QGridLayout(&windowWidget)),
    // construct manageable widget
    manageableGrid(QGridLayout(&manageableWidget)),
    managingAccountsLabel(QLabel(tr("Managed Accounts"))),
    managingAccountGroupsLabel(QLabel(tr("Managed Account Groups"))),
    manageableNamesLayout(QVBoxLayout(&manageableNamesWidget)),
    addManageableAccountButton(QPushButton(tr("Add Account"))),
    removeManageableAccountButton(QPushButton(tr("Remove Account"))),
    modifyManageableAccountGroupLayout(QHBoxLayout(&modifyManageableAccountGroupWidget)),
    addManageableAccountGroupButton(QPushButton(tr("Add Account Group"))),
    removeManageableAccountGroupButton(QPushButton(tr("Remove Account Group"))),
    modifyManageableAccountLayout(QHBoxLayout(&modifyManageableAccountWidget)),
    manageableAccountsButton(QPushButton(tr("Managed Accounts"))),
    manageableAccountGroupsButton(QPushButton(tr("Managed Account Groups"))),
    manageableTypeGroupBox(QGroupBox(tr("Current Managed Type"))),
    // construct managing widget
    managingGrid(QGridLayout(&managingWidget)),
    managingAccountsInfoLabel(QLabel(tr("Account Information"))),
    managingAccountGroupsInfoLabel(QLabel(tr("Account Group Information"))),
    managingInfoLayout(QVBoxLayout(&managingInfoWidget)),
    managingSendMessageButton(QPushButton(tr("Send message"))),
    managingViewInboxButton(QPushButton(tr("View inbox")))
{
    resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.7);

    layoutManageableWidget();
    windowGrid.addWidget(&manageableWidget, 0, 0);

    layoutManagingWidget();
    windowGrid.addWidget(&managingWidget, 0, 1);

    setCentralWidget(&windowWidget);
}

void onClickManageableAccountsButton() {

}

void MainWindow::layoutManageableWidget() {
    int workingRow = 0;

    manageableGrid.setColumnStretch(1, 1);

    managingTypeStackedLayout.insertWidget(ACCOUNTS, &managingAccountsLabel);
    managingTypeStackedLayout.insertWidget(ACCOUNT_GROUPS, &managingAccountGroupsLabel);
    
    managingTypeStackedLayout.setCurrentIndex(managingType);
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
    modifyManageablesButtonsStackedLayout.insertWidget(ACCOUNTS, &modifyManageableAccountWidget);

    modifyManageableAccountGroupLayout.addWidget(&addManageableAccountGroupButton);
    modifyManageableAccountGroupLayout.addStretch(1);
    modifyManageableAccountGroupLayout.addWidget(&removeManageableAccountGroupButton);
    modifyManageablesButtonsStackedLayout.insertWidget(ACCOUNT_GROUPS, &modifyManageableAccountGroupWidget);

    modifyManageablesButtonsStackedLayout.setCurrentIndex(managingType);
    manageableGrid.addLayout(&modifyManageablesButtonsStackedLayout, workingRow, 0, 1, 2);
    ++workingRow;

    // set up manageable account type buttons
    QObject::connect(&manageableAccountsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountsButtonClicked);
    QObject::connect(&manageableAccountGroupsButton, &QPushButton::clicked, this, &MainWindow::manageableAccountGroupsButtonClicked);
    manageableTypeLayout.addWidget(&manageableAccountsButton);
    manageableTypeLayout.addWidget(&manageableAccountGroupsButton);
    manageableTypeLayout.addStretch(1);
    manageableTypeGroupBox.setLayout(&manageableTypeLayout);

    manageableGrid.addWidget(&manageableTypeGroupBox, workingRow, 0);
    ++workingRow;
}

void MainWindow::layoutManagingWidget() {
    int workingRow = 0;

    // set up managed accounts info labels
    managingTypeInfoStackedLayout.insertWidget(ACCOUNTS, &managingAccountsInfoLabel);
    managingTypeInfoStackedLayout.insertWidget(ACCOUNT_GROUPS, &managingAccountGroupsInfoLabel);
    managingTypeInfoStackedLayout.setCurrentIndex(managingType);
    managingGrid.addLayout(&managingTypeInfoStackedLayout, workingRow, 0);
    ++workingRow;

    // set up managed accounts info
    managingInfoWidget.setBackgroundRole(QPalette::Base);
    managingInfoWidget.setAutoFillBackground(true);
    managingInfoLayout.setContentsMargins(0, 0, 0, 16);

    managingGrid.setRowStretch(workingRow, 1);
    managingGrid.addWidget(&managingInfoWidget, workingRow, 0);
    ++workingRow;

    // set up actionable buttons for that account

    managingActionButtonsLayout.addWidget(&managingSendMessageButton);
    managingActionButtonsLayout.addStretch(1);
    managingActionButtonsLayout.addWidget(&managingViewInboxButton);
    // 66px magic number, gets buttons to align with lefthand side's textbox
    managingActionButtonsLayout.setContentsMargins(0, 0, 0, 67);

    managingGrid.addLayout(&managingActionButtonsLayout, workingRow, 0);
    ++workingRow;
}

void MainWindow::updateManagingType(ManagingType type) {
    managingType = type;

    managingTypeStackedLayout.setCurrentIndex(managingType);
    managingTypeInfoStackedLayout.setCurrentIndex(managingType);
    modifyManageablesButtonsStackedLayout.setCurrentIndex(managingType);
}

void MainWindow::manageableAccountsButtonClicked() {
    updateManagingType(ACCOUNTS);
}

void MainWindow::manageableAccountGroupsButtonClicked() {
    updateManagingType(ACCOUNT_GROUPS);

}