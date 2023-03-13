#include "account-type-selection-wizard-page.hpp"

#include "add-account-wizard.hpp"

AccountTypeSelectionWizardPage::AccountTypeSelectionWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : QWizardPage(parent),
    addAccountWizard(addAccountWizard),
    wizardIntroductionLabel(tr("This wizard will let you add an account to your set of managed accounts.")),
    accountTypePromptingLabel(tr("Select the type of account you would like to add.")),
    accountTypeSelectionGroupBox(tr("Account types"))
{}

void AccountTypeSelectionWizardPage::construct() {
    setLayout(&accountTypeSelectionLayout);
    setTitle("Add an account");

    int workingRow = 0;
    
    wizardIntroductionLabel.setWordWrap(true);
    accountTypeSelectionLayout.addWidget(&wizardIntroductionLabel, workingRow++, 0);

    accountTypeSelectionLayout.setRowStretch(workingRow++, 1);

    accountTypePromptingLabel.setWordWrap(true);
    accountTypeSelectionLayout.addWidget(&accountTypePromptingLabel, workingRow++, 0);


    for (const auto& pageMapPair : addAccountWizard->accountSpecificPageMap()) {
        accountTypeSelectionGroupBoxLayout.addWidget(pageMapPair.second.radioButton.get());
        accountTypeSelectionRadioButtons.push_back(pageMapPair.second.radioButton.get());
        connect(pageMapPair.second.radioButton.get(), &QRadioButton::toggled, this, &AccountTypeSelectionWizardPage::completeChanged);
        AccountManager::AccountVendor vendor = pageMapPair.first;
        connect(pageMapPair.second.radioButton.get(), &QRadioButton::toggled, [this, vendor](bool checked){
            if (checked) {
                emit vendorSelectionChanged(vendor);
            }
        });
    }
    accountTypeSelectionGroupBox.setLayout(&accountTypeSelectionGroupBoxLayout);
    accountTypeSelectionLayout.addWidget(&accountTypeSelectionGroupBox, workingRow++, 0);

}

bool AccountTypeSelectionWizardPage::isComplete() const {
    bool completed = false;
    for (const auto* radioButton : accountTypeSelectionRadioButtons) {
        completed |= radioButton->isChecked();
    }
    return completed;
}