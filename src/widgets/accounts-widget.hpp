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

#include "../account-manager.hpp"

class AccountsWidget : public QWidget {
    public:
        AccountsWidget(QWidget* parent = nullptr);

        void updateManagingType(AccountManager::ManagingType managingType);

    private:
        QGridLayout manageableGrid;

        QStackedLayout managingTypeStackedLayout;
        QLabel managingAccountsLabel;
        QLabel managingAccountGroupsLabel;

        QWidget manageableNamesWidget;
        QVBoxLayout manageableNamesLayout;
        std::vector<QLabel> manageableNames;

        QStackedLayout modifyManageablesButtonsStackedLayout;

        QWidget modifyManageableAccountWidget;
        QHBoxLayout modifyManageableAccountLayout;
        QPushButton addManageableAccountButton;
        QPushButton removeManageableAccountButton;

        QWidget modifyManageableAccountGroupWidget;
        QHBoxLayout modifyManageableAccountGroupLayout;
        QPushButton addManageableAccountGroupButton;
        QPushButton removeManageableAccountGroupButton;
};