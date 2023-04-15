#include "add-account-vendor-wizard-page.hpp"

#include "add-account-wizard.hpp"

AddAccountVendorWizardPage::AddAccountVendorWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : QWizardPage(parent),
    addAccountWizard(addAccountWizard)
{}

void AddAccountVendorWizardPage::construct() {
    auto* accountSpecificsStackedLayout = new QStackedLayout;
    setLayout(accountSpecificsStackedLayout);
    for (const auto& pageMapPair : addAccountWizard->accountSpecificPageMap()) {
        accountSpecificsStackedLayout->insertWidget(pageMapPair.first, pageMapPair.second.page);
        connect(pageMapPair.second.page, &ValidatableAddAccountPage::completeChanged, this, &AddAccountVendorWizardPage::completeChanged);
    }

    connect(addAccountWizard, &AddAccountWizard::vendorSelectionChanged, [accountSpecificsStackedLayout](AccountManager::AccountVendor vendor){
        accountSpecificsStackedLayout->setCurrentIndex(vendor);
    });
}

bool AddAccountVendorWizardPage::isComplete() const {
    return addAccountWizard->accountSpecificPageMap().at(addAccountWizard->vendorSelection()).page->isComplete();
}