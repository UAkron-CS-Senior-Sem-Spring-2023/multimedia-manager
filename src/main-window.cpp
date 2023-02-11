#include "main-window.hpp"

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
    manageableNamesLayout = std::make_unique<QVBoxLayout>();
    manageableNamesLayout->setContentsMargins(0, 0, 0, 16);

    manageableGrid->addLayout(manageableNamesLayout.get(), 1, 0);

    // set up manageable account type buttons

    manageableAccountsButton = std::make_unique<QPushButton>(tr("Managed Accounts"));
    manageableAccountGroupsButton = std::make_unique<QPushButton>(tr("Managed Account Groups"));

    manageableTypeLayout = std::make_unique<QHBoxLayout>();
    manageableTypeLayout->addWidget(manageableAccountsButton.get());
    manageableTypeLayout->addWidget(manageableAccountGroupsButton.get());
    manageableTypeLayout->addStretch(1);
    
    manageableTypeGroupBox = std::make_unique<QGroupBox>(tr("Current Managed Type"));
    manageableTypeGroupBox->setLayout(manageableTypeLayout.get());

    manageableGrid->addWidget(manageableTypeGroupBox.get(), 2, 0);
}

void MainWindow::constructManagingWidget() {
    managingWidget = std::make_unique<QWidget>();

    managingGrid = std::make_unique<QGridLayout>(managingWidget.get());
    managingGrid->setRowStretch(0, 1);

    managingInfoLayout = std::make_unique<QVBoxLayout>();
    managingInfoLayout->setContentsMargins(0, 0, 0, 16);

    managingGrid->addLayout(managingInfoLayout.get(), 1, 0);

    managingSendMessageButton = std::make_unique<QPushButton>(tr("Send message"));
    managingViewInboxButton = std::make_unique<QPushButton>(tr("View inbox"));

    managingActionButtonsLayout = std::make_unique<QHBoxLayout>();
    managingActionButtonsLayout->addWidget(managingSendMessageButton.get());
    managingActionButtonsLayout->addStretch(1);
    managingActionButtonsLayout->addWidget(managingViewInboxButton.get());
    managingActionButtonsLayout->setContentsMargins(0, 0, 0, 64);

    managingGrid->addLayout(managingActionButtonsLayout.get(), 2, 0);
}