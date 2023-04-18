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

        Account* selectedAccount();
        AccountGroup* selectedAccountGroup();
        void changeSelectedAccount(Account* account);
        void changeSelectedAccountGroup(AccountGroup* accountGroup);
    signals:
        void onUpdateManagingType(AccountManager::ManagingType managingType);
        void selectedAccountChanged(Account* account);
        void selectedAccountSourceRecipientTypeChanged(AccountManager::SourceRecipientType sourceRecipientType);
        void selectedAccountGroupChanged(AccountGroup* accountGroup);
        void selectedAccountGroupSourceRecipientTypeChanged(AccountManager::SourceRecipientType sourceRecipientType);
    private:
        Account* selectedAccount_;
        AccountGroup* selectedAccountGroup_;

        void manageableAccountsButtonClicked();
        void manageableAccountGroupsButtonClicked();
};