#pragma once

#include <QWizardPage>
#include <QStackedLayout>

#include "accounts/account-manager.hpp"

class AddAccountWizard;

class AddAccountVendorWizardPage : public QWizardPage {
    public:
        AddAccountVendorWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent = nullptr);
        // must be constructed after AddAccountWizard is finished initializing, so there is a separate constructor
        void construct();

        void setVendorSelection(AccountManager::AccountVendor vendor);

        bool isComplete() const override;
    private:
        AddAccountWizard* addAccountWizard;

        QStackedLayout accountSpecificsStackedLayout;
};