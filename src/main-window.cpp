#include "main-window.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    windowWidget = std::make_unique<QWidget>();
    windowGrid = std::make_unique<QGridLayout>(windowWidget.get());
    windowGrid->setRowStretch(0, 1);
    windowGrid->setColumnStretch(0, 2);
    
    // set up managed list
    managedLayout = std::make_unique<QVBoxLayout>();
    managedLayout->setContentsMargins(0, 0, 0, 16);

    windowGrid->addLayout(managedLayout.get(), 1, 0);

    // set up managed account type buttons
    managedTypeLayout = std::make_unique<QHBoxLayout>();
    managedTypeGroupBox = std::make_unique<QGroupBox>(tr("Current Managed Type"));

    managedAccountsButton = std::make_unique<QPushButton>(tr("Managed Accounts"));
    managedAccountsButton->setMaximumSize(160, 35);
    managedAccountGroupsButton = std::make_unique<QPushButton>(tr("Managed Account Groups"));
    managedAccountGroupsButton->setMaximumSize(200, 35);
    managedTypeLayout->addWidget(managedAccountsButton.get());
    managedTypeLayout->addWidget(managedAccountGroupsButton.get());
    managedTypeLayout->addStretch(1);
    managedTypeGroupBox->setLayout(managedTypeLayout.get());

    windowGrid->addWidget(managedTypeGroupBox.get(), 2, 0);

    setCentralWidget(windowWidget.get());
}