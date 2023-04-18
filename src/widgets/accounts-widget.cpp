#include "accounts-widget.hpp"

#include "main-window.hpp"

AccountsWidget::AccountsWidget(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent),
    mainWindow(mainWindow)
{
    int workingRow = 0;

    auto* manageableGrid = new QGridLayout(this);
    manageableGrid->setColumnStretch(1, 1);

    auto* managingTypeStackedLayout = new QStackedLayout;
    managingTypeStackedLayout->insertWidget(AccountManager::ACCOUNTS, new QLabel(tr("Managed Accounts")));
    managingTypeStackedLayout->insertWidget(AccountManager::ACCOUNT_GROUPS, new QLabel(tr("Managed Account Groups")));
    managingTypeStackedLayout->setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, managingTypeStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid->addLayout(managingTypeStackedLayout, workingRow++, 0);

    // set up manageable list
    auto* accountNamesWidget = new QWidget;
    accountNamesWidget->setBackgroundRole(QPalette::Base);
    accountNamesWidget->setAutoFillBackground(true);

    auto* accountNamesLayout = new QGridLayout(accountNamesWidget);
    connect(&AccountManager::singleton(), &AccountManager::onAccountsChanged, [this, accountNamesLayout](const SetList<Account*>& accounts) {
        for (auto& widget : accountNames) {
            accountNamesLayout->removeWidget(widget.get());
        }
        accountNames.clear();
        
        int workingRow = 0;
        for (auto* account : accounts) {
            accountNames.push_back(std::make_unique<QPushButton>(QString::fromStdString(account->name())));
            accountNamesLayout->addWidget(accountNames.back().get(), workingRow++, 0);
            connect(accountNames.back().get(), &QPushButton::clicked, [this, account](){
                this->mainWindow->changeSelectedAccount(account);
            });
        }
        accountNamesLayout->setRowStretch(workingRow, 1);
    });
    auto* namesStackedLayout = new QStackedLayout;
    namesStackedLayout->insertWidget(AccountManager::ACCOUNTS, accountNamesWidget);

    auto* accountGroupNamesWidget = new QWidget;
    accountGroupNamesWidget->setBackgroundRole(QPalette::Base);
    accountGroupNamesWidget->setAutoFillBackground(true);

    auto* accountGroupNamesLayout = new QGridLayout(accountGroupNamesWidget);
    connect(&AccountManager::singleton(), &AccountManager::onAccountGroupsChanged, [this, accountGroupNamesLayout](const SetList<AccountGroup*>& accountGroups) {
        for (auto& widget : accountGroupNames) {
            accountGroupNamesLayout->removeWidget(widget.get());
        }
        accountGroupNames.clear();

        int workingRow = 0;
        for (auto* accountGroup : accountGroups) {
            accountGroupNames.push_back(std::make_unique<QPushButton>(QString::fromStdString(accountGroup->name())));
            accountGroupNamesLayout->addWidget(accountGroupNames.back().get(), workingRow++, 0);
            connect(accountGroupNames.back().get(), &QPushButton::clicked, [this, accountGroup](){
                this->mainWindow->changeSelectedAccountGroup(accountGroup);
            });
        }
        accountGroupNamesLayout->setRowStretch(workingRow, 1);
    });
    namesStackedLayout->insertWidget(AccountManager::ACCOUNT_GROUPS, accountGroupNamesWidget);

    manageableGrid->setRowStretch(workingRow, 1);
    connect(mainWindow, &MainWindow::onUpdateManagingType, namesStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid->addLayout(namesStackedLayout, workingRow++, 0, 1, 2);

    auto* modifyManageablesButtonsStackedLayout = new QStackedLayout;

    auto* modifyManageableAccountWidget = new QWidget;
    auto* modifyManageableAccountLayout = new QHBoxLayout(modifyManageableAccountWidget);

    auto* addManageableAccountButton = new QPushButton(tr("Add Account"));
    // set up buttons to modify accounts
    connect(addManageableAccountButton, &QPushButton::clicked, [this]() {
        addAccountWizard.show();
    });
    auto* removeManageableAccountButton = new QPushButton(tr("Remove Selected Account"));
    removeManageableAccountButton->setEnabled(false);
    connect(mainWindow, &MainWindow::selectedAccountChanged, [removeManageableAccountButton](const Account* account) {
        removeManageableAccountButton->setEnabled(account != nullptr);
    });
    connect(removeManageableAccountButton, &QPushButton::clicked, [this]() {
        auto* selectedAccount = this->mainWindow->selectedAccount();
        this->mainWindow->changeSelectedAccount(nullptr);
        AccountManager::singleton().removeAccount(selectedAccount);
    });

    modifyManageableAccountLayout->addWidget(addManageableAccountButton);
    modifyManageableAccountLayout->addStretch(1);
    modifyManageableAccountLayout->addWidget(removeManageableAccountButton);
    modifyManageablesButtonsStackedLayout->insertWidget(AccountManager::ACCOUNTS, modifyManageableAccountWidget);

    auto* modifyManageableAccountGroupWidget = new QWidget;
    auto* modifyManageableAccountGroupLayout = new QHBoxLayout(modifyManageableAccountGroupWidget);

    auto* addManageableAccountGroupButton = new QPushButton(tr("Add Account Group"));
    connect(addManageableAccountGroupButton, &QPushButton::clicked, [this]() {
        addAccountGroupWizard.show();
    });
    auto removeManageableAccountGroupButton = new QPushButton(tr("Remove Selected Account Group"));
    removeManageableAccountGroupButton->setEnabled(false);
    connect(mainWindow, &MainWindow::selectedAccountGroupChanged, [removeManageableAccountGroupButton](const AccountGroup* accountGroup) {
        removeManageableAccountGroupButton->setEnabled(accountGroup != nullptr);
    });
    connect(removeManageableAccountGroupButton, &QPushButton::clicked, [this]() {
        auto* selectedAccountGroup = this->mainWindow->selectedAccountGroup();
        this->mainWindow->changeSelectedAccountGroup(nullptr);
        AccountManager::singleton().removeAccountGroup(selectedAccountGroup);
    });

    modifyManageableAccountGroupLayout->addWidget(addManageableAccountGroupButton);
    modifyManageableAccountGroupLayout->addStretch(1);
    modifyManageableAccountGroupLayout->addWidget(removeManageableAccountGroupButton);

    modifyManageablesButtonsStackedLayout->insertWidget(AccountManager::ACCOUNT_GROUPS, modifyManageableAccountGroupWidget);
    modifyManageablesButtonsStackedLayout->setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    connect(mainWindow, &MainWindow::onUpdateManagingType, modifyManageablesButtonsStackedLayout, &QStackedLayout::setCurrentIndex);
    manageableGrid->addLayout(modifyManageablesButtonsStackedLayout, workingRow++, 0, 1, 2);
}