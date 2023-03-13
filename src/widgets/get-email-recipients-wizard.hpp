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

#include <iostream>

class GetEmailRecipientsWizard : public QWizard {
    Q_OBJECT

    public:
        GetEmailRecipientsWizard(QWidget* parent = nullptr);
    signals:
        void gotEmailRecipients(std::list<std::string> emailRecipients);
    private:
        std::list<std::pair<std::size_t, QPushButton*>> recipientRemovalButtons;
        std::unordered_set<std::string> recipientsAccountedFor;
};