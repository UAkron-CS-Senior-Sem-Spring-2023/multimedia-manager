#include "main-window.hpp"



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    windowWidget = std::make_unique<QWidget>();
    windowGrid = std::make_unique<QGridLayout>(windowWidget.get());

    constructManageableWidget();
    windowGrid->addWidget(manageableWidget.get(), 0, 0);

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
    manageableTypeLayout = std::make_unique<QHBoxLayout>();
    manageableTypeGroupBox = std::make_unique<QGroupBox>(tr("Current Managed Type"));

    manageableAccountsButton = std::make_unique<QPushButton>(tr("Managed Accounts"));
    manageableAccountsButton->setMaximumSize(160, 35);
    manageableAccountGroupsButton = std::make_unique<QPushButton>(tr("Managed Account Groups"));
    manageableAccountGroupsButton->setMaximumSize(200, 35);
    manageableTypeLayout->addWidget(manageableAccountsButton.get());
    manageableTypeLayout->addWidget(manageableAccountGroupsButton.get());
    manageableTypeLayout->addStretch(1);
    manageableTypeGroupBox->setLayout(manageableTypeLayout.get());

    manageableGrid->addWidget(manageableTypeGroupBox.get(), 2, 0);
}