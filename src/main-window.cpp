#include "main-window.hpp"

#include <QPalette>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    windowWidget = std::make_unique<QWidget>();
    windowGrid = std::make_unique<QGridLayout>(windowWidget.get());

    constructManageableWidget();
    windowGrid->addWidget(manageableWidget.get(), 0, 0);

    constructManagingWidget();
    windowGrid->addWidget(managingWidget.get(), 0, 1);

    setCentralWidget(windowWidget.get());
}

void onClickManageableAccountsButton() {

}

void MainWindow::constructManageableWidget() {
    int workingRow = 0;
    manageableWidget = std::make_unique<QWidget>();

    manageableGrid = std::make_unique<QGridLayout>(manageableWidget.get());
    manageableGrid->setColumnStretch(1, 1);

    managingTypeStackedLayout = std::make_unique<QStackedLayout>();
    managingAccountsLabel = std::make_unique<QLabel>(tr("Managed Accounts"));
    managingTypeStackedLayout->insertWidget(ACCOUNTS, managingAccountsLabel.get());
    managingAccountGroupsLabel = std::make_unique<QLabel>(tr("Managed Account Groups"));
    managingTypeStackedLayout->insertWidget(ACCOUNT_GROUPS, managingAccountGroupsLabel.get());
    
    managingTypeStackedLayout->setCurrentIndex(managingType);
    manageableGrid->addLayout(managingTypeStackedLayout.get(), workingRow, 0);
    ++workingRow;

    // set up manageable list
    manageableNamesWidget = std::make_unique<QWidget>();
    manageableNamesWidget->setBackgroundRole(QPalette::Base);
    manageableNamesWidget->setAutoFillBackground(true);
    manageableNamesLayout = std::make_unique<QVBoxLayout>(manageableNamesWidget.get());
    manageableNamesLayout->setContentsMargins(0, 0, 0, 16);

    manageableGrid->setRowStretch(workingRow, 1);
    manageableGrid->addWidget(manageableNamesWidget.get(), workingRow, 0, 1, 2);
    ++workingRow;

    // set up buttons to modify accounts
    modifyManageablesButtonsStackedLayout = std::make_unique<QStackedLayout>();

    modifyManageableAccountWidget = std::make_unique<QWidget>();
    addManageableAccountButton = std::make_unique<QPushButton>(tr("Add Account"));
    removeManageableAccountButton = std::make_unique<QPushButton>(tr("Remove Account"));
    modifyManageableAccountLayout = std::make_unique<QHBoxLayout>(modifyManageableAccountWidget.get());
    modifyManageableAccountLayout->addWidget(addManageableAccountButton.get());
    modifyManageableAccountLayout->addStretch(1);
    modifyManageableAccountLayout->addWidget(removeManageableAccountButton.get());
    modifyManageablesButtonsStackedLayout->insertWidget(ACCOUNTS, modifyManageableAccountWidget.get());

    modifyManageableAccountGroupWidget = std::make_unique<QWidget>();
    addManageableAccountGroupButton = std::make_unique<QPushButton>(tr("Add Account Group"));
    removeManageableAccountGroupButton = std::make_unique<QPushButton>(tr("Remove Account Group"));
    modifyManageableAccountGroupLayout = std::make_unique<QHBoxLayout>(modifyManageableAccountGroupWidget.get());
    modifyManageableAccountGroupLayout->addWidget(addManageableAccountGroupButton.get());
    modifyManageableAccountGroupLayout->addStretch(1);
    modifyManageableAccountGroupLayout->addWidget(removeManageableAccountGroupButton.get());
    modifyManageablesButtonsStackedLayout->insertWidget(ACCOUNT_GROUPS, modifyManageableAccountGroupWidget.get());

    modifyManageablesButtonsStackedLayout->setCurrentIndex(managingType);
    manageableGrid->addLayout(modifyManageablesButtonsStackedLayout.get(), workingRow, 0, 1, 2);
    ++workingRow;

    // set up manageable account type buttons
    manageableAccountsButton = std::make_unique<QPushButton>(tr("Managed Accounts"));
    QObject::connect(manageableAccountsButton.get(), &QPushButton::clicked, this, &MainWindow::manageableAccountsButtonClicked);
    manageableAccountGroupsButton = std::make_unique<QPushButton>(tr("Managed Account Groups"));
    QObject::connect(manageableAccountGroupsButton.get(), &QPushButton::clicked, this, &MainWindow::manageableAccountGroupsButtonClicked);
    
    manageableTypeLayout = std::make_unique<QHBoxLayout>();
    manageableTypeLayout->addWidget(manageableAccountsButton.get());
    manageableTypeLayout->addWidget(manageableAccountGroupsButton.get());
    manageableTypeLayout->addStretch(1);
    
    manageableTypeGroupBox = std::make_unique<QGroupBox>(tr("Current Managed Type"));
    manageableTypeGroupBox->setLayout(manageableTypeLayout.get());

    manageableGrid->addWidget(manageableTypeGroupBox.get(), workingRow, 0);
    ++workingRow;
}

void MainWindow::constructManagingWidget() {
    int workingRow = 0;
    managingWidget = std::make_unique<QWidget>();

    managingGrid = std::make_unique<QGridLayout>(managingWidget.get());

    // set up managed accounts info labels
    managingTypeInfoStackedLayout = std::make_unique<QStackedLayout>();
    managingAccountsInfoLabel = std::make_unique<QLabel>(tr("Account Information"));
    managingTypeInfoStackedLayout->insertWidget(ACCOUNTS, managingAccountsInfoLabel.get());
    managingAccountGroupsInfoLabel = std::make_unique<QLabel>(tr("Account Group Information"));
    managingTypeInfoStackedLayout->insertWidget(ACCOUNT_GROUPS, managingAccountGroupsInfoLabel.get());
    
    managingTypeInfoStackedLayout->setCurrentIndex(managingType);
    managingGrid->addLayout(managingTypeInfoStackedLayout.get(), workingRow, 0);
    ++workingRow;

    // set up managed accounts info
    managingInfoWidget = std::make_unique<QWidget>();
    managingInfoWidget->setBackgroundRole(QPalette::Base);
    managingInfoWidget->setAutoFillBackground(true);
    managingInfoLayout = std::make_unique<QVBoxLayout>(managingInfoWidget.get());
    managingInfoLayout->setContentsMargins(0, 0, 0, 16);

    managingGrid->setRowStretch(workingRow, 1);
    managingGrid->addWidget(managingInfoWidget.get(), workingRow, 0);
    ++workingRow;

    // set up actionable buttons for that account
    managingSendMessageButton = std::make_unique<QPushButton>(tr("Send message"));
    managingViewInboxButton = std::make_unique<QPushButton>(tr("View inbox"));

    managingActionButtonsLayout = std::make_unique<QHBoxLayout>();
    managingActionButtonsLayout->addWidget(managingSendMessageButton.get());
    managingActionButtonsLayout->addStretch(1);
    managingActionButtonsLayout->addWidget(managingViewInboxButton.get());
    // 66px magic number, gets buttons to align with lefthand side's textbox
    managingActionButtonsLayout->setContentsMargins(0, 0, 0, 67);

    managingGrid->addLayout(managingActionButtonsLayout.get(), workingRow, 0);
    ++workingRow;
}

void MainWindow::updateManagingType(ManagingType type) {
    managingType = type;

    managingTypeStackedLayout->setCurrentIndex(managingType);
    managingTypeInfoStackedLayout->setCurrentIndex(managingType);
    modifyManageablesButtonsStackedLayout->setCurrentIndex(managingType);
}

void MainWindow::manageableAccountsButtonClicked() {
    updateManagingType(ACCOUNTS);
}

void MainWindow::manageableAccountGroupsButtonClicked() {
    updateManagingType(ACCOUNT_GROUPS);

}