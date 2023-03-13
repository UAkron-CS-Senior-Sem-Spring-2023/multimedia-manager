#pragma once

#include <memory>
#include <list>
#include <unordered_set>

#include <QWizard>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class GetEmailRecipientsWizard : public QWizard {
    Q_OBJECT

    public:
        GetEmailRecipientsWizard(QWidget* parent = nullptr);
    signals:
        void gotEmailRecipients(std::list<std::string> emailRecipients);
    private:
        QWizardPage getRecipientsWizardPage;
        QGridLayout getRecipientsLayout;
        QLabel explanationLabel;
        QLabel recipientRemovalLabel;
        QWidget recipientRemovalButtonsWidget;
        QScrollArea recipientRemovalButtonsScrollArea;
        QGridLayout recipientRemovalButtonsLayout;
        std::list<std::pair<std::size_t, std::unique_ptr<QPushButton>>> recipientRemovalButtons;
        std::unordered_set<std::string> recipientsAccountedFor;
        QLabel recipientEnteringLabel;
        QLineEdit recipientEnteringTextLine;
        QPushButton recipientEnteringButton;

        std::size_t uniqueIdentifier = 0;
};