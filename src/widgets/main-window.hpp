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
    signals:
        void onUpdateManagingType(AccountManager::ManagingType managingType);
    private:
        AccountManager accountManager;

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