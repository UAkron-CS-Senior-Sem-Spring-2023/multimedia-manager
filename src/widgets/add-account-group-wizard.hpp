#pragma once

#include <unordered_map>

#include <QWizard>
#include <QGridLayout>

#include "accounts/account.hpp"

#include "set-list.hpp"

class AddAccountGroupWizard : public QWizard {
    Q_OBJECT

    public:
        AddAccountGroupWizard(QWidget* parent = nullptr);
    signals:
        void accountsUpdated();
    private:
        SetList<Account*> unincludedAccounts;
        SetList<Account*> includedAccounts;
        std::list<QPushButton*> unincludedButtons;
        std::list<QPushButton*> includedButtons;

        void updateAccounts(const SetList<Account*>& newAccounts);
        void flipButtonState(Account* account);
};