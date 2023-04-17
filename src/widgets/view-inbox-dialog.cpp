#include "view-inbox-dialog.hpp"

#include <QGridLayout>
#include <QDesktopServices>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <set>

ViewInboxDialog::ViewInboxDialog()
    : inboxesPopulated({{false,{}}, {true,{}}})
{
    setModal(true);
}

void ViewInboxDialog::allInboxesPopulatedSignal() {
    delete dialogLayout;
    dialogLayout = new QGridLayout;
    setLayout(dialogLayout);

    std::set<Inbox::iterator::value_type> orderedMailURLPairs;  
    for (const auto* inbox : inboxesPopulated.at(true)) {
        auto itPtr = inbox->begin();
        auto endPtr = inbox->end();
        for (auto& it = *itPtr; it != *endPtr; ++it) {
            orderedMailURLPairs.insert(*it);
        }
    }

    auto* mailWidget = new QWidget;
    mailWidget->setBackgroundRole(QPalette::Base);
    mailWidget->setAutoFillBackground(true);
    auto* mailLayout = new QGridLayout(mailWidget);
    auto* mailScrollArea = new QScrollArea;
    mailScrollArea->setWidget(mailWidget);
    mailScrollArea->setWidgetResizable(true);
    dialogLayout->addWidget(mailScrollArea, 0, 0);
    int workingRow = 0;
    for (auto rit = orderedMailURLPairs.rbegin(); rit != orderedMailURLPairs.rend(); ++rit) {
        const auto& mail = rit->first;
        QString url = QString::fromStdString(rit->second);

        std::string buttonText = std::string("From: ") + mail.from + std::string(";        To: ") + mail.to
                               + std::string("\nSubject: ") + mail.subject;
        auto* button = new QPushButton(QString::fromStdString(buttonText));
        connect(button, &QPushButton::clicked, [url]() {
            QDesktopServices::openUrl(QUrl(url));
        });
        mailLayout->addWidget(button, workingRow, 0);
        ++workingRow;
    }

    emit allInboxesPopulated();
}

void ViewInboxDialog::populateInboxes() {
    for (auto it = inboxesPopulated.at(false).begin(); it != inboxesPopulated.at(false).end(); ++it) {
        connect(*it, &Inbox::populatedInbox, [this, it]() {
            inboxesPopulated.at(true).push_back(*it);
            inboxesPopulated.at(false).erase(it);
            if (inboxesPopulated.at(false).size() == 0) {
                allInboxesPopulatedSignal();
            }
        });
    }
    for (auto it = std::next(inboxesPopulated.at(false).begin()); it != inboxesPopulated.at(false).end(); ++it) {
        (*std::prev(it))->populate();
    }
    (*std::prev(inboxesPopulated.at(false).end()))->populate();
}

void ViewInboxDialog::clearInboxes() {
    inboxesPopulated.at(false).clear();
    inboxesPopulated.at(true).clear();
}

void ViewInboxDialog::setInbox(const Inbox* inbox) {
    clearInboxes();
    inboxesPopulated.at(false).push_back(inbox);
}

void ViewInboxDialog::addInbox(const Inbox* inbox) {
    inboxesPopulated.at(false).push_back(inbox);
}