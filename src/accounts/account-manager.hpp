#pragma once

#include <list>
#include <memory>
#include <unordered_set>

#include <QObject>

#include "set-list.hpp"

#include "account.hpp"
#include "account-group.hpp"

class AccountManager : public QObject {
    Q_OBJECT

    public:
        static AccountManager& singleton();

        enum ManagingType {
            ACCOUNTS,
            ACCOUNT_GROUPS
        };
        static ManagingType INITIAL_MANAGING_TYPE;

        enum SourceRecipientType {
            SOURCE,
            RECIPIENT,
            DUAL,
            NONE
        };

        enum AccountVendor {
            GMAIL,
            OUTLOOK
        };


        void addSourceAccount(std::unique_ptr<SourceAccount> sourceAccount);
        void addRecipientAccount(std::unique_ptr<RecipientAccount> recipientAccount);
        void addDualAccount(std::unique_ptr<DualAccount> dualAccount);
        void addAccountGroup(std::unique_ptr<AccountGroup> accountGroup);

        SourceRecipientType accountSourceRecipientType(const Account* account) const;

        const SetList<Account*>& accounts() const;
    signals:
        void onAccountAdded(const Account* account);
        void onAccountsChanged(const SetList<Account*>& accounts);
        void onSourceAccountAdded(const SourceAccount* sourceAccount);
        void onSourceAccountsChanged(const SetList<SourceAccount*>& sourceAccounts);
        void onRecipientAccountAdded(const RecipientAccount* recipientAccount);
        void onRecipientAccountsChanged(const SetList<RecipientAccount*>& recipientAccounts);
        void onDualAccountAdded(const DualAccount* dualAccount);
        void onDualAccountsChanged(const SetList<DualAccount*>& dualAccounts);
        void onAccountGroupAdded(const AccountGroup* accountGroup);
        void onAccountGroupsChanged(const SetList<AccountGroup*>& accountGroups);
    private:
        static std::unique_ptr<AccountManager> singleton_;

        std::list<std::unique_ptr<Account>> accountsOwned;
        std::unordered_map<const Account*, SourceRecipientType> accountSourceRecipientTypes_;
        SetList<Account*> accounts_;
        SetList<SourceAccount*> sourceAccounts_;
        SetList<RecipientAccount*> recipientAccounts_;
        SetList<DualAccount*> dualAccounts_;

        std::list<std::unique_ptr<AccountGroup>> accountGroupsOwned;
        SetList<AccountGroup*> accountGroups_;
        
};