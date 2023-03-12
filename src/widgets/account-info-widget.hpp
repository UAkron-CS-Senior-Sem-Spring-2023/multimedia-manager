#pragma once

#include <QGridLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <vector>

class MainWindow;

class AccountInfoWidget : public QWidget {
    public:
        AccountInfoWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
    private:
        MainWindow* mainWindow;

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