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
#include "add-account-group-wizard.hpp"

class MainWindow;

class AccountsWidget : public QWidget {
    public:
        AccountsWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
    private:
        MainWindow* mainWindow;
        AddAccountWizard addAccountWizard;
        AddAccountGroupWizard addAccountGroupWizard;

        std::vector<std::unique_ptr<QPushButton>> accountNames;
        std::vector<std::unique_ptr<QPushButton>> accountGroupNames;
};