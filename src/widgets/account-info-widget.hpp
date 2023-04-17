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
#include "view-inbox-dialog.hpp"

class MainWindow;

class AccountInfoWidget : public QWidget {
    public:
        AccountInfoWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
        ~AccountInfoWidget();
    private:
        MainWindow* mainWindow;

        QWidget* currentAccountInfoWidget = nullptr;
        QWidget* currentAccountGroupInfoWidget = nullptr;

        SendMessageWizard accountSendMessageWizard;
        SendMessageWizard accountGroupSendMessageWizard;
        ViewInboxDialog viewInboxDialog;
};