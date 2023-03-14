#include "add-account-group-wizard.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>

#include "accounts/account-manager.hpp"

AddAccountGroupWizard::AddAccountGroupWizard(QWidget* parent)
    : QWizard(parent)
{
    QGridLayout* addAccountGroupLayout = new QGridLayout;
    int workingRow = 0;

    QLabel* accountGroupDisclaimer = new QLabel(tr("Please note: Any account group created can only perform the actions that are shared between all accounts "
        "within the group, (i.e. only if all accounts can send a message will the account group be able to send a message."
    ));
    accountGroupDisclaimer->setWordWrap(true);
    addAccountGroupLayout->addWidget(accountGroupDisclaimer, workingRow++, 0);
    addAccountGroupLayout->addWidget(new QLabel(tr("Press the buttons below to transfer accounts into the new account group to create.")), workingRow++, 0);
    addAccountGroupLayout->addWidget(new QLabel(tr("Accounts not included in group to make:")), workingRow++, 0);

    QWidget* unincludedAccountsWidget = new QWidget;
    unincludedAccountsWidget->setBackgroundRole(QPalette::Base);
    unincludedAccountsWidget->setAutoFillBackground(true);
    QGridLayout* unincludedAccountsLayout = new QGridLayout(unincludedAccountsWidget);

    QScrollArea* unincludedAccountsWidgetScrollArea = new QScrollArea;
    unincludedAccountsWidgetScrollArea->setWidget(unincludedAccountsWidget);
    unincludedAccountsWidgetScrollArea->setWidgetResizable(true);

    addAccountGroupLayout->addWidget(unincludedAccountsWidgetScrollArea, workingRow++, 0);

    addAccountGroupLayout->addWidget(new QLabel(tr("Accounts included in group to make:")), workingRow++, 0);

    QWidget* includedAccountsWidget = new QWidget;
    includedAccountsWidget->setBackgroundRole(QPalette::Base);
    includedAccountsWidget->setAutoFillBackground(true);
    QGridLayout* includedAccountsLayout = new QGridLayout(includedAccountsWidget);

    QScrollArea* includedAccountsWidgetScrollArea = new QScrollArea;
    includedAccountsWidgetScrollArea->setWidget(includedAccountsWidget);
    includedAccountsWidgetScrollArea->setWidgetResizable(true);

    connect(this, &AddAccountGroupWizard::accountsUpdated, [this, unincludedAccountsLayout, includedAccountsLayout] () {
        for (auto& unincludedButton : unincludedButtons) {
            unincludedAccountsLayout->removeWidget(unincludedButton);
            delete unincludedButton;
        }
        for (auto& includedButton : includedButtons) {
            includedAccountsLayout->removeWidget(includedButton);
            delete includedButton;
        }
        unincludedButtons.clear();
        includedButtons.clear();

        for (auto* account : unincludedAccounts) {
            std::size_t selfRow = unincludedButtons.size();
            unincludedButtons.push_back(new QPushButton(QString::fromStdString(account->name())));

            unincludedAccountsLayout->addWidget(unincludedButtons.back(), selfRow, 0);
            connect(unincludedButtons.back(), &QPushButton::clicked, [this, account]() {
                this->flipButtonState(account);
            });
        }
        
        for (auto* account : includedAccounts) {
            std::size_t selfRow = includedButtons.size();
            includedButtons.push_back(new QPushButton(QString::fromStdString(account->name())));

            includedAccountsLayout->addWidget(includedButtons.back(), selfRow, 0);
            connect(includedButtons.back(), &QPushButton::clicked, [this, account]() {
                this->flipButtonState(account);
            });
        }
    });

    updateAccounts(AccountManager::singleton().accounts());
    connect(&AccountManager::singleton(), &AccountManager::onAccountsChanged, this, &AddAccountGroupWizard::updateAccounts);

    connect(this, &AddAccountGroupWizard::finished, [this]() {
        if (includedAccounts.size() > 0) {
            AccountManager::singleton().addAccountGroup(std::make_unique<AccountGroup>(includedAccounts));
        }
        includedAccounts.clear();
        unincludedAccounts.clear();

        updateAccounts(AccountManager::singleton().accounts());
    });

    addAccountGroupLayout->addWidget(includedAccountsWidgetScrollArea);

    QWizardPage* addAccountGroupWizardPage = new QWizardPage;
    addAccountGroupWizardPage->setLayout(addAccountGroupLayout);

    addPage(addAccountGroupWizardPage);
}

void AddAccountGroupWizard::updateAccounts(const SetList<Account*>& newAccounts) {
    SetList<Account*> newUnincludedAccounts;
    SetList<Account*> newIncludedAccounts;
    for (auto* account : newAccounts) {
        if (includedAccounts.count(account) == 0) {
            newUnincludedAccounts.push_back(account);
        } else {
            newIncludedAccounts.push_back(account);
        }
    }

    std::swap(unincludedAccounts, newUnincludedAccounts);
    std::swap(includedAccounts, newIncludedAccounts);
    emit accountsUpdated();
}

void AddAccountGroupWizard::flipButtonState(Account* account) {
    bool isIncluded = includedAccounts.count(account) != 0;
    if (isIncluded) {
        unincludedAccounts.push_back(account);
        includedAccounts.erase(account);
    } else {
        includedAccounts.push_back(account);
        unincludedAccounts.erase(account);
    }
    emit accountsUpdated();
}