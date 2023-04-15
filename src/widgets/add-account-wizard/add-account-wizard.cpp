#include "add-account-wizard.hpp"

#include <limits>

#include <QLabel>
#include <QVBoxLayout>

#include "add-gmail-account-page.hpp"

AddAccountWizard::AddAccountWizard(QWidget* parent)
    : QWizard(parent),
    accountTypeSelectionWizardPage(this),
    addAccountVendorWizardPage(this)
{
    initializeAccountSpecificPageMap();
    auto startingPageId = addPage(&accountTypeSelectionWizardPage);
    addPage(&addAccountVendorWizardPage);

    accountTypeSelectionWizardPage.construct();
    addAccountVendorWizardPage.construct();
    connect(&accountTypeSelectionWizardPage, &AccountTypeSelectionWizardPage::vendorSelectionChanged, this, &AddAccountWizard::setVendorSelection);

    connect(this, &AddAccountWizard::finished, [this, startingPageId]() {
        // brings the wizard back to start when finished, (I'm surprised this isn't done automatically)
        setCurrentId(startingPageId);
    });

    connect(this, &AddAccountWizard::accepted, [this, startingPageId]() {

        // moves all accounts created into account manager
        for (auto& sourceAccount : sourceAccountsToAdd) {
            AccountManager::singleton().addSourceAccount(std::move(sourceAccount));
        }
        sourceAccountsToAdd.clear();
        for (auto& recipientAccount : recipientAccountsToAdd) {
            AccountManager::singleton().addRecipientAccount(std::move(recipientAccount));
        }
        recipientAccountsToAdd.clear();
        for (auto& dualAccount : dualAccountsToAdd) {
            AccountManager::singleton().addDualAccount(std::move(dualAccount));
        }
        dualAccountsToAdd.clear();
    });
}

void AddAccountWizard::setVendorSelection(AccountManager::AccountVendor vendor) {
    vendorSelection_ = vendor;
    vendorSelectionChanged(vendorSelection_);
}
AccountManager::AccountVendor AddAccountWizard::vendorSelection() const {
    return vendorSelection_;
}

const std::unordered_map<AccountManager::AccountVendor, AddAccountWizard::AccountSpecificPageInfo>& AddAccountWizard::accountSpecificPageMap() {
    return accountSpecificPageMap_;
}

void AddAccountWizard::clearAccountsToAdd() {
    sourceAccountsToAdd.clear();
    recipientAccountsToAdd.clear();
    dualAccountsToAdd.clear();
}

void AddAccountWizard::addSourceAccountToAdd(std::unique_ptr<SourceAccount> sourceAccount) {
    sourceAccountsToAdd.push_back(std::move(sourceAccount));
}
void AddAccountWizard::addRecipientAccountToAdd(std::unique_ptr<RecipientAccount> recipientAccount) {
    recipientAccountsToAdd.push_back(std::move(recipientAccount));
}
void AddAccountWizard::addDualAccountToAdd(std::unique_ptr<DualAccount> dualAccount) {
    dualAccountsToAdd.push_back(std::move(dualAccount));
}

void AddAccountWizard::initializeAccountSpecificPageMap() {
    accountSpecificPageMap_.insert_or_assign(AccountManager::GMAIL, AccountSpecificPageInfo {
        .radioButton = new QRadioButton(tr("Gmail")),
        .page = new AddGmailAccountPage(this)
    });
}