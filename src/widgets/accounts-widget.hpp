#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

#include <vector>

#include "add-account-wizard/add-account-wizard.hpp"

class MainWindow;

class AccountsWidget : public QWidget {
    public:
        AccountsWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
    private:
        MainWindow* mainWindow;

        QGridLayout manageableGrid;

        QStackedLayout managingTypeStackedLayout;
        QLabel managingAccountsLabel;
        QLabel managingAccountGroupsLabel;

        QStackedLayout namesStackedLayout;

        QWidget accountNamesWidget;
        QGridLayout accountNamesLayout;
        std::vector<std::unique_ptr<QPushButton>> accountNames;

        QWidget accountGroupNamesWidget;
        QGridLayout accountGroupNamesLayout;
        std::vector<std::unique_ptr<QPushButton>> accountGroupNames;

        QStackedLayout modifyManageablesButtonsStackedLayout;

        QWidget modifyManageableAccountWidget;
        QHBoxLayout modifyManageableAccountLayout;
        QPushButton addManageableAccountButton;
        QPushButton removeManageableAccountButton;

        QWidget modifyManageableAccountGroupWidget;
        QHBoxLayout modifyManageableAccountGroupLayout;
        QPushButton addManageableAccountGroupButton;
        QPushButton removeManageableAccountGroupButton;

        void addAccountClicked();
};