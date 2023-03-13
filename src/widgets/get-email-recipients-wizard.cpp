#include "get-email-recipients-wizard.hpp"

#include <QPalette>

GetEmailRecipientsWizard::GetEmailRecipientsWizard(QWidget* parent)
    : QWizard(parent)
{
    int workingRow = 0;
    QGridLayout* getRecipientsLayout = new QGridLayout;

    QLabel* explanationLabel = new QLabel(tr("Through this dialog enter the names of the recipients you want this mailing account to go to."));
    explanationLabel->setContentsMargins(0, 0, 0, 18);
    getRecipientsLayout->addWidget(explanationLabel, workingRow++, 0, 1, 2);
    getRecipientsLayout->addWidget(new QLabel(tr("Press on the recipients below to remove them from the list once added.")), workingRow++, 0, 1, 2);
    getRecipientsLayout->setRowStretch(workingRow, 1);

    QWidget* recipientRemovalButtonsWidget = new QWidget;
    recipientRemovalButtonsWidget->setBackgroundRole(QPalette::Base);
    recipientRemovalButtonsWidget->setAutoFillBackground(true);
    QGridLayout* recipientRemovalButtonsLayout = new QGridLayout(recipientRemovalButtonsWidget);

    QScrollArea* recipientRemovalButtonsScrollArea = new QScrollArea;
    recipientRemovalButtonsScrollArea->setWidget(recipientRemovalButtonsWidget);
    recipientRemovalButtonsScrollArea->setWidgetResizable(true);

    getRecipientsLayout->addWidget(recipientRemovalButtonsWidget, workingRow++, 0, 1, 2);
    getRecipientsLayout->addWidget(new QLabel(tr("Enter a recipient's email address in the field below then press the 'Add Recipient' button to add it to the list.")), workingRow++, 0, 1, 2);
    QLineEdit* recipientEnteringTextLine = new QLineEdit;
    getRecipientsLayout->addWidget(recipientEnteringTextLine, workingRow, 0);
    QPushButton* recipientEnteringButton = new QPushButton(tr("Add Recipient"));
    connect(recipientEnteringButton, &QPushButton::clicked, [this, recipientEnteringTextLine, recipientRemovalButtonsLayout](){
        std::string recipientEntered = recipientEnteringTextLine->text().toStdString();
        if (recipientEntered == "" || recipientsAccountedFor.count(recipientEntered) != 0) {
            return;
        }

        recipientsAccountedFor.insert(recipientEntered);

        recipientRemovalButtons.push_back({
            recipientRemovalButtons.size(),
            new QPushButton(QString::fromStdString(recipientEntered))
        });
        auto selfIterator = std::prev(recipientRemovalButtons.end());
        recipientRemovalButtonsLayout->addWidget(selfIterator->second, selfIterator->first, 0);
        // upon clicking one of the removal buttons, remove itself and decrease the rows of everything after it by one
        connect(selfIterator->second, &QPushButton::clicked, [this, recipientRemovalButtonsLayout, selfIterator]() {
            selfIterator->second->setParent(nullptr);
            recipientsAccountedFor.erase(selfIterator->second->text().toStdString());

            for (auto begin = std::next(selfIterator); begin != recipientRemovalButtons.end(); ++begin) {
                --begin->first;
                recipientRemovalButtonsLayout->addWidget(begin->second, begin->first, 0);
            }
            recipientRemovalButtons.erase(selfIterator);

            recipientRemovalButtonsLayout->setRowStretch(recipientRemovalButtons.size() + 1, 0);
            recipientRemovalButtonsLayout->setRowStretch(recipientRemovalButtons.size(), 1);
        });
        recipientRemovalButtonsLayout->setRowStretch(recipientRemovalButtons.size(), 0);
        recipientRemovalButtonsLayout->setRowStretch(recipientRemovalButtons.size() + 1, 1);
    });
    getRecipientsLayout->addWidget(recipientEnteringButton, workingRow++, 1);


    QWizardPage* getRecipientsWizardPage = new QWizardPage;
    getRecipientsWizardPage->setLayout(getRecipientsLayout);

    addPage(getRecipientsWizardPage);

    connect(this, &GetEmailRecipientsWizard::finished, [this, recipientRemovalButtonsLayout](){
        std::list<std::string> emailRecipients;
        for (const auto& recipientRemovalButton : recipientRemovalButtons) {
            emailRecipients.push_back(recipientRemovalButton.second->text().toStdString());
        }

        emit gotEmailRecipients(std::move(emailRecipients));

        recipientRemovalButtons.clear();
        recipientsAccountedFor.clear();
    });
}