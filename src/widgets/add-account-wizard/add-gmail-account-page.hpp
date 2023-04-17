#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QPalette>

#include "validatable-add-account-page.hpp"

class AddAccountWizard;

class AddGmailAccountPage : public ValidatableAddAccountPage {
    public:
        AddGmailAccountPage(AddAccountWizard* addAccountWizard, QWidget* parent = nullptr);

        bool isComplete() const override;
    private:
        AddAccountWizard* addAccountWizard;

        std::string oauthBearer;
        std::string refreshToken;
        std::string gmail;
        bool verified = false;
};