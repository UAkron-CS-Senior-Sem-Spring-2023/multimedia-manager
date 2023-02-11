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

void MainWindow::constructManageableWidget() {
    manageableWidget = std::make_unique<QWidget>();

    manageableGrid = std::make_unique<QGridLayout>(manageableWidget.get());
    manageableGrid->setRowStretch(0, 1);
    manageableGrid->setColumnStretch(1, 1);

    // set up manageable list
    manageableNamesWidget = std::make_unique<QWidget>();
    manageableNamesWidget->setBackgroundRole(QPalette::Base);
    manageableNamesWidget->setAutoFillBackground(true);
    manageableNamesLayout = std::make_unique<QVBoxLayout>(manageableNamesWidget.get());
    manageableNamesLayout->setContentsMargins(0, 0, 0, 16);

    manageableGrid->addWidget(manageableNamesWidget.get(), 0, 0, 1, 2);

    // set up manageable account type buttons
    manageableAccountsButton = std::make_unique<QPushButton>(tr("Managed Accounts"));
    manageableAccountGroupsButton = std::make_unique<QPushButton>(tr("Managed Account Groups"));

    manageableTypeLayout = std::make_unique<QHBoxLayout>();
    manageableTypeLayout->addWidget(manageableAccountsButton.get());
    manageableTypeLayout->addWidget(manageableAccountGroupsButton.get());
    manageableTypeLayout->addStretch(1);
    
    manageableTypeGroupBox = std::make_unique<QGroupBox>(tr("Current Managed Type"));
    manageableTypeGroupBox->setLayout(manageableTypeLayout.get());

    manageableGrid->addWidget(manageableTypeGroupBox.get(), 1, 0);
}

void MainWindow::constructManagingWidget() {
    managingWidget = std::make_unique<QWidget>();

    managingGrid = std::make_unique<QGridLayout>(managingWidget.get());
    managingGrid->setRowStretch(0, 1);

    // set up managed accounts info
    managingInfoWidget = std::make_unique<QWidget>();
    managingInfoWidget->setBackgroundRole(QPalette::Base);
    managingInfoWidget->setAutoFillBackground(true);
    managingInfoLayout = std::make_unique<QVBoxLayout>(managingInfoWidget.get());
    managingInfoLayout->setContentsMargins(0, 0, 0, 16);

    managingGrid->addWidget(managingInfoWidget.get(), 0, 0);

    // set up actionable buttons for that account
    managingSendMessageButton = std::make_unique<QPushButton>(tr("Send message"));
    managingViewInboxButton = std::make_unique<QPushButton>(tr("View inbox"));

    managingActionButtonsLayout = std::make_unique<QHBoxLayout>();
    managingActionButtonsLayout->addWidget(managingSendMessageButton.get());
    managingActionButtonsLayout->addStretch(1);
    managingActionButtonsLayout->addWidget(managingViewInboxButton.get());
    // 66px magic number, gets buttons to align with lefthand side's textbox
    managingActionButtonsLayout->setContentsMargins(0, 0, 0, 66);

    managingGrid->addLayout(managingActionButtonsLayout.get(), 1, 0);
}