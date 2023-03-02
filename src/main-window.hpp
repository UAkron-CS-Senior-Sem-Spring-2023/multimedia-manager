#pragma once

#include <QMainWindow>

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

#include <memory>
#include <vector>
#include <unordered_map>

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);
    private:
        enum ManagingType {
            ACCOUNTS,
            ACCOUNT_GROUPS
        };
        ManagingType managingType = ACCOUNTS;

        QWidget windowWidget;
        QGridLayout windowGrid;

        // lefthand side of application
        // contains list of groups of accounts/accounts and buttons to switch management style
        void layoutManageableWidget();
        QWidget manageableWidget;
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

        QGroupBox manageableTypeGroupBox;
        QHBoxLayout manageableTypeLayout;
        void manageableAccountsButtonClicked();
        QPushButton manageableAccountsButton;
        void manageableAccountGroupsButtonClicked();
        QPushButton manageableAccountGroupsButton;

        // righthand side of application
        // contains the selected group of accounts/account and buttons to perform actions on that group/account
        void layoutManagingWidget();
        QWidget managingWidget;
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

        void updateManagingType(ManagingType type);
};