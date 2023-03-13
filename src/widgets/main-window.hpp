#pragma once

#include <QMainWindow>

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

#include <memory>
#include <vector>
#include <unordered_map>

#include "accounts-widget.hpp"
#include "account-info-widget.hpp"

#include "accounts/account-manager.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);

        AddAccountWizard& addAccountWizard();

        void changeSelectedAccount(const Account* account);
        void changeSelectedAccountGroup(const AccountGroup* accountGroup);
        const Account* selectedAccount() const;
        const AccountGroup* selectedAccountGroup() const;
    signals:
        void onUpdateManagingType(AccountManager::ManagingType managingType);
        void selectedAccountChanged(const Account* account);
        void selectedAccountSourceRecipientTypeChanged(AccountManager::SourceRecipientType sourceRecipientType);
        void selectedAccountGroupChanged(const AccountGroup* accountGroup);
        void selectedAccountGroupSourceRecipientTypeChanged(AccountManager::SourceRecipientType sourceRecipientType);
    private:
        AddAccountWizard addAccountWizard_;

        QWidget windowWidget;
        QGridLayout windowGrid;

        AccountsWidget accountsWidget;
        AccountInfoWidget accountInfoWidget;

        QHBoxLayout manageableTypeLayout;
        QGroupBox manageableTypeGroupBox;
        QHBoxLayout manageableTypeGroupBoxLayout;
        void manageableAccountsButtonClicked();
        QPushButton manageableAccountsButton;
        void manageableAccountGroupsButtonClicked();
        QPushButton manageableAccountGroupsButton;
};