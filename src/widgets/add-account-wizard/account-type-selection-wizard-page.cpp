#include "account-type-selection-wizard-page.hpp"

#include "add-account-wizard.hpp"

AccountTypeSelectionWizardPage::AccountTypeSelectionWizardPage(AddAccountWizard* addAccountWizard, QWidget* parent)
    : QWizardPage(parent),
    addAccountWizard(addAccountWizard)
{}

void AccountTypeSelectionWizardPage::construct() {
    auto* accountTypeSelectionLayout = new QGridLayout;
    setLayout(accountTypeSelectionLayout);
    setTitle("Add an account");

    int workingRow = 0;
    
    auto* wizardIntroductionLabel = new QLabel(tr("This wizard will let you add an account to your set of managed accounts."));
    wizardIntroductionLabel->setWordWrap(true);
    accountTypeSelectionLayout->addWidget(wizardIntroductionLabel, workingRow++, 0);

    accountTypeSelectionLayout->setRowStretch(workingRow++, 1);

    auto* accountTypePromptingLabel = new QLabel(tr("Select the type of account you would like to add."));
    accountTypePromptingLabel->setWordWrap(true);
    accountTypeSelectionLayout->addWidget(accountTypePromptingLabel, workingRow++, 0);

    auto* accountTypeSelectionGroupBoxLayout = new QVBoxLayout;
    for (const auto& pageMapPair : addAccountWizard->accountSpecificPageMap()) {
        accountTypeSelectionGroupBoxLayout->addWidget(pageMapPair.second.radioButton);
        accountTypeSelectionRadioButtons.push_back(pageMapPair.second.radioButton);
        connect(pageMapPair.second.radioButton, &QRadioButton::toggled, this, &AccountTypeSelectionWizardPage::completeChanged);
        AccountManager::AccountVendor vendor = pageMapPair.first;
        connect(pageMapPair.second.radioButton, &QRadioButton::toggled, [this, vendor](bool checked){
            if (checked) {
                emit vendorSelectionChanged(vendor);
            }
        });
    }

    auto* accountTypeSelectionGroupBox = new QGroupBox(tr("Account types"));
    accountTypeSelectionGroupBox->setLayout(accountTypeSelectionGroupBoxLayout);
    accountTypeSelectionLayout->addWidget(accountTypeSelectionGroupBox, workingRow++, 0);

}

bool AccountTypeSelectionWizardPage::isComplete() const {
    bool completed = false;
    for (const auto* radioButton : accountTypeSelectionRadioButtons) {
        completed |= radioButton->isChecked();
    }
    return completed;
}