#pragma once

#include <memory>
#include <unordered_map>

#include <QWizardPage>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>

#include "accounts/account-manager.hpp"

class AddAccountWizard;

class AccountTypeSelectionWizardPage : public QWizardPage {
    Q_OBJECT

    public:
        AccountTypeSelectionWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent = nullptr);
        // must be constructed after AddAccountWizard is finished initializing, so there is a separate constructor
        void construct();

        bool isComplete() const override;
    signals:
        void vendorSelectionChanged(AccountManager::AccountVendor vendor);
    private:
        AddAccountWizard* addAccountWizard;

        std::vector<const QRadioButton*> accountTypeSelectionRadioButtons;

        static AccountManager::AccountVendor INITIAL_VENDOR_TYPE;
};