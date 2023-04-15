#pragma once

#include <memory>
#include <vector>

#include <QWizard>
#include <QLabel>
#include <QStackedLayout>

#include "account-type-selection-wizard-page.hpp"
#include "add-account-vendor-wizard-page.hpp"
#include "validatable-add-account-page.hpp"

class AddAccountWizard : public QWizard {
    Q_OBJECT
    
    public:
        AddAccountWizard(QWidget* parent = nullptr);

        struct AccountSpecificPageInfo {
            QRadioButton* radioButton;
            ValidatableAddAccountPage* page;
        };

        void setVendorSelection(AccountManager::AccountVendor vendor);
        AccountManager::AccountVendor vendorSelection() const;

        const std::unordered_map<AccountManager::AccountVendor, AccountSpecificPageInfo>& accountSpecificPageMap();

        void clearAccountsToAdd();
        void addSourceAccountToAdd(std::unique_ptr<SourceAccount> sourceAccount);
        void addRecipientAccountToAdd(std::unique_ptr<RecipientAccount> recipientAccount);
        void addDualAccountToAdd(std::unique_ptr<DualAccount> dualAccount);
    signals:
        void vendorSelectionChanged(AccountManager::AccountVendor vendor);
    private:
        AccountTypeSelectionWizardPage accountTypeSelectionWizardPage;
        AddAccountVendorWizardPage addAccountVendorWizardPage;

        AccountManager::AccountVendor vendorSelection_;
        
        std::unordered_map<AccountManager::AccountVendor, AccountSpecificPageInfo> accountSpecificPageMap_;

        std::vector<std::unique_ptr<SourceAccount>> sourceAccountsToAdd;
        std::vector<std::unique_ptr<RecipientAccount>> recipientAccountsToAdd;
        std::vector<std::unique_ptr<DualAccount>> dualAccountsToAdd;

        void initializeAccountSpecificPageMap();
};