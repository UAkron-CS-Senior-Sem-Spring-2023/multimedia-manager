#include "add-account-vendor-wizard-page.hpp"

#include "add-account-wizard.hpp"

AddAccountVendorWizardPage::AddAccountVendorWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : QWizardPage(parent),
    addAccountWizard(addAccountWizard)
{}

void AddAccountVendorWizardPage::construct() {
    setLayout(&accountSpecificsStackedLayout);
    for (const auto& pageMapPair : addAccountWizard->accountSpecificPageMap()) {
        accountSpecificsStackedLayout.insertWidget(pageMapPair.first, pageMapPair.second.page.get());
        connect(pageMapPair.second.page.get(), &ValidatableAddAccountPage::completeChanged, this, &AddAccountVendorWizardPage::completeChanged);
    }

    connect(addAccountWizard, &AddAccountWizard::vendorSelectionChanged, this, &AddAccountVendorWizardPage::setVendorSelection);
}

void AddAccountVendorWizardPage::setVendorSelection(AccountManager::AccountVendor vendor) {
    accountSpecificsStackedLayout.setCurrentIndex(vendor);
}

bool AddAccountVendorWizardPage::isComplete() const {
    return addAccountWizard->accountSpecificPageMap().at(addAccountWizard->vendorSelection()).page->isComplete();
}