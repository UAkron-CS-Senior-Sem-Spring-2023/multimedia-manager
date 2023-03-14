#pragma once

#include <QGridLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <vector>

#include "send-message-wizard.hpp"

class MainWindow;

class AccountInfoWidget : public QWidget {
    public:
        AccountInfoWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
        ~AccountInfoWidget();
    private:
        MainWindow* mainWindow;

        QGridLayout managingGrid;

        QStackedLayout managingTypeStackedLayout;

        QWidget managingAccountInfoWidget;
        QGridLayout managingAccountInfoLayout;
        QLabel managingAccountInfoLabel;
        QWidget managingAccountInfoBackgroundWidget;
        QGridLayout managingAccountInfoBackgroundLayout;
        QWidget* currentAccountInfoWidget = nullptr;
        QWidget noAccountInfoWidget;

        QWidget managingAccountGroupInfoWidget;
        QGridLayout managingAccountGroupInfoLayout;
        QLabel managingAccountGroupInfoLabel;
        QWidget managingAccountGroupInfoBackgroundWidget;
        QGridLayout managingAccountGroupInfoBackgroundLayout;
        QWidget* currentAccountGroupInfoWidget = nullptr;
        QWidget noAccountGroupInfoWidget;

        QStackedLayout managingActionButtonsStackedLayout;

        QWidget managingAccountActionButtonsWidget;
        QHBoxLayout managingAccountActionButtonsLayout;
        QPushButton managingAccountSendMessageButton;
        QPushButton managingAccountViewInboxButton;

        QWidget managingAccountGroupActionButtonsWidget;
        QHBoxLayout managingAccountGroupActionButtonsLayout;
        QPushButton managingAccountGroupSendMessageButton;
        QPushButton managingAccountGroupViewInboxButton;

        SendMessageWizard accountSendMessageWizard;
        SendMessageWizard accountGroupSendMessageWizard;
};