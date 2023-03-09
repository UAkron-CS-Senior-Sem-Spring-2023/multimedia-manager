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

#include "../account-manager.hpp"

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);
    private:
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

        void updateManagingType(AccountManager::ManagingType managingType);
};