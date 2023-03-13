#include "get-email-recipients-wizard.hpp"

#include <QPalette>

GetEmailRecipientsWizard::GetEmailRecipientsWizard(QWidget* parent)
    : QWizard(parent),
    explanationLabel(tr("Through this dialog enter the names of the recipients you want this mailing account to go to.")),
    recipientRemovalLabel(tr("Press on the recipients below to remove them from the list once added.")),
    recipientRemovalButtonsLayout(&recipientRemovalButtonsWidget),
    recipientEnteringLabel(tr("Enter a recipient's email address in the field below then press the 'Add Recipient' button to add it to the list.")),
    recipientEnteringButton(tr("Add Recipient"))

{
    int workingRow = 0;
    explanationLabel.setContentsMargins(0, 0, 0, 18);
    getRecipientsLayout.addWidget(&explanationLabel, workingRow++, 0, 1, 2);
    getRecipientsLayout.addWidget(&recipientRemovalLabel, workingRow++, 0, 1, 2);
    recipientRemovalButtonsWidget.setBackgroundRole(QPalette::Base);
    recipientRemovalButtonsWidget.setAutoFillBackground(true);
    getRecipientsLayout.setRowStretch(workingRow, 1);
    recipientRemovalButtonsScrollArea.setWidget(&recipientRemovalButtonsWidget);
    recipientRemovalButtonsScrollArea.setWidgetResizable(true);
    getRecipientsLayout.addWidget(&recipientRemovalButtonsScrollArea, workingRow++, 0, 1, 2);
    getRecipientsLayout.addWidget(&recipientEnteringLabel, workingRow++, 0, 1, 2);
    getRecipientsLayout.addWidget(&recipientEnteringTextLine, workingRow, 0);
    connect(&recipientEnteringButton, &QPushButton::clicked, [this](){
        std::string recipientEntered = recipientEnteringTextLine.text().toStdString();
        if (recipientEntered == "" || recipientsAccountedFor.count(recipientEntered) != 0) {
            return;
        }

        recipientsAccountedFor.insert(recipientEntered);

        recipientRemovalButtons.push_back({
            recipientRemovalButtons.size(),
            std::make_unique<QPushButton>(QString::fromStdString(recipientEntered))
        });
        auto selfIterator = std::prev(recipientRemovalButtons.end());
        recipientRemovalButtonsLayout.addWidget(selfIterator->second.get(), selfIterator->first, 0);
        // upon clicking one of the removal buttons, remove itself and decrease the rows of everything after it by one
        connect(selfIterator->second.get(), &QPushButton::clicked, [this, selfIterator]() {
            recipientsAccountedFor.erase(selfIterator->second->text().toStdString());

            for (auto begin = std::next(selfIterator); begin != recipientRemovalButtons.end(); ++begin) {
                --begin->first;
                recipientRemovalButtonsLayout.addWidget(begin->second.get(), begin->first, 0);
            }
            recipientRemovalButtons.erase(selfIterator);

            recipientRemovalButtonsLayout.setRowStretch(recipientRemovalButtons.size() + 1, 0);
            recipientRemovalButtonsLayout.setRowStretch(recipientRemovalButtons.size(), 1);
        });
        recipientRemovalButtonsLayout.setRowStretch(recipientRemovalButtons.size(), 0);
        recipientRemovalButtonsLayout.setRowStretch(recipientRemovalButtons.size() + 1, 1);
    });
    getRecipientsLayout.addWidget(&recipientEnteringButton, workingRow++, 1);

    getRecipientsWizardPage.setLayout(&getRecipientsLayout);

    addPage(&getRecipientsWizardPage);

    connect(this, &GetEmailRecipientsWizard::finished, [this](){
        std::list<std::string> emailRecipients;
        for (const auto& recipientRemovalButton : recipientRemovalButtons) {
            emailRecipients.push_back(recipientRemovalButton.second->text().toStdString());
        }

        emit gotEmailRecipients(std::move(emailRecipients));
        
        recipientRemovalButtons.clear();
        recipientsAccountedFor.clear();
    });
}