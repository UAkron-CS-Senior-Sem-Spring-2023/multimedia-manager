#pragma once

#include <QGridLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <vector>

#include "../account-manager.hpp"

class AccountInfoWidget : public QWidget {
    public:
        AccountInfoWidget(QWidget* parent = nullptr);

        void updateManagingType(AccountManager::ManagingType type);

    private:
        QGridLayout managingGrid;

        QStackedLayout managingTypeInfoStackedLayout;
        QLabel managingAccountsInfoLabel;
        QLabel managingAccountGroupsInfoLabel;

        QWidget managingInfoWidget;
        QVBoxLayout managingInfoLayout;
        std::vector<QLabel> managingInfoLabels;

        QHBoxLayout managingActionButtonsLayout;
        QPushButton managingSendMessageButton;
        QPushButton managingViewInboxButton;
};