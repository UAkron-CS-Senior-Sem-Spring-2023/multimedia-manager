#pragma once

#include "../accounts/inbox.hpp"

#include <unordered_map>
#include <list>

#include <QDialog>
#include <QGridLayout>

class ViewInboxDialog : public QDialog {
    Q_OBJECT

    public:
        ViewInboxDialog();

        void clearInboxes();
        void setInbox(const Inbox* inbox);
        void addInbox(const Inbox* inbox);
        void populateInboxes();
    signals:
        void allInboxesPopulated() const;
    private:
        void allInboxesPopulatedSignal();

        QGridLayout* dialogLayout = nullptr;

        std::unordered_map<bool, std::list<const Inbox*>> inboxesPopulated;
};